@echo ========================================
@echo 开始编译NLEPreview...
@echo ========================================

@set VCENV="%VS120COMNTOOLS:"=%..\IDE\devenv.com"

set log="build_debug_log.txt"
if exist %log% (del %log%)
@%VCENV% ../NLEPreview.sln /rebuild "Debug|Win32" /Out %log%
@IF %ERRORLEVEL% NEQ 0 goto label_error

@goto end

:label_error

@echo 底层编译出错，请纠正后重新编译... 
@pause

:end

@echo 底层编译完毕