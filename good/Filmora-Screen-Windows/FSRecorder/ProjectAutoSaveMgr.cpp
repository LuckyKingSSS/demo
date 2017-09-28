#include "stdafx.h"
#include "ProjectAutoSaveMgr.h"

#include "RecorderViewsMgr.h"
#include "FSRecorder.h"
#include "Communicator.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/GlobalDefine.h"
#include "inc_FSCommonlib/ProjectSavingMgr.h"
#include <QtXml/QtXml>

#include "inc_FSCommonlib/ExternalAPI.h"

#include "FSRecorder.h"
#include "RecorderViewsMgr.h"

#define  ElemNodeText_UnTitle		"Untitle"


ProjectAutoSaveMgr* ProjectAutoSaveMgr::GetInstance()
{
    static ProjectAutoSaveMgr mgr(0);
    return &mgr;
}


void ProjectAutoSaveMgr::LoadProject(ProjectFileInfo *info)
{
    ProjPreproccessor::GetInstance()->SetLoadFromEditor(false);
    if (info == nullptr)
    {
        // Untitle自动保存
        GetMgr(Mgr_UntitleSave)->Start();
    }
    else
    {
        ProjectFileInfo *curInfo = nullptr;
        auto untitleInfo = m_autoSaveMgrMap[Mgr_UntitleSave]->GetUntitleProjectInfo();
        if (untitleInfo != nullptr && untitleInfo->filepath == info->filepath)
        {
            // 打开了Untitle工程
            ProjectFilesMgr::GetInstance()->SetCurrent(untitleInfo);
            ProjPreproccessor::GetInstance()->LoadProject(untitleInfo->filepath);

            GetMgr(Mgr_UntitleSave)->Start();
            ProjectMgr::GetInstance()->SetNewProject(true);
            ProjectMgr::GetInstance()->SetContentChanged(true);
        }
        else
        {
            auto button = LoadNewProject(info->filepath, false);
            if (button != FSMessageBox::TitleBar_Close)
            {
                if (button == FSMessageBox::No)
                {
                    // 打开双击工程
                    ProjectFilesMgr::GetInstance()->SetCurrent(info);
                    ProjPreproccessor::GetInstance()->LoadProject(info->filepath);
                }
                // 开始自动保存
                GetMgr(Mgr_TitleSave)->Start();
            }
        }


    }

}

void ProjectAutoSaveMgr::LoadEditorProjectFromEditor(QString projectPath)
{
    
    _LoadProjectFromEditor(projectPath, false);
}

void ProjectAutoSaveMgr::LoadRecorderProjectFromEditor(QString projectPath)
{
    _LoadProjectFromEditor(projectPath, true);

}

FSMessageBox::Button ProjectAutoSaveMgr::LoadNewProject(QString projectPath, bool recorderProjectFromEditor)
{
    // 是否打开更新的工程
    FSMessageBox::Button button = FSMessageBox::No;

    GetMgr(Mgr_TitleSave)->LoadProjectInfo(ProjectPath2Directory(projectPath));
    auto titleInfo = GetMgr(Mgr_TitleSave)->GetUntitleProjectInfo();
    if (titleInfo != nullptr)
    {

        button = (FSMessageBox::Button)Communicator::GetInstance()->ShowMessageBox(sFilmoraScreen, tr("New Project found, Do you want to open new?"), FSMessageBox::Yes | FSMessageBox::No);
        if (button == FSMessageBox::Yes)
        {
            ProjectFilesMgr::GetInstance()->SetCurrent(titleInfo);
            ProjPreproccessor::GetInstance()->LoadProject(titleInfo->filepath, recorderProjectFromEditor);

            // 修改真实工程文件，保存时自动保存到此工程
            //ProjectMgr::GetInstance()->SetContentChanged(true);
            ProjectMgr::GetInstance()->SetProjectPath(projectPath);
        }
    }


    return button;
}


void ProjectAutoSaveMgr::_LoadProjectFromEditor(QString projectPath, bool recorderProjectFromEditor)
{

    ProjPreproccessor::GetInstance()->SetLoadFromEditor(true);
    auto button = LoadNewProject(projectPath, recorderProjectFromEditor);

    if (button != FSMessageBox::TitleBar_Close)
    {
        ProjectFileInfo *info = ProjPreproccessor::GetInstance()->AddProj2Recent(projectPath);

        if (button == FSMessageBox::No)
        {
            // 打开双击工程
            ProjectFilesMgr::GetInstance()->SetCurrent(info);
            ProjPreproccessor::GetInstance()->LoadProject(info->filepath);
        }
        // 开始自动保存
        GetMgr(Mgr_TitleSave)->Start();
    }


}

