/* ### WS@H Project:NLE ### */
#ifndef UNIVERSAL_ANALYTICS_H_
#define UNIVERSAL_ANALYTICS_H_

typedef void* ua_tracker;
typedef void* ua_hit;

/*************************************************
//  Method:			ua_create_tracker
//  Description:	创建一个记录者
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	ua_tracker 记录者对象
//  Parameter:   	tracking_id - The tracking ID / web property ID. The format is UA-XXXX-Y.
//  Parameter:   	client_id - This anonymously identifies a particular user, device, or browser instance. this is randomly generated for each particular instance of an application install.
//  Parameter:   	app_name - Specifies the application name.
*************************************************/
ua_tracker __stdcall ua_create_tracker(const char* tracking_id, const char* client_id, const char* app_name);

/*************************************************
//  Method:			ua_create_event_hit
//  Description:	创建一个动作事件
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	ua_hit 事件对象
//  Parameter:   	category - 事件类别
//  Parameter:   	action - 事件动作
//  Parameter:   	label - 事件标签（NULL代表没有）
//  Parameter:   	value - 事件价值（<0代表没有）
*************************************************/
ua_hit __stdcall ua_create_event_hit(const char* category, const char* action, const char* label, int value);

/*************************************************
//  Method:			ua_create_screenview_hit
//  Description:	创建一个跳转页面事件
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	ua_hit 事件对象
//  Parameter:   	screen_name - 跳转后页面名称
*************************************************/
ua_hit __stdcall ua_create_screenview_hit(const char* screen_name);

/*************************************************
//  Method:			ua_create_timing_hit
//  Description:	创建一个计时器事件
//  Author:			chenming
//  Date:           2015/07/22
//  Returns:       	ua_hit 事件对象
//  Parameter:   	category - 计时器类别
//  Parameter:   	variable - 计时器名称
//  Parameter:   	value - 时间（毫秒）
//  Parameter:   	label - 标签（可选，NULL代表没有）
*************************************************/
ua_hit __stdcall ua_create_timing_hit(const char* category, const char* variable, int value, const char* label);

/*************************************************
//  Method:			ua_create_socialnetwork_hit
//  Description:	创建一个社交事件
//  Author:			chenming
//  Date:           2015/07/27
//  Returns:       	ua_hit 事件对象
//  Parameter:   	network - 社交网络名称
//  Parameter:   	action - 社交动作
//  Parameter:   	target - 社交目标
*************************************************/
ua_hit __stdcall ua_create_socialnetwork_hit(const char* network, const char* action, const char* target);

/*************************************************
//  Method:			ua_create_exception_hit
//  Description:	创建一个异常事件
//  Author:			chenming
//  Date:           2015/07/28
//  Returns:       	ua_hit 事件对象
//  Parameter:   	description - 异常描述（最大150字符）
//  Parameter:   	fatal - 是否重大事故（1 or 0)
*************************************************/
ua_hit __stdcall ua_create_exception_hit(const char* description, const int fatal);

/*************************************************
//  Method:			ua_tracker_set_app_ver
//  Description:	设置应用程序版本号
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	ver_name - 版本号字符串
*************************************************/
void __stdcall ua_tracker_set_app_ver(ua_tracker tracker, const char* ver_name);

/*************************************************
//  Method:			ua_tracker_set_language
//  Description:	设置应用程序语言
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	lang_name - 语言字符串
*************************************************/
void __stdcall ua_tracker_set_language(ua_tracker tracker, const char* lang_name);

/*************************************************
//  Method:			ua_tracker_set_viewport_size
//  Description:	设置应用程序界面尺寸
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	width - 长
//  Parameter:   	height - 宽
*************************************************/
void __stdcall ua_tracker_set_viewport_size(ua_tracker tracker, const int width, const int height);

/*************************************************
//  Method:			ua_tracker_set_custom_dimension
//  Description:	设置自定义维度
//  Author:			chenming
//  Date:           2015/07/23
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	index - 自定义维度索引(1-20)
//  Parameter:   	value - 维度数据
*************************************************/
void __stdcall ua_tracker_set_custom_dimension(ua_tracker tracker, const int index, const char* value);

/*************************************************
//  Method:			ua_tracker_set_custom_metric
//  Description:	设置自定义指标
//  Author:			chenming
//  Date:           2015/07/23
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	index - 自定义指标索引(1-20)
//  Parameter:   	value - 指标数值
*************************************************/
void __stdcall ua_tracker_set_custom_metric(ua_tracker tracker, const int index, const int value);

/*************************************************
//  Method:			ua_send_hit
//  Description:	发送一次事件
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
//  Parameter:   	hit - 时间对象
*************************************************/
void __stdcall ua_send_hit(ua_tracker tracker, ua_hit hit);

/*************************************************
//  Method:			ua_destroy_tracker
//  Description:	销毁记录对象
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	tracker - 记录对象
*************************************************/
void __stdcall ua_destroy_tracker(ua_tracker tracker);

/*************************************************
//  Method:			ua_destroy_hit
//  Description:	销毁事件
//  Author:			chenming
//  Date:           2015/07/17
//  Returns:       	void __stdcall
//  Parameter:   	hit - 事件对象
*************************************************/
void __stdcall ua_destroy_hit(ua_hit hit);


#endif //UNIVERSAL_ANALYTICS_H_