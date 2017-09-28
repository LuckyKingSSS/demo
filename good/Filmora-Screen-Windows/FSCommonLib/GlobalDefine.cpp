#include "stdafx.h"
#include "inc_FSCommonlib/GlobalDefine.h"

QString GetCmdContent(const QString &msg, const QString &cmd)
{
	if (msg.contains(cmd))
	{
		int posCmd = msg.indexOf(cmd);
		int posDolor = msg.indexOf("|",posCmd);
		
		QString content = msg.mid(posCmd + cmd.length(), posDolor - posCmd - cmd.length());
		return content;
	}
	return "";
}

QString FSCOMMONLIB_EXPORT GetCmdContentFromEnd(const QString &msg, const QString &cmd)
{
    if (msg.contains(cmd))
    {
        int posCmd = msg.lastIndexOf(cmd);
        int posDolor = msg.indexOf("|", posCmd);

        QString content = msg.mid(posCmd + cmd.length(), posDolor - posCmd - cmd.length());
        return content;
    }
    return "";
}
