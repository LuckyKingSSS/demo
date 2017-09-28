@echo off

set SignPath=..\Signer
set BinPath=..\..\bin\x64\release
set RecorderExe=%BinPath%\FSRecorder.exe
set EditorExe=%BinPath%\FSEditor.exe
set InsExe=.\Output\Filmora Scrn^(x64^).exe

if "%1" == "_Recorder" (
rem  %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll "%RecorderExe%"
rem  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%RecorderExe%"
  
  %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%RecorderExe%"
%SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /v /as /a /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%RecorderExe%"
) 

if "%1" == "_Editor" (
	 %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%EditorExe%"
%SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /v /as /a /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%EditorExe%"

 rem %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll "%EditorExe%"
rem  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%EditorExe%"
)

if "%1" == "_Ins" (
%SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%InsExe%"
%SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /v /as /a /n "Wondershare Technology Co.,Ltd" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%InsExe%"

rem	%SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll "%InsExe%"
rem  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 "%InsExe%"
)