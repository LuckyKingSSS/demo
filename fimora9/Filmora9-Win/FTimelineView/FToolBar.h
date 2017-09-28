#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
class FImageButton;

class FToolBar : public QWidget
{
    Q_OBJECT
public:
    enum ToolBarType{ Undo, Redo, Split };

public:
    FToolBar(QWidget *parent);
    ~FToolBar();

Q_SIGNALS:
    void sigButtonClicked(int type);

private:
    void slotButtonClicked(int type);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    FImageButton *m_btnUndo;
    FImageButton *m_btnRedo;
    FImageButton *m_btnSplit;
};

#endif // TOOLBAR_H
