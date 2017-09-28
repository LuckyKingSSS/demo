#ifndef _H_NLEEVENTDEFINE_H_
#define _H_NLEEVENTDEFINE_H_



#include "NLEType.h"

typedef enum enumNLEEventID
{
	NLEEVENT_BASE = 0x1000,
	///更新属性消息
	NLEEVENT_UPDATE_PROPERTIES = NLEEVENT_BASE + 1,

	NLEEVENT_CLIP_BASE = NLEEVENT_BASE + 1000,

	///属性变化
	///WPARAM 
	NLEEVENT_CLIP_PROPERTIES_CHANGE = NLEEVENT_CLIP_BASE + 1,

	///Markdown范围变化
	///WPARAM 发生变化前的Markdown
	///LPARAM 变化前后，结构体指针(MarkRangeChange*),see in MarkdownCommon.h
	NLEEVENT_MARKDOWN_RANGE_CHANGE,

	///任务完成通知
	///WPARAM 保留
	///LPARAM 保留
	NLEEVENT_TASK_FINISHED,

	///后台任务是否接受
	///WPARAM TRUE or FALSE
	///LPARAM 保留
	NLEEVENT_BACKBENCH_ACCEPT,

	///前一个render属性有变化，其输出的frame会变化
	///WPARAM 保留
	///LPARAM 保留
	NLEEVENT_SOURCE_RENDER_UPDATED,

}NLEEventID;


#endif //_H_NLEEVENTDEFINE_H_
