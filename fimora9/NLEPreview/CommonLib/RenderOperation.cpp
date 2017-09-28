#include "stdafx.h"
#include "RenderOperation.h"
#include <QBuffer>

//using namespace Gdiplus;
/*
**  9格图缩放
*/
QPixmap* NinePatch(const QPixmap* pix,int iHorzSplit,int iVertSplit, int DstWidth, int DstHeight)
{
	if (pix == nullptr || pix == (QPixmap*)0x11){
		return nullptr;
	}
	int pixWidth = pix->width();
	int pixHeight = pix->height();

	QPixmap pix_1 = pix->copy(0, 0, iHorzSplit, iVertSplit);
	QPixmap pix_2 = pix->copy(iHorzSplit, 0, pixWidth-iHorzSplit*2, iVertSplit);
	QPixmap pix_3 = pix->copy(pixWidth-iHorzSplit, 0, iHorzSplit, iVertSplit);

	QPixmap pix_4 = pix->copy(0, iVertSplit, iHorzSplit, pixHeight-iVertSplit*2);
	QPixmap pix_5 = pix->copy(iHorzSplit, iVertSplit, pixWidth-iHorzSplit*2, pixHeight-iVertSplit*2);
	QPixmap pix_6 = pix->copy(pixWidth-iHorzSplit, iVertSplit, iHorzSplit, pixHeight-iVertSplit*2);

	QPixmap pix_7 = pix->copy(0, pixHeight-iVertSplit, iHorzSplit, iVertSplit);
	QPixmap pix_8 = pix->copy(iHorzSplit, pixHeight-iVertSplit, pixWidth-iHorzSplit*2, pixWidth - iHorzSplit*2);
	QPixmap pix_9 = pix->copy(pixWidth-iHorzSplit, pixHeight-iVertSplit, iHorzSplit, iVertSplit);

	pix_2 = pix_2.scaled(DstWidth-iHorzSplit*2,iVertSplit, Qt::IgnoreAspectRatio);//保持高度拉宽
	pix_4 = pix_4.scaled(iHorzSplit, DstHeight-iVertSplit*2, Qt::IgnoreAspectRatio);//保持宽度拉高
	pix_5 = pix_5.scaled(DstWidth-iHorzSplit*2,DstHeight-iVertSplit*2, Qt::IgnoreAspectRatio);
	pix_6 = pix_6.scaled(iHorzSplit, DstHeight-iVertSplit*2, Qt::IgnoreAspectRatio);//保持宽度拉高
	pix_8 = pix_8.scaled(DstWidth-iHorzSplit*2, iVertSplit);//保持高度拉宽


	QPixmap* resultImg =new QPixmap(DstWidth, DstHeight);

	if (!resultImg->isNull()) {
		QPainter* painter = new QPainter;
		painter->begin(resultImg);
		painter->drawPixmap(0,0,pix_1);
		painter->drawPixmap(iHorzSplit, 0, pix_2);
		painter->drawPixmap(DstWidth-iHorzSplit,0,pix_3);

		painter->drawPixmap(0, iVertSplit, pix_4);
		painter->drawPixmap(iHorzSplit, iVertSplit, pix_5);
		painter->drawPixmap(DstWidth-iHorzSplit, iVertSplit, pix_6);

		painter->drawPixmap(0, DstHeight-iVertSplit, pix_7);
		painter->drawPixmap(iHorzSplit, DstHeight-iVertSplit, pix_8);
		painter->drawPixmap(DstWidth-iHorzSplit, DstHeight-iVertSplit, pix_9);
		painter->end();
		delete painter;
	}

	return resultImg;
}


void RenderOperation::DrawImage( QPainter& painter, QString &strImg, const QRect& rc)
{
	const QImage& img = QImage(strImg);

	if (img.isNull())
		return;

	painter.drawImage(rc.x(), rc.y(), img);
}

void RenderOperation::DrawImage(QPainter& painter, QString &strImg, const QRect& rc, const QRect& rcSource)
{
	const QImage& img = QImage(strImg);

	if (img.isNull())
		return;

	painter.drawImage(rc, img, rcSource);
}

void RenderOperation::DrawImage( QPainter& painter, QString &strImg, const QRect& rc, const QRect& rcSource, const QRect &rcCorner )
{
	QPixmap pxMap;	
	if (pxMap.load(strImg))
	{
		QPixmap pxSrc = pxMap.copy(rcSource);
		if (pxSrc.isNull())
			return;

		QPixmap *pix = NinePatch(&pxSrc, rcCorner.left(), rcCorner.top(),
			rc.width(), rc.height());
		if (pix)
		{
			painter.drawPixmap(rc, *pix);
		}
		delete pix;
	}
}

void RenderOperation::DrawImage( QPainter& painter,const QPixmap &pixMap, const QRect& rc, const QRect& rcSource, const QRect &rcCorner )
{
	if(pixMap.isNull())
		return;

 	QPixmap *pix = NinePatch(&pixMap, rcCorner.left(), rcCorner.top(),
 		rc.width(), rc.height());
 	if(pix)
 	{
 		painter.drawPixmap(rc, *pix);
 	}
 	delete pix;
}

