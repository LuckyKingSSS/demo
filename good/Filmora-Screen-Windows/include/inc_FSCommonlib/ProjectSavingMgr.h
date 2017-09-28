#ifndef PROJECTSAVINGMGR_H
#define PROJECTSAVINGMGR_H

#include "inc_FSCommonlib/fscommonlib_global.h"


#include <QObject>

// Recorder和Editor间通信 和 自动保存通信相干扰的问题
// 增加全局锁，确保自动保存期间，其它通信需要等待
class FSCOMMONLIB_EXPORT ProjectSavingMgr : public QObject
{
    Q_OBJECT

public:
    static ProjectSavingMgr* GetInstance();
    void Release();

    bool IsSaving();
    void SetSaving(bool saving);

    void WaitFinishSave(int msecs = 0);

public:
    ProjectSavingMgr(QObject *parent);
    ~ProjectSavingMgr();

private:
    QSharedMemory *memory;
    QString m_mutexName;
};

#endif // PROJECTSAVINGMGR_H
