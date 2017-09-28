#pragma once
#include "inc_FSCommonlib/fscommonlib_global.h"
//recorder to editor通信协议
#define command_GotoEditor			"<cmd_gotoeditor>"
#define command_HideEditor			"<cmd_hideeditor>"
#define command_TranslateFiles		"<cmd_translatefiles>"
#define command_LoadProjectString	"<cmd_loadprojectstring>"
#define command_GetProjectString	"<command_getprojectstring>"
#define command_GetEditorThumbnail	"<command_geteditorthumbnail>"
#define command_TellContentChanged	"<command_tellcontentchanged>"
#define command_RecentOpens			"<command_recentopens>"
#define command_TellIsNewProject	"<command_tellisnewproject>"
#define command_TellProjectName		"<command_tellprojectname>"
#define command_TellSaveProjectFinish	"<command_tellsaveprojectfinish>"
#define command_TellRegisterResult	"<command_tellregisterresult>"
#define command_EditorMessagebox	"<command_editormessagebox>"



#define response2Editor_IsContentChanged	"<response2editor_iscontentchanged>"
#define response2Editor_IsNewProject		"<response2editor_isnewproject>"

//#define response2Editor_GetRecentOpens		"<response2editor_getrecentopens>"
//editor to recorder通信协议
#define request_GotoRecorder		"<request_gotorecorder>"
#define request_SetContentChanged	"<request_setcontentchanged>"
#define request_IsNewProject		"<request_isnewproject>"
//#define request_IsContentChanged	"<request_iscontentchanged>"
#define request_SaveProject			"<request_saveproject>"
#define request_SaveNewProject		"<request_savenewproject>"
#define request_Quit				"<request_quit>"
#define request_QuitDontSave				"<request_quitdontsave>"
#define request_ClearRecorder		"<request_clearrecorder>"
#define request_LoadProject			"<request_loadproject>"
#define request_LoadImportProject	"<request_loadimportproject>"
#define request_EditorShowed		"<request_editorshowed>"
//#define request_GetRecentOpens		"<request_getrecentopens>"
#define request_SaveThumbnailFile	"<request_savethumbnailfile>"
#define request_TellRegisterResult	"<request_tellregisterresult>"

#define response2Recorder_PushProjectString		"<response2recorder_pushprojectstring>"
#define response2Recorder_ThumbnailPath			"<response2recorder_thumbnailpath>"
#define response2Recorder_EditorMessagebox		"<response2recorder_EditorMessagebox>"

#include <QString>
QString FSCOMMONLIB_EXPORT GetCmdContent(const QString &msg, const QString &cmd);
QString FSCOMMONLIB_EXPORT GetCmdContentFromEnd(const QString &msg, const QString &cmd);



static const char *sFilmoraScreen = "Filmora Scrn";