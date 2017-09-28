#include "stdafx.h"
#include "FSExportDataDef.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/PathOperation.h"
#include <QtXml/QDomDocument>
#include <QtConcurrent/QtConcurrent>
#include "FEncodeDataControl.h"

FEncodeDataControl::FEncodeDataControl(QObject *parent)
	: QObject(parent)
{
	QString strTmp[12] = { "WMV", "MP4", "AVI", "MOV", "F4V", "MKV", "TS", "3GP", "MPG2", "WEBM", "GIF","MP3" };
	for (int i = 0; i < 12;++i)
	{
		m_TypeArray[i] = strTmp[i];
	}
	ParseFormatInfo();
}

FEncodeDataControl::~FEncodeDataControl()
{

}

void FEncodeDataControl::ParseFormatInfo()
{
	QString formatFile = PathOperation::MakeWorkPath(FormatFileName);
	bool bExisted = FileOperation::IsFileExist(formatFile);
	Q_ASSERT(bExisted);

	QDomDocument doc;
	if (!OpenXmlFile(doc, formatFile))
	{
		return;
	}
	QDomElement rootElem = doc.documentElement();
	QDomElement formatinfoElem = rootElem.firstChildElement();
	if (formatinfoElem.isNull() || formatinfoElem.tagName() != "FormatInfo") return;
	while (!formatinfoElem.isNull())
	{
		MediaType curType;
		QString fourcc = formatinfoElem.attribute("FourCC");
		curType = getMediaType(fourcc);
		if (fourcc != "")
		{
			FormatInfo info;
			info.mFileFcc = fourcc;
			info.mFormat = formatinfoElem.attribute("Format");

			QDomElement tmpElem = formatinfoElem.firstChildElement("Name");
			if (!tmpElem.isNull())
			{
				info.mName = tmpElem.text();
			}

			tmpElem = tmpElem.nextSiblingElement("VideoEnc");
			if (!tmpElem.isNull())
			{
				info.mVideoEnc.mVideodefFcc = tmpElem.attribute("defVideoFourCC");
				info.mVideoEnc.mVideodefResolution = tmpElem.attribute("defResolution");

				QDomElement tmpElem2 = tmpElem.firstChildElement("EncParam");
				while (!tmpElem2.isNull())
				{
					if (tmpElem2.attribute("VideoFourCC") == info.mVideoEnc.mVideodefFcc)
					{
						QString resolution = tmpElem2.attribute("Resolution");
						QString defBitRate = tmpElem2.attribute("defVideoBitrate");
						QString defFrameRate = tmpElem2.attribute("defFrameRate");

						EncParam_Video para;
						para.mVideoFourcc = info.mVideoEnc.mVideodefFcc;
						para.mResolution = resolution;
						para.mDefVideoBitrate = defBitRate;
						para.mDefVideoFramerate = defFrameRate;

						QDomElement tmpElem3 = tmpElem2.firstChildElement("FrameRate");
						if (!tmpElem3.isNull())
						{
							auto ddd = tmpElem3.text();
							//QStringList list = ddd.split(';', QString::SkipEmptyParts);
							//tmpElem3.text().split(';', QString::SkipEmptyParts);
							para.mVideoFramerates = ddd.split(';', QString::SkipEmptyParts);
						}
						QDomElement tmpElem4 = tmpElem2.firstChildElement("VideoBitrate");
						if (!tmpElem4.isNull())
						{
							para.mVideoBitrates = tmpElem4.text().split(';', QString::SkipEmptyParts);
						}
						info.mVideoEnc.mEncParams.push_back(para);

					}
					tmpElem2 = tmpElem2.nextSiblingElement();
				}
			}

			tmpElem = tmpElem.nextSiblingElement("AudioEnc");
			if (!tmpElem.isNull())
			{
				info.mAudioEnc.mAudiodefFcc = tmpElem.attribute("defAudioFourCC");
				info.mAudioEnc.mAudiodefChannel = tmpElem.attribute("defChannel");

				QDomElement tmpElem2 = tmpElem.firstChildElement("EncParam");
				while (!tmpElem2.isNull())
				{
					if (tmpElem2.attribute("AudioFourCC") == info.mAudioEnc.mAudiodefFcc)
					{
						EncParam_Audio audioParam;
						audioParam.mAudioFourcc = info.mAudioEnc.mAudiodefFcc;
						audioParam.mChannel = tmpElem2.attribute("Channel");
						audioParam.mDefBitrate = tmpElem2.attribute("defAudioBitrate");
						audioParam.mDefSampleRate = tmpElem2.attribute("defSampleRate");

						QDomElement tmpElem3 = tmpElem2.firstChildElement("SampleRate");
						if (!tmpElem3.isNull())
						{
							QStringList list = tmpElem3.text().split(';', QString::SkipEmptyParts);
							audioParam.mAudioSamplerates = list;
						}
						QDomElement tmpElem4 = tmpElem2.firstChildElement("AudioBitrate");
						if (!tmpElem4.isNull())
						{
							audioParam.mAudioBitrates = tmpElem4.text().split(';', QString::SkipEmptyParts);
						}
						info.mAudioEnc.mEncParams.push_back(audioParam);
					}
					tmpElem2 = tmpElem2.nextSiblingElement();
				}
			}
			mInfoList.insert(curType, info);
		}
		formatinfoElem = formatinfoElem.nextSiblingElement();
	}
}

