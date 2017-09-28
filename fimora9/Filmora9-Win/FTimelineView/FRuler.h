#ifndef FRULER_H
#define FRULER_H

#include <QWidget>
class TimelineRuler;

class FRuler : public QWidget
{
    Q_OBJECT

public:
    FRuler(QWidget *parent);
    ~FRuler();

    TimelineRuler* Ruler();
protected:
    void resizeEvent(QResizeEvent *event);

private:
    TimelineRuler *m_pRuler;

};

#endif // FRULER_H
