#include "stdafx.h"
#include "MKCollecter.h"
#include "MKCollecterPrivate.h"
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#include <QThread>

#include "windows.h"
static HINSTANCE hDllInst = NULL;	// DLL模块实例句柄
static HHOOK	hLLKeyboard = NULL;
static HHOOK hMouse = NULL;


int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		hDllInst = hInstance;		
	return true;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		PMOUSEHOOKSTRUCT pMouseInfo = (PMOUSEHOOKSTRUCT)lParam;
		switch (wParam)
		{
			case WM_MOUSEMOVE:      
				emit MKCollecter::GetInstance()->sigMouseMove(QPoint(pMouseInfo->pt.x, pMouseInfo->pt.y));
				break;
            case WM_RBUTTONDOWN:
                emit MKCollecter::GetInstance()->sigRButtonDown(QPoint(pMouseInfo->pt.x, pMouseInfo->pt.y));
                break;
            case WM_RBUTTONUP:
                emit MKCollecter::GetInstance()->sigRButtonUp(QPoint(pMouseInfo->pt.x, pMouseInfo->pt.y));
                break;
			case WM_LBUTTONDOWN:	
                emit MKCollecter::GetInstance()->sigLButtonDown(QPoint(pMouseInfo->pt.x, pMouseInfo->pt.y));
                break;
			case WM_LBUTTONUP:
				//qDebug() << pMouseInfo->pt.x << " " << pMouseInfo->pt.y;
				emit MKCollecter::GetInstance()->sigLButtonUp(QPoint(pMouseInfo->pt.x, pMouseInfo->pt.y));
				break;
			case WM_LBUTTONDBLCLK:	break;
		}
	}
	return CallNextHookEx(hMouse, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION )
	{
		auto control = qobject_cast<MKCollecterPrivate*>(MKCollecter::GetInstance());
		if ((p->flags & 0x80) == 0)
		{
			int mk = 0;
			if (GetKeyState(VK_CONTROL) & 0x8000) mk |= MKCollecter::ModifyKey_Control;
			if (GetKeyState(VK_SHIFT) & 0x8000) mk |= MKCollecter::ModifyKey_Shift;
            if (GetKeyState(VK_MENU) & 0x8000) mk |= MKCollecter::ModifyKey_Alt;
            if (GetKeyState(VK_LWIN) & 0x8000) mk |= MKCollecter::ModifyKey_Win;
            if (GetKeyState(VK_RWIN) & 0x8000) mk |= MKCollecter::ModifyKey_Win;
			
			control->SetKey(p->vkCode, mk);
			emit control->sigKeyPressed(p->vkCode, mk);
		}
		else
		{
			control->SetKey(VK_APPS, 0);
			emit control->sigKeyUp(p->vkCode);
			//qDebug() << "Up: "<<p->vkCode;
		}
		// 不向下传递钩子
		if (!control->GetEnalbedHotKeyDeliver() && control->GetHotKeys().contains(control->GetHotKeyString()))
			return 1;
	}
	return CallNextHookEx(hLLKeyboard, nCode, wParam, lParam);
}

bool SetMouseHook(bool bInstall)
{
//#ifdef _DEBUG
//	bInstall = false;
//#endif
	bool bSucc = false;
	if (bInstall && !hMouse)
	{
		hMouse = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, hDllInst, 0);
		bSucc =  hMouse != nullptr;
	}
	if (!bInstall && hMouse)
	{
		bSucc = UnhookWindowsHookEx(hMouse);
		hMouse = NULL;
	}
	return bSucc;
}

bool SetKeyboardHook(bool bInstall)
{
	bool bSucc = false;
	if (bInstall && !hLLKeyboard)
	{
		hLLKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hDllInst, 0);
		bSucc =  (hLLKeyboard != nullptr);

		qDebug() << "SetWindowsHookEx: " << bSucc;
	}

	if (!bInstall && hLLKeyboard)
	{
		bSucc =  UnhookWindowsHookEx(hLLKeyboard);
		hLLKeyboard = NULL;
		qDebug() << "UnhookWindowsHookEx: " << bSucc;
	}
	return bSucc;
}

