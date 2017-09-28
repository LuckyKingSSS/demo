#ifndef FENCODEDATACONTROL_H
#define FENCODEDATACONTROL_H

#include <QObject>
#include <QtXml/QDomDocument>
#include "FSExportDataDef.h"



#define FormatFileName "Format.dat"
class FEncodeDataControl : public QObject
{
	Q_OBJECT
public:
	enum OptionCurrentText{
		Video_Encoder = 0,
		Video_Resolution,
		Video_FrameRate,
		Video_BitRate,
		Audio_Encoder,
		Audio_Channel,
		Audio_BitRate,
		Audio_SampleRate
	};
	enum MediaType{
		Type_WMV,
		Type_Mp4,
		Type_AVI,
		Type_MOV,
		Type_F4V,
		Type_MKV,
		Type_TS,
		Type_3GP,
		Type_MPEG2,
		Type_WEBM,
		Type_GIF,
		Type_MP3
	};
public:
	FEncodeDataControl(QObject *parent);
	~FEncodeDataControl();
	QStringList GetVideoEncoderList(FEncodeDataControl::MediaType index);
	QStringList GetResolutionList(const QString  & fcc, FEncodeDataControl::MediaType index);
	QStringList GetVidioFramerateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &resolution);
	QStringList GetVidioBitrateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &resolution);
	QStringList GetAudioChannelList(const QString &fcc, FEncodeDataControl::MediaType index);
	QStringList GetAudioEncoderList(FEncodeDataControl::MediaType index);
	QStringList GetAudioBitrateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &Channel);
	QStringList GetAudioSampleRateList(FEncodeDataControl::MediaType index, const QString &Fcc, const QString &Channel);

	QString GetDefaultVideoFourcc(FEncodeDataControl::MediaType index);
	QString GetDefaultVideoResolution(FEncodeDataControl::MediaType index);
	QString GetDefaultVideoBitrate(FEncodeDataControl::MediaType index,QString strEncoder,QString strResolution);
	QString GetDefaultVideoFramerate(FEncodeDataControl::MediaType index, QString strEncoder, QString strResolution);
	QString GetDefaultAudioFourcc(FEncodeDataControl::MediaType index);
	QString GetDefaultAudioChannel(FEncodeDataControl::MediaType index);
	QString GetDefaultAudioBitrate(FEncodeDataControl::MediaType index, QString strEncoder, QString strChannel);
	QString GetDefaultAudioSampleRate(FEncodeDataControl::MediaType index, QString strEncoder, QString strChannel);
	const QMap<OptionCurrentText, QString>& GetDefaultParameter(MediaType type);
	QString GetCurFourCC(int iIndex);
	QString GetCurFileFormat(MediaType type);
private:
	EncParam_Video GetVideoEncParam(FEncodeDataControl::MediaType index, QString strCurFcc, QString resolustion);
	EncParam_Audio GetAudioEncParam(FEncodeDataControl::MediaType index, QString strCurFcc, QString channel);
	void GetAudioEncParamListWithFcc(const QString & fcc, const FormatInfo &info, QList <EncParam_Audio> &list);
	EncParam_Audio GetAudioEncParamWithChannel(const QList <EncParam_Audio> &list, const QString &channel);
	void GetVideoEncParamListWithFcc(const QString  & fcc, const FormatInfo &info, QList <EncParam_Video> &list);
	EncParam_Video GetVideoEncParamWithResolution(const QList <EncParam_Video> &list, const QString &resolution);
	

private:
	void FEncodeDataControl::ParseFormatInfo();
	bool OpenXmlFile(QDomDocument &doc, QString strFile);
	const QMap<MediaType, FormatInfo>& getTotalFormat();
	QString getCurParameter(OptionCurrentText item);
	MediaType getMediaType(QString strType);
private:
	QMap<MediaType,FormatInfo> mInfoList;
	QMap<OptionCurrentText, QString> m_DefParameter; //ƒ¨»œ≈‰÷√
	QString m_TypeArray[12];
};

#endif // FENCODEDATACONTROL_H
