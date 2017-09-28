#ifndef FPREVIEWPLAYERHEADER_H
#define FPREVIEWPLAYERHEADER_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"

class FButton;
class FPreviewPlayerHeader : public FBaseWidget
{
	Q_OBJECT

public:
	FPreviewPlayerHeader(QWidget *parent);
	~FPreviewPlayerHeader();

private:
	void InitUI();
	void InitConnections();

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	FButton* m_UndoBtn;
	FButton* m_RedoBtn;
	FButton* m_ShowOriginalBtn;
	FButton* m_CompareMenuBtn;
};

#endif // FPREVIEWPLAYERHEADER_H
