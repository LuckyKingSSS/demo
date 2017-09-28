#ifndef PROPERTYPANELMANAGER_H
#define PROPERTYPANELMANAGER_H

#include "inc_FPropertyPanel/fpropertypanel_global.h"
#include <QtCore/QObject>

class FPROPERTYPANEL_EXPORT PropertyPanelManager : public QObject
{
    Q_OBJECT

public:
	PropertyPanelManager(QObject *parent);
	~PropertyPanelManager();

private:
    
};

#endif // PROPERTYPANLEMANAGER_H
