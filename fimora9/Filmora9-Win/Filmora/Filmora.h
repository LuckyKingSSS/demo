#ifndef FILMORA_H
#define FILMORA_H

#include <QtWidgets/QWidget>

class FTimelineView;

class Filmora : public QWidget
{
    Q_OBJECT

public:
    Filmora(QWidget *parent = 0);
    ~Filmora();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *m_matterView;
    QWidget *m_playerView;
    FTimelineView *m_timelineView;
};


#endif // FILMORA_H