bool ProjectAutoSaveMgr::SaveAsNew(const QString newpath)
{

    auto LoadProjectInfoFun = [&](){
        GetMgr(Mgr_TitleSave)->LoadProjectInfo(ProjectPath2Directory(newpath));
        GetMgr(Mgr_TitleSave)->Finish();
        GetMgr(Mgr_TitleSave)->LoadProjectInfo(ProjectPath2Directory(newpath));
    };

    int type = GetCurrentMgrType();
    if (type == Mgr_UntitleSave)
    {
        GetMgr(Mgr_UntitleSave)->Finish();

        LoadProjectInfoFun();
        GetMgr(Mgr_TitleSave)->Start();
    }
    else if (type == Mgr_TitleSave)
    {
        //// 修改自动保存工程untitle = false
        LoadProjectInfoFun();
    }

    return true;
}

bool ProjectAutoSaveMgr::Save()
{
    int type = GetCurrentMgrType();
    if (type == Mgr_TitleSave)
    {

        GetMgr(Mgr_TitleSave)->SetUntitle(false);
        GetMgr(Mgr_TitleSave)->Save(GetMgr(Mgr_TitleSave)->GetProjectInfoXmlPath());
    }

    return true;
}

void ProjectAutoSaveMgr::QuitDontSave()
{
    if (GetCurrentMgr())
    {
        GetCurrentMgr()->Finish();
    }
}



ProjectFileInfo* ProjectAutoSaveMgr::GetUntitleProjectInfo()
{
    return GetMgr(Mgr_UntitleSave)->GetUntitleProjectInfo();
}

ProjectAutoSaveBase* ProjectAutoSaveMgr::GetCurrentMgr()
{
    for (auto temp: m_autoSaveMgrMap)
    {
        if (temp->IsStart())
        {
            return temp;
        }
    }
    return nullptr;
}

ProjectAutoSaveMgr::MgrType ProjectAutoSaveMgr::GetCurrentMgrType()
{
    if (GetCurrentMgr())
        return (ProjectAutoSaveMgr::MgrType)GetCurrentMgr()->GetType();
    return Mgr_Unknow;
}

ProjectAutoSaveMgr::ProjectAutoSaveMgr(QObject *parent)
    : QObject(parent)
{
    m_autoSaveMgrMap.insert(Mgr_UntitleSave, new UntitleAutoSave(this));
    m_autoSaveMgrMap.insert(Mgr_TitleSave, new TitleAutoSaver(this));

}

ProjectAutoSaveMgr::~ProjectAutoSaveMgr()
{

}

ProjectAutoSaveBase* ProjectAutoSaveMgr::GetMgr(MgrType type)
{
    Q_ASSERT(m_autoSaveMgrMap.contains((int)type));
    return m_autoSaveMgrMap[type];
}


QString ProjectAutoSaveMgr::ProjectPath2Directory(QString path)
{
    return path + "ect";
}

//////////////////////////////////////////////////////////////////////////
ProjectAutoSaveBase::ProjectAutoSaveBase(ProjectAutoSaveMgr::MgrType type, QObject *parent)
:QObject(parent)
{
    m_maxProjectFileCount = 30;
    m_currentData = nullptr;
    m_mgrType = type;
    m_bUntitle = false;

    m_timer = new QTimer(this);
    int testTime = qMax(5, FSGetSettingsTest("ProjectAutoSaveTime", 5 * 60).toInt()) * 1000;
    m_timer->setInterval(testTime);
    //m_timer->setInterval(5 * 60 * 1000);
    connect(m_timer, &QTimer::timeout, this, [&](){
        if (ProjectMgr::GetInstance()->IsContentChanged() && !Communicator::GetInstance()->IsSaving())
        {
            ProjectSavingMgr::GetInstance()->WaitFinishSave();
            ProjectSavingMgr::GetInstance()->SetSaving(true);
            AutoSave();
            ProjectSavingMgr::GetInstance()->SetSaving(false);

        }
    });
}

ProjectAutoSaveBase::~ProjectAutoSaveBase()
{
    qDeleteAll(m_datas);
    if (m_currentData)
        delete m_currentData;
}

