/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  AudioPallarView.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    10/18/2016 14:42
 */
#ifndef _H_AUDIOPALLARVIEW_H_
#define _H_AUDIOPALLARVIEW_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

class CAudioPillarView : public QWidget
{
	Q_OBJECT

public:
	CAudioPillarView(QWidget *parent = NULL);
	~CAudioPillarView();

	void SetPallarRange(int nMinValue, int nMaxValue);	
	void SetPillarValue(int nLeftVolume, int nRightVolume);
protected:
	void showEvent(QShowEvent *event);

private:
	QLabel *m_pLeftVolumeView;
	QLabel *m_pRightVolumeView;

	int m_rangeCount;

};

#endif //_H_AUDIOPALLARVIEW_H_
