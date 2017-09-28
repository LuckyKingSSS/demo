#include "stdafx.h"
#include "inc_FSCommonlib/ProjectSavingMgr.h"

static ProjectSavingMgr *s_ProjectSavingMgr = nullptr;
ProjectSavingMgr* ProjectSavingMgr::GetInstance()
{
    if (s_ProjectSavingMgr == nullptr)
    {
        s_ProjectSavingMgr = new ProjectSavingMgr(0);
    }
    return s_ProjectSavingMgr;

}

void ProjectSavingMgr::Release()
{
    if (s_ProjectSavingMgr)
    {
        delete s_ProjectSavingMgr;
        s_ProjectSavingMgr = nullptr;
    }
}

bool ProjectSavingMgr::IsSaving()
{
    QSharedMemory memory(m_mutexName);
    bool isSaving = 0;
    if (memory.attach())
    {
        char to[2] = { 0 };
        memory.lock();
        char *from = static_cast<char*>(memory.data());
        memcpy(to, from, 1);
        isSaving = atoi(to);
        memory.unlock();
    }

    return isSaving;
}

void ProjectSavingMgr::SetSaving(bool saving)
{
    qDebug() << QString("SetSaving") + QApplication::applicationName() << (saving ? "1" : "0");
    QSharedMemory memory(m_mutexName);
    if (memory.attach())
    {
        auto byteArray = QString::number(saving?1:0).toLatin1();
        memory.lock();
        char *to = static_cast<char*>(memory.data());
        memcpy(to, byteArray.data(), 1);
        memory.unlock();
    }
}

void ProjectSavingMgr::WaitFinishSave( int msecs)
{
    if (msecs > 0)
    {
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();
        while (IsSaving() && elapsedTimer.elapsed() < msecs)
        {
            ::Sleep(100);
        }
    }
    else
    {
        while (IsSaving())
        {
            ::Sleep(100);
        }
    }
}


ProjectSavingMgr::ProjectSavingMgr(QObject *parent)
    : QObject(parent)
    , m_mutexName("SharedMemory_ProjectSavingMemory")
{

    memory = new QSharedMemory(m_mutexName);
    if (!memory->attach())
    {
        bool fff = memory->create(1);
        int i = 0;
    }

}

ProjectSavingMgr::~ProjectSavingMgr()
{
    if (memory->isAttached())
    {
        memory->detach();
    }
    delete memory;
}