void ProjectAutoSaveBase::Start()
{
    m_timer->start();
}

void ProjectAutoSaveBase::Finish()
{
    m_timer->stop();
    SetUntitle(false);
    Save(GetProjectInfoXmlPath());
}



bool ProjectAutoSaveBase::IsStart()
{
    return m_timer->isActive();
}

ProjectFileInfo* ProjectAutoSaveBase::GetUntitleProjectInfo()
{
    if (IsUntitle())
    {
        return m_currentData;
    }
    return nullptr;
}


void ProjectAutoSaveBase::LoadProjectInfo(QString projectDir)
{
    ClearProjectInfo();

    m_projectInfoDirectory = projectDir;
    m_projectInfoXmlPath = projectDir + "/UntitleProjects.xml";
    m_untitleProjectFilePath = projectDir + "/Untitle Project.wfsproj";
   
    QDir dir(projectDir);
    if (!dir.exists())
    {
        dir.mkpath(projectDir);
    }
    else
    {
        Load(m_projectInfoXmlPath);

        // 创建untitle
        if (IsUntitle() && !m_datas.isEmpty())
        {
            auto lastData = m_datas.last();
            QFile::remove(m_untitleProjectFilePath);
            bool retvalue = QFile::copy(lastData->filepath, m_untitleProjectFilePath);
            if (retvalue)
            {
                m_currentData = new ProjectFileInfo;
                m_currentData->filepath = m_untitleProjectFilePath;
                m_currentData->savetime = lastData->savetime;
                m_currentData->imgpath = lastData->imgpath;
            }
            else
            {
                SetUntitle(false);
            }
        }
    }

}

void ProjectAutoSaveBase::ClearProjectInfo()
{
    m_projectInfoDirectory.clear();
    m_projectInfoXmlPath.clear();
    m_untitleProjectFilePath.clear();
    //m_mgrType = ProjectAutoSaveMgr::Mgr_Unknow;

    SetUntitle(false);

    if (m_currentData)
    {
        delete m_currentData;
        m_currentData = nullptr;
    }

    qDeleteAll(m_datas);
    m_datas.clear();
}

QString ProjectAutoSaveBase::GetProjectInfoDirectory()
{
    return m_projectInfoDirectory;
}

QString ProjectAutoSaveBase::GetProjectInfoXmlPath()
{
    return m_projectInfoXmlPath;
}

QString ProjectAutoSaveBase::GetUnititleProjectFilePath()
{
    return m_untitleProjectFilePath;
}

int ProjectAutoSaveBase::GetType()
{
    return m_mgrType;
}



void ProjectAutoSaveBase::AutoSave()
{
    QString projectDir = GetProjectInfoDirectory();
    QString  imgPath = projectDir + QString("/%1.png").arg(::GetTickCount());

    if (!ProjectMgr::GetInstance()->IsEditorProject())
    {
        auto thumbnail = ((PreviewPage*)RecorderViewsMgr::GetInstance()->GetFSRecorderMainView()->GetPageView(FSRecorderMainView::Page_Preview))->GetFirstItemThumbnail();
        thumbnail.save(imgPath);
    }
    else
    {
        QString tempPath = Communicator::GetInstance()->GetEditorThumbnail();
        QFile::copy(tempPath, imgPath);
        QFile::remove(tempPath);
    }

    //QString newProjectPaht = QString("%1/Untitle Project %2.wfsproj").arg(projectDir).arg(GetProjectPathNewNo());
    QString newProjectPaht = QString("%1/Untitle Project %2.wfsproj").arg(projectDir).arg(ProjectFileInfo::CurrentTimeString());
    if (ProjectMgr::GetInstance()->AutoSaveNew(newProjectPaht))
    {
        ProjectFileInfo *newData = new ProjectFileInfo;
        newData->filepath = newProjectPaht;
        newData->imgpath = imgPath;
        newData->savetime = newData->CurrentTimeString();
        AddNewAutoProject(newData);
        SetUntitle(true);
        Save(GetProjectInfoXmlPath());
    }

}

//
//int ProjectAutoSaveBase::GetProjectPathNewNo()
//{
//    int No = 0;
//    for (auto temp : m_datas)
//    {
//        auto splits = QFileInfo(temp->filepath).baseName().split(' ');
//        if (!splits.isEmpty())
//        {
//            bool ok = false;
//            int curNo = splits.last().toInt(&ok);
//            if (!ok)
//                curNo = 0;
//            if (No < curNo)
//            {
//                No = curNo;
//            }
//        }
//
//    }
//    return ++No;
//}