MKCollecter* MKCollecter::GetInstance()
{
	static MKCollecterPrivate ins;
	return &ins;
}

MKCollecterPrivate::MKCollecterPrivate()
{
	m_enalbedHotKeyDeliver = false;

	QThread *pThread = new QThread(this);
	moveToThread(pThread);
	pThread->start();
}

MKCollecterPrivate::~MKCollecterPrivate()
{

}

bool MKCollecterPrivate::SetMouseHook(bool bInstall)
{
#ifndef _DEBUG
    QMetaObject::invokeMethod(this, "_SetMouseHook", Qt::QueuedConnection, Q_ARG(bool, bInstall));
#endif
    return true;
}

bool MKCollecterPrivate::SetKeyboardHook(bool bInstall)
{
#ifndef _DEBUG
	QMetaObject::invokeMethod(this, "_SetKeyboardHook", Qt::QueuedConnection, Q_ARG(bool, bInstall));
#endif
    return true;
}

bool MKCollecterPrivate::GetMouseHook()
{
    return hMouse;
}

bool MKCollecterPrivate::GetKeyboardHook()
{
    return hLLKeyboard;
}



bool MKCollecterPrivate::_SetMouseHook(bool bInstall)
{
	return ::SetMouseHook(bInstall);
}

bool MKCollecterPrivate::_SetKeyboardHook(bool bInstall)
{
	bool bSucc = ::SetKeyboardHook(bInstall);
	return bSucc;
}

QString MKCollecterPrivate::GetControlString(int mk)
{
	QString text;
	if (mk & MKCollecter::ModifyKey_Shift)
	{
		text += "Shift+";
	}
	if (mk & MKCollecter::ModifyKey_Control)
	{
		text += "Ctrl+";
	}
	if (mk & MKCollecter::ModifyKey_Alt)
	{
		text += "Alt+";
	}
    if (mk & MKCollecter::ModifyKey_Win)
    {
        text += "Win+";
    }
	return text;
}