bool FEncodeDataControl::OpenXmlFile(QDomDocument &doc, QString strFile)
{
	QFile file(strFile);
	QString strError = "";
	int nRow = 0, nColumn = 0;
	if (!file.open(QIODevice::ReadOnly)) return false;
	if (!doc.setContent(&file, false, &strError, &nRow, &nColumn))
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}

const QMap<FEncodeDataControl::MediaType, FormatInfo>& FEncodeDataControl::getTotalFormat()
{
	return mInfoList;
}


QString FEncodeDataControl::getCurParameter(OptionCurrentText item)
{
	return "";
}

FEncodeDataControl::MediaType FEncodeDataControl::getMediaType(QString strType)
{
	MediaType type = Type_MPEG2;
	int iIndex = 0;
	for (int i = 0; i < 11;++i)
	{
		//比较前先清空空白字符
		QString strTypeTmp = strType;
		strTypeTmp = strTypeTmp.trimmed();
		QString strTmp = m_TypeArray[i];
		strTmp = strTmp.trimmed();
		if (!strTmp.compare(strTypeTmp, Qt::CaseInsensitive))
		{
			iIndex = i;
			m_TypeArray[i] = strType;
			break;
		}
	}
	type = (FEncodeDataControl::MediaType)iIndex;
	return type;
}

QStringList FEncodeDataControl::GetResolutionList(const QString  & fcc, FEncodeDataControl::MediaType index)
{
	QStringList list;
		QList <EncParam_Video> Paramlist;
		GetVideoEncParamListWithFcc(fcc, mInfoList[index], Paramlist);
		for (auto para : Paramlist)
		{
#ifndef _WIN64
			Resolution res(para.mResolution);
			if (res.Area() > 1920 * 1440) continue;
#endif
			list.push_back(para.mResolution);
		}
	return list;
}

QStringList FEncodeDataControl::GetVidioFramerateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &resolution)
{
		QList <EncParam_Video> Paramlist;
		GetVideoEncParamListWithFcc(Fcc, mInfoList[index], Paramlist);
		for (auto para : Paramlist)
		{
			if (resolution == para.mResolution)
			{
				return para.mVideoFramerates;
			}
		}
	return QStringList();
}

QStringList FEncodeDataControl::GetVidioBitrateList(MediaType index, const QString &Fcc, const QString &resolution)
{
	EncParam_Video  info =  GetVideoEncParam(index, Fcc, resolution);
	return info.mVideoBitrates;
}

