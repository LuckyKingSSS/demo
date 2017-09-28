#ifndef FCOMMONLIB_H
#define FCOMMONLIB_H


#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QAction>

#include <QtWidgets/QMenu>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>



#include "inc_FCommonLib/fcommonlib_global.h"
#include "inc_CommonLib/CommonWidgets.h"


// Í¼Æ¬°´Å¥
class FCOMMONLIB_EXPORT FImageButton :public BaseButton
{
    Q_OBJECT
public:
    FImageButton(int id, const QString &objectname, QWidget *parent);
    FImageButton(const QString &objectname, QWidget *parent);

};


// ×´Ì¬°´Å¥
class FCOMMONLIB_EXPORT FStateButton : public BaseStateButton
{
    Q_OBJECT
public:
    FStateButton(int id, const QString &objectname, QWidget *parent);

};


// ÎÄ×Ö°´Å¥
class FCOMMONLIB_EXPORT FButton : public QPushButton
{
    Q_OBJECT
public:
    FButton(QWidget *parent);

};


class FCOMMONLIB_EXPORT FAction : public QAction
{
    Q_OBJECT
public:
    FAction(QObject *parent);

};

class FCOMMONLIB_EXPORT FMenu : public QMenu
{
    Q_OBJECT
public:
    FMenu(QWidget *parent);

};

class FCOMMONLIB_EXPORT FLabel : public QLabel
{
    Q_OBJECT
public:
    FLabel(QWidget *parent);


};


class FCOMMONLIB_EXPORT FListWidget :public QListWidget
{
    Q_OBJECT
public:
    FListWidget(QWidget *parent);
};

class FCOMMONLIB_EXPORT FLineEdit :public QLineEdit
{
    Q_OBJECT
public:
    FLineEdit(QWidget *parent);
};


class FCOMMONLIB_EXPORT FComboBox :public QComboBox
{
    Q_OBJECT
public:
    FComboBox(QWidget *parent);

    void SetAutoFixedMenuWidth(bool enabled);

protected:
    void FixedMenuWidth();
    void mousePressEvent(QMouseEvent *event);
private:
    bool m_autoFixedMenuWidth;
};


class FCOMMONLIB_EXPORT FSpinBox :public QSpinBox
{
    Q_OBJECT
public:
    FSpinBox(QWidget *parent);
};


class FCOMMONLIB_EXPORT FCheckBox :public QCheckBox
{
	Q_OBJECT
public:
	FCheckBox(QWidget *parent);
};

class FCOMMONLIB_EXPORT FRadioButton :public QRadioButton
{
	Q_OBJECT
public:
	FRadioButton(QWidget *parent);
};

class FCOMMONLIB_EXPORT FScrollArea :public QScrollArea
{
	Q_OBJECT
public:
	FScrollArea(QWidget *parent);
};
#endif // FCOMMONLIB_H
