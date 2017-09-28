#ifndef AUDIODISPLAYVIEW_H
#define AUDIODISPLAYVIEW_H


class AudioIndicator : public QWidget
{
    Q_OBJECT
public:
    AudioIndicator(QWidget *parent);
    ~AudioIndicator();

    void SetChannelValue(int left, int right);

protected:
    void paintEvent(QPaintEvent *e);

private:   
    QPoint GetCenterPoint(int radius, int margin, int indexH, int indexV);
    void FillPathData(QList<QPainterPath> &list, QPoint center, int raduis, float precent);


private:
    int mILeftChannel;
    int mIRightChannel;   
};

#endif // AUDIODISPLAYVIEW_H