QStringList FEncodeDataControl::GetVideoEncoderList(FEncodeDataControl::MediaType index)
{
	QStringList list;
		for (auto Param : mInfoList[index].mVideoEnc.mEncParams)
		{
			if (!list.contains(Param.mVideoFourcc))
			{
				list.push_back(Param.mVideoFourcc);
			}
		}
	return list;
}


EncParam_Video FEncodeDataControl::GetVideoEncParam(FEncodeDataControl::MediaType index,QString strCurFcc, QString resolustion)
{
		QList <EncParam_Video> List;
		GetVideoEncParamListWithFcc(strCurFcc, mInfoList[index], List);
		return GetVideoEncParamWithResolution(List, resolustion);
}

EncParam_Audio FEncodeDataControl::GetAudioEncParam(FEncodeDataControl::MediaType index, QString strCurFcc, QString channel)
{
		QList <EncParam_Audio> List;
		GetAudioEncParamListWithFcc(strCurFcc, mInfoList[index], List);
		return GetAudioEncParamWithChannel(List, channel);
}

QStringList FEncodeDataControl::GetAudioChannelList(const QString &fcc, FEncodeDataControl::MediaType index)
{
	QStringList list;
		QList <EncParam_Audio> Paramlist;
		GetAudioEncParamListWithFcc(fcc, mInfoList[index], Paramlist);
		for (auto para : Paramlist)
		{
			list.push_back(para.mChannel);
		}
	return list;
}

QStringList FEncodeDataControl::GetAudioEncoderList(FEncodeDataControl::MediaType index)
{
	QStringList list;
		for (auto Param : mInfoList[index].mAudioEnc.mEncParams)
		{
			if (!list.contains(Param.mAudioFourcc))
			{
				list.push_back(Param.mAudioFourcc);
			}
		}
	return list;
}

QStringList FEncodeDataControl::GetAudioBitrateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &Channel)
{
		QList <EncParam_Audio> Paramlist;
		GetAudioEncParamListWithFcc(Fcc, mInfoList[index], Paramlist);
		for (auto para : Paramlist)
		{
			if (Channel == para.mChannel)
			{
				return para.mAudioBitrates;
			}
		}
	return QStringList();
}

QStringList FEncodeDataControl::GetAudioSampleRateList(MediaType index, const QString &Fcc, const QString &Channel)
{
	EncParam_Audio audioInfo = GetAudioEncParam(index, Fcc, Channel);
	return audioInfo.mAudioSamplerates;
}

QString FEncodeDataControl::GetDefaultVideoFourcc(MediaType index)
{
	return GetDefaultParameter(index)[OptionCurrentText::Video_Encoder];
}

QString FEncodeDataControl::GetDefaultVideoResolution(MediaType index)
{
	return GetDefaultParameter(index)[OptionCurrentText::Video_Resolution];
}

QString FEncodeDataControl::GetDefaultVideoBitrate(MediaType index, QString strEncoder, QString strResolution)
{
	EncParam_Video  info = GetVideoEncParam(index, strEncoder, strResolution);
	return info.mDefVideoBitrate;
}

QString FEncodeDataControl::GetDefaultVideoFramerate(MediaType index, QString strEncoder, QString strResolution)
{
	EncParam_Video  info = GetVideoEncParam(index, strEncoder, strResolution);
	return info.mDefVideoFramerate;
}

QString FEncodeDataControl::GetDefaultAudioFourcc(MediaType index)
{
	return GetDefaultParameter(index)[OptionCurrentText::Audio_Encoder];
}

QString FEncodeDataControl::GetDefaultAudioChannel(MediaType index)
{
	return GetDefaultParameter(index)[OptionCurrentText::Audio_Channel];
}

QString FEncodeDataControl::GetDefaultAudioBitrate(MediaType index, QString strEncoder, QString strChannel)
{
	EncParam_Audio info = GetAudioEncParam(index, strEncoder, strChannel);
	return info.mDefBitrate;
}

