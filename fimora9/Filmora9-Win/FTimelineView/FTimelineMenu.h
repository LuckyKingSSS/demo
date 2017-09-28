#ifndef FTIMELINEMENU_H
#define FTIMELINEMENU_H

#include <QObject>

class FTracksView;
class FTimelineMenu : public QObject
{
    Q_OBJECT

public:
    FTimelineMenu(QObject *parent);
    ~FTimelineMenu();

    void Popup(QPoint pos);

    void RetranslateUi();

private slots:
    void slotDelete();
private:
    FTracksView *m_tracksView;


    QAction *m_pActDelete;

};

#endif // FTIMELINEMENU_H