void RenderOperation::DrawImage( QPainter& painter,const QPixmap &pixMap, const QRect& rc)
{
	if(pixMap.isNull())
		return;
	painter.drawPixmap(rc, pixMap);
}


void RenderOperation::DrawImage(QPainter & painter, QImage & img, const QRect & rc, const QRect & rcCorner)
{
	if ( img.isNull() )
	{
		return;
	}

	QPixmap pxSrc =  QPixmap::fromImage( img );
	QPixmap *pix = NinePatch(&pxSrc, rcCorner.left(), rcCorner.top(),
		rc.width(), rc.height());
	if( pix )
	{
		painter.drawPixmap(rc, *pix);
	}
	delete pix;
}


QPixmap * RenderOperation::GetNinePatch( QPixmap & sImg, const QRect & rcSrc, const QRect & rcDst, const QRect & rcCorner )
{
	if ( sImg.isNull() )
	{
		return NULL;
	}

	QPixmap pixImg = sImg.copy( rcSrc );
	if ( pixImg.isNull() )
	{
		return NULL;
	}

	QPixmap * newPix = NinePatch( &pixImg, rcCorner.left(), rcCorner.top(), rcDst.width(), rcDst.height() );
	return newPix;
}


void RenderOperation::DrawImage( QPainter& painter, char *pImageMemory, unsigned int imgSize, const QRect& rc, const QRect& rcSource )
{
	if(pImageMemory == NULL || imgSize <= 0 )	return;

	QBuffer buffer;
	buffer.setData(pImageMemory, imgSize);
	QImage image;
	image.loadFromData(buffer.buffer());
	if( (image.width() == rc.width()) && (image.height() == rc.height()) )
	{
		painter.drawImage(rc, image, rcSource);
	}
	else
	{
		QImage result = image.scaled(image.width(), image.height()).scaled(rc.width(), rc.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
		QRect rcSrc(0, 0, result.width(), result.height());
		painter.drawImage(rc, result, rcSrc);
	}
}


QImage RenderOperation::ScaledImage( QString & strImg, const QSize & szDst, const QRect & rcSrc )
{
	QImage imgALl;
	imgALl.load( strImg );
	if ( imgALl.isNull() )
		return imgALl;

	QImage img;
	if ( rcSrc.isNull() )
		img = imgALl;
	else
		img = imgALl.copy( rcSrc );	

	if ( img.isNull() )
		return img;

	// 不用缩放，直接返回
	if( ( img.width() == szDst.width() ) && 
		( img.height() == szDst.height() ) )
		return img;

	// 放缩图片，以固定大小显示
	QImage result = img.scaled( img.width(), img.height() ).scaled( szDst.width(), szDst.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	return result;
}

QImage RenderOperation::ScaledImage( QImage & imgSrc, const QSize & szDst, const QRect & rcSrc )
{
	if ( imgSrc.isNull() )
		return imgSrc;

	QImage img;
	if ( rcSrc.isNull() )
		img = imgSrc;
	else
		img = imgSrc.copy( rcSrc );	

	if ( img.isNull() )
		return img;

	// 不用缩放，直接返回
	if( ( img.width() == szDst.width() ) && 
		( img.height() == szDst.height() ) )
		return img;

	// 放缩图片，以固定大小显示
	QImage result = img.scaled( img.width(), img.height() ).scaled( szDst.width(), szDst.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	return result;
}




void RenderOperation::DrawText( QPainter& painter, QString &text, QRect& rc, QRect &rcBr, int align /*= Qt::AlignCenter*/, bool endellipsis/* = false */)
{
	if(endellipsis == true)
	{
		QFontMetrics metrics(painter.font());
		QString elidedText = metrics.elidedText(text, Qt::ElideRight, rc.width());
		painter.drawText(rc, align, elidedText, &rcBr);
	}
	else
	{
		painter.drawText(rc, align, text, &rcBr);
	}
}

void RenderOperation::DrawImageRound(QPainter& painter, QImage img, const QRect& rc)
{
	QSize size(40,40);

	QImage resultImage(size,QImage::Format_ARGB32_Premultiplied);
	QImage mask(":/Img/res/contact_icon_border_100x100.png");
	QPainter pant(&resultImage);
	pant.setCompositionMode(QPainter::CompositionMode_Source);
	pant.fillRect(resultImage.rect(), Qt::transparent);
	pant.setCompositionMode(QPainter::CompositionMode_SourceOver);
	pant.drawImage(0, 0, mask.scaled(size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
	pant.setCompositionMode(QPainter::CompositionMode_SourceOut);
	pant.drawImage(0, 0, img.scaled(size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
	pant.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	pant.end();

	painter.drawImage(rc.x(), rc.y(), resultImage);
}