void ProjectAutoSaveBase::Load(QString path)
{
    QDomDocument doc;
    if (!XmlOperation::OpenXmlFile(doc, path))
        return;

    QDomElement rootElem = doc.documentElement();

    auto untitleNodes = rootElem.elementsByTagName(ElemNodeText_UnTitle);
    if (!untitleNodes.isEmpty())
    {
        m_bUntitle = untitleNodes.at(0).toElement().text().toInt();
    }
    auto nodes = rootElem.elementsByTagName(ElemNodeText_ProjectFile);
    for (int i = 0; i < nodes.size(); ++i)
    {
        auto elem = nodes.at(i).toElement();

        ProjectFileInfo *data = new ProjectFileInfo;
        auto tempElem = elem.firstChildElement(ElemNodeText_FilePath);
        if (!tempElem.isNull())
        {
            data->filepath = tempElem.text();
        }

        tempElem = elem.firstChildElement(ElemNodeText_SaveTime);
        if (!tempElem.isNull())
        {
            data->savetime = tempElem.text();
        }

        tempElem = elem.firstChildElement(ElemNodeText_ThumbnailPath);
        if (!tempElem.isNull())
        {
            data->imgpath = tempElem.text();
        }
        auto sameIter = std::find_if(m_datas.begin(), m_datas.end(), [&](ProjectFileInfo *info){
            return data->filepath == info->filepath;
        });
        if (sameIter == m_datas.end())
        {
            m_datas.push_back(data);

        }
        else
        {
            delete data;
        }

    }

    std::sort(m_datas.begin(), m_datas.end(), [&](const ProjectFileInfo *l, const ProjectFileInfo *r){
        return l->savetime < r->savetime;
    });

}

void ProjectAutoSaveBase::Save(QString path)
{
    QDomDocument doc;
    auto rootElem = doc.createElement(ElemNodeText_ProjectFileAutoSave);
    doc.appendChild(rootElem);

    XmlOperation::AddXmlElement(doc, rootElem, ElemNodeText_UnTitle, IsUntitle() ? "1": "0");

    for (auto info : m_datas)
    {
        auto projectElem = doc.createElement(ElemNodeText_ProjectFile);
        rootElem.appendChild(projectElem);
        XmlOperation::AddXmlElement(doc, projectElem, ElemNodeText_FilePath, info->filepath);
        XmlOperation::AddXmlElement(doc, projectElem, ElemNodeText_ThumbnailPath, info->imgpath);
        XmlOperation::AddXmlElement(doc, projectElem, ElemNodeText_SaveTime, QString("%1").arg(info->savetime));
    }

    XmlOperation::SaveXmlContent2File(doc, path);

}



bool ProjectAutoSaveBase::IsUntitle()
{
    return m_bUntitle;
}

void ProjectAutoSaveBase::SetUntitle(bool untitle)
{
    m_bUntitle = untitle;
}

void ProjectAutoSaveBase::AddNewAutoProject(ProjectFileInfo *info)
{
    m_datas.push_back(info);
    while (m_datas.size() > m_maxProjectFileCount)
    {
        RemoveAutoProject(m_datas.first());
    }
}

void ProjectAutoSaveBase::RemoveAutoProject(ProjectFileInfo *info)
{
    Q_ASSERT(info);
    for (auto temp : m_datas)
    {
        if (temp == info)
        {
            m_datas.removeOne(temp);
            QFile::remove(info->filepath);
            QFile::remove(info->imgpath);
            delete info;
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
UntitleAutoSave::UntitleAutoSave(QObject *parent)
:ProjectAutoSaveBase(ProjectAutoSaveMgr::Mgr_UntitleSave, parent)
{
    m_currentData = nullptr;

    QString projectSaveDir = QApplication::applicationDirPath() + "/ProjectFilesAutoSave";
    QDir dir(projectSaveDir);
    if (!dir.exists())
    {
        dir.mkpath(projectSaveDir);
    }
    LoadProjectInfo(projectSaveDir);

}


TitleAutoSaver::TitleAutoSaver(QObject *parent)
:ProjectAutoSaveBase(ProjectAutoSaveMgr::Mgr_TitleSave,parent)
{

}