QString FEncodeDataControl::GetDefaultAudioSampleRate(MediaType index, QString strEncoder, QString strChannel)
{
	EncParam_Audio info = GetAudioEncParam(index, strEncoder,strChannel);
	return info.mDefSampleRate;
}

void FEncodeDataControl::GetAudioEncParamListWithFcc(const QString & fcc, const FormatInfo &info, QList <EncParam_Audio> &list)
{
	for each (auto var in info.mAudioEnc.mEncParams)
	{
		if (var.mAudioFourcc == fcc)
		{
			list.push_back(var);
		}
	}
}

EncParam_Audio FEncodeDataControl::GetAudioEncParamWithChannel(const QList <EncParam_Audio> &list, const QString &channel)
{
	EncParam_Audio param;
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i).mChannel == channel)
			return list.at(i);
	}
	if (!list.isEmpty())
		param = list.at(0);
	return param;
}

void FEncodeDataControl::GetVideoEncParamListWithFcc(const QString  & fcc, const FormatInfo &info, QList <EncParam_Video> &list)
{
	for each (auto var in info.mVideoEnc.mEncParams)
	{
		if (var.mVideoFourcc == fcc)
		{
			list.push_back(var);
		}
	}
}

EncParam_Video FEncodeDataControl::GetVideoEncParamWithResolution(const QList <EncParam_Video> &list, const QString &resolution)
{
	QPair<int, int> pair = { 0, 0 };
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i).mResolution == resolution)
			return list.at(i);
		if (i == 0)
		{
			pair.second = qAbs(Resolution(resolution).Area() - Resolution(list.at(i).mResolution).Area());
		}
		else
		{
			int sub = qAbs(Resolution(resolution).Area() - Resolution(list.at(i).mResolution).Area());
			if (pair.second > sub)
			{
				pair.first = i;
				pair.second = sub;
			}
		}
	}
	if (list.isEmpty())
		return EncParam_Video();
	return list.at(pair.first);
}

const QMap<FEncodeDataControl::OptionCurrentText, QString>& FEncodeDataControl::GetDefaultParameter(MediaType type)
{
	FormatInfo info = mInfoList[type];
	QList <EncParam_Video> Paramlist;
	GetVideoEncParamListWithFcc(info.mVideoEnc.mVideodefFcc, info, Paramlist);
	m_DefParameter.insert(OptionCurrentText::Video_Encoder, info.mVideoEnc.mVideodefFcc);
	m_DefParameter.insert(OptionCurrentText::Video_Resolution, info.mVideoEnc.mVideodefResolution);
	EncParam_Video  param = GetVideoEncParamWithResolution(Paramlist, info.mVideoEnc.mVideodefResolution);
	m_DefParameter.insert(OptionCurrentText::Video_FrameRate,param.mDefVideoFramerate);
	m_DefParameter.insert(OptionCurrentText::Video_BitRate, param.mDefVideoFramerate);

	QList <EncParam_Audio> AudioParamlist;
	GetAudioEncParamListWithFcc(info.mAudioEnc.mAudiodefFcc, info, AudioParamlist);
	m_DefParameter.insert(OptionCurrentText::Audio_Encoder, info.mAudioEnc.mAudiodefFcc);
	m_DefParameter.insert(OptionCurrentText::Audio_Channel, info.mAudioEnc.mAudiodefChannel);
	EncParam_Audio  Audioparam = GetAudioEncParamWithChannel(AudioParamlist, info.mAudioEnc.mAudiodefChannel);
	m_DefParameter.insert(OptionCurrentText::Audio_BitRate, Audioparam.mDefBitrate);
	m_DefParameter.insert(OptionCurrentText::Audio_SampleRate, Audioparam.mDefSampleRate);
	return m_DefParameter;
}

QString FEncodeDataControl::GetCurFourCC(int iIndex)
{
	return m_TypeArray[iIndex];
}

QString FEncodeDataControl::GetCurFileFormat(MediaType type)
{
	return mInfoList[type].mFormat;
}