QString MKCollecterPrivate::GetKeyString(int key)
{
	QString text;
	switch (key)
	{
	case '0': text = "0"; break;
	case '1': text = "1"; break;
	case '2': text = "2"; break;
	case '3': text = "3"; break;
	case '4': text = "4"; break;
	case '5': text = "5"; break;
	case '6': text = "6"; break;
	case '7': text = "7"; break;
	case '8': text = "8"; break;
	case '9': text = "9"; break;

	case 'A': text = "A"; break;
	case 'B': text = "B"; break;
	case 'C': text = "C"; break;
	case 'D': text = "D"; break;
	case 'E': text = "E"; break;
	case 'F': text = "F"; break;
	case 'G': text = "G"; break;
	case 'H': text = "H"; break;
	case 'I': text = "I"; break;
	case 'J': text = "J"; break;
	case 'K': text = "K"; break;
	case 'L': text = "L"; break;
	case 'M': text = "M"; break;
	case 'N': text = "N"; break;
	case 'O': text = "O"; break;
	case 'P': text = "P"; break;
	case 'Q': text = "Q"; break;
	case 'R': text = "R"; break;
	case 'S': text = "S"; break;
	case 'T': text = "T"; break;
	case 'U': text = "U"; break;
	case 'V': text = "V"; break;
	case 'W': text = "W"; break;
	case 'X': text = "X"; break;
	case 'Y': text = "Y"; break;
	case 'Z': text = "Z"; break;

	case VK_F1: text = "F1"; break;
	case VK_F2: text = "F2"; break;
	case VK_F3: text = "F3"; break;
	case VK_F4: text = "F4"; break;
	case VK_F5: text = "F5"; break;
	case VK_F6: text = "F6"; break;
	case VK_F7: text = "F7"; break;
	case VK_F8: text = "F8"; break;
	case VK_F9: text = "F9"; break;
	case VK_F10: text = "F10"; break;
	case VK_F11: text = "F11"; break;
	case VK_F12: text = "F12"; break;
	case VK_F13: text = "F13"; break;
	case VK_F14: text = "F14"; break;
	case VK_F15: text = "F15"; break;
	case VK_F16: text = "F16"; break;
	case VK_F17: text = "F17"; break;
	case VK_F18: text = "F18"; break;
	case VK_F19: text = "F19"; break;
	case VK_F20: text = "F20"; break;

	case VK_LCONTROL: case VK_RCONTROL: text = "Ctrl"; break;
	case VK_LSHIFT: case VK_RSHIFT: text = "Shift"; break;
	case VK_LMENU: case VK_RMENU: text = "Alt"; break;
    case VK_ESCAPE: text = "ESC"; break;

	case 0x9: text = "Tab"; break;
	case VK_CAPITAL: text = "Caps Lock"; break;
	case VK_OEM_3: text = "~"; break;

	case VK_OEM_MINUS: text = "-"; break;
	case VK_OEM_PLUS: text = "+"; break;
	case 0x8: text = "Backspace"; break;
	case VK_OEM_4: text = "["; break;
	case VK_OEM_6: text = "]"; break;
	case VK_OEM_5: text = "\\"; break;
	case VK_OEM_1: text = ";"; break;
	case VK_OEM_7: text = "'"; break;
	case VK_OEM_COMMA: text = ","; break;
	case VK_OEM_PERIOD: text = "."; break;
	case VK_OEM_2: text = "/"; break;
	
	case VK_INSERT: text = "Ins"; break;
	case VK_HOME: text = "Home"; break;
	case VK_PRIOR: text = "PgUp"; break;
	case VK_DELETE: text = "Del"; break;
	case VK_END: text = "End"; break;
	case VK_NEXT: text = "PgDown"; break;

	case VK_LEFT: text = "Left"; break;
	case VK_UP: text = "Up"; break;
	case VK_RIGHT: text = "Right"; break;
	case VK_DOWN: text = "Down"; break;

	case VK_SPACE: text = "Space"; break;
	case 0xd: text = "Enter"; break;
		
	case VK_SNAPSHOT: text = "PrtScn"; break;
	case VK_SCROLL: text = "Scroll Lock"; break;
	case VK_PAUSE: text = "Pause"; break;

	case VK_NUMLOCK: text = "Num Lock"; break;
	case VK_DIVIDE: text = "/"; break;
	case VK_MULTIPLY: text = "*"; break;
	case VK_SUBTRACT: text = "-"; break;
	case VK_ADD: text = "+"; break;
	case VK_DECIMAL: text = "."; break;

	case 0xc: text = "5"; break;
	case VK_NUMPAD0: text = "0"; break;
	case VK_NUMPAD1: text = "1"; break;
	case VK_NUMPAD2: text = "2"; break;
	case VK_NUMPAD3: text = "3"; break;
	case VK_NUMPAD4: text = "4"; break;
	case VK_NUMPAD5: text = "5"; break;
	case VK_NUMPAD6: text = "6"; break;
	case VK_NUMPAD7: text = "7"; break;
    case VK_NUMPAD8: text = "8"; break; 
    case VK_NUMPAD9: text = "9"; break;

	case VK_LWIN: case VK_RWIN: text = "Win"; break;
	case VK_APPS: text = "App"; break;

		
	default:
		text = "Unknow Key";
	}
	return text;
}

QString MKCollecterPrivate::GetHotKeyString()
{
	return GetControlString(m_mk) + GetKeyString(m_key);

}
void MKCollecterPrivate::EnalbedHotKeyDeliver(bool enabled)
{
	m_enalbedHotKeyDeliver = enabled;
}

bool MKCollecterPrivate::GetEnalbedHotKeyDeliver()
{
	return m_enalbedHotKeyDeliver;
}



void MKCollecterPrivate::SetKey(int key, int mk)
{
	m_key = key;
	m_mk = mk;
}

void MKCollecterPrivate::AddHotKey(QString key)
{
	if (!m_hotkeys.contains(key))
		m_hotkeys.push_back(key);
}

void MKCollecterPrivate::ClearHotKey()
{
	m_hotkeys.clear();
}

void MKCollecterPrivate::RemoveHotKey(QString key)
{
	m_hotkeys.removeOne(key);
}
QList<QString> MKCollecterPrivate::GetHotKeys()
{
	return m_hotkeys;
}
