/* ### WS@H Project:Multimedia_Platform ### */
#ifndef              AGILEERRORCODE_sjfoeiughnl4p8lser_H_
#define              AGILEERRORCODE_sjfoeiughnl4p8lser_H_


#define             ERR_AGILETRANS_UNKNOWN_ERROR                      0x80000010
#define             ERR_AGILETRANS_PARAMETERS_ERROR                   0x80000011
#define             ERR_AGILETRANS_NOT_IMPLEMNET                      0x80000012


#define              ERR_DEMUXER_REACH_END_OF_SOURCE                   0x80001000
#define              ERR_DEMUXER_REACH_EXCEPTION_OF_SOURCE             0x80001001
//#define              ERR_DEMUXER_COMPONENT_NOT_INITIALIAZE             0x80001002
//#define              ERR_DEMUXER_COMPONENT_IS_RUNNING                  0x80001003
#define              ERR_DEMUXER_COMPONENT_NOT_INITIALIZE              0x80001004
#define              ERR_DEMUXER_COMPONENT_TRIMSEGMENT_EXSIT           0x80001005
#define              ERR_DEMUXER_COMPONENT_TRIMSEGMENTS_NO_EXSIT       0x80001006
#define              ERR_DEMUXER_COMPONENT_DEMUXER_NOT_READY           0x80001007
#define              ERR_DEMUXER_START_DEMUX_THREAD_FAIL               0x80001008
#define              ERR_DEMUXER_PLUGIN_ONE_STREAM_FINISH              0x80001009
#define              ERR_DEMUXER_PLUGIN_ALL_STREAM_FINISH              0x8000100A
#define              ERR_DEMUXER_COMPONENT_NOT_YET_CONFIG              0x8000100B
#define              ERR_DEMUXER_COMPONENT_RETIRE_EMPTY_SAMPLE_FAIL    0x8000100C
#define              ERR_DEMUXER_COMPONENT_STREAM_PTS_OVER_HEAD        0x8000100D
#define              ERR_DEMUXER_COMPONENT_STREAM_REACH_TRIM_END       0x8000100E
#define              ERR_DEMUXER_COMPONENT_DONOT_SUPPORT_CHANGE_OUTPUT 0x8000100F





#define              ERR_DECMGR_COMPONENT_NOT_INITIALIZED              0x80002000
#define              ERR_DECMGR_COMPONENT_DEMUXER_RUNNING              0x80002001
#define              ERR_DECMGR_COMPONENT_NO_SOURCE                    0x80002002
#define              ERR_DECMGR_COMPONENT_SOURCE_ILLIGLE               0x80002003
#define              ERR_DECMGR_COMPONENT_SETTING_NOT_CONFIRM          0x80002004
#define              ERR_DECMGR_COMPONENT_NO_MORE_SOURCE               0x80002005
#define              ERR_DECMGR_COMPONENT_CODECS_NOT_READY             0x80002006
#define              ERR_DECMGR_COMPONENT_NO_MEDIAINFO                 0x80002007
#define              ERR_DECMGR_COMPONENT_NO_MEDIAMETADATAINFO         0x80002008


#define              ERR_ENCODER_COMPONENT_NOT_INITIALIZED             0x80003000
#define              ERR_ENCODER_COMPONENT_UNDER_RUNNING               0x80003001
#define              ERR_ENCODER_COMPONENT_CHANNEL_NOT_FOUND           0x80003002
#define              ERR_ENCODER_COMPONENT_NOT_RUNNING                 0x80003003


#define              ERR_MUXERMGR_COMPONENT_NOT_INITIALIZED            0x80004000
#define              ERR_MUXERMGR_COMPONENT_UNDER_RUNNING              0x80004001
#define              ERR_MUXERMGR_COMPONENT_NOT_RUNNING                0x80004002
#define              ERR_MUXERMGR_COMPONENT_FILE_EXCEPTION             0x80004003
#define              ERR_MUXERMGR_COMPONENT_FAIL_WRITE_SAMPLES         0x80004004
#define              ERR_MUXERMGR_COMPONENT_APPEARING_PADDING          0x80004005
#define              ERR_MUXERMGR_COMPONENT_WAITING_NEW_FILE           0x80004006
#define              ERR_MUXERMGR_COMPONENT_RETIRE_DATA_FAIL           0x80004007//can't not get Data Sample from Buffer.
#define              ERR_MUXERMGR_COMPONENT_PTS_OVER_HEAD              0x80004008
#define              ERR_MUXERMGR_COMPONENT_UNAVAILABLE_FRAME          0x80004009





enum          DEMUX_STATUS
{
	DEMUX_STOP,
	DEMUX_RUNNING,
	DEMUX_PAUSE
};
#endif