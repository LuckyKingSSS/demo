@echo off

if exist ".\Output" (rmdir /s /q .\Output)

echo ========================================
echo 开始编译FilmoraScreen...
echo ========================================
set VCENV="%VS120COMNTOOLS:"=%..\IDE\devenv.com"
set log="build_x64.txt"
if exist %log% (del %log%)
rem %VCENV% ../../FilmoraScreen.sln /rebuild "Release|x64" /Out %log%
rem if %errorlevel% NEQ 0 goto label_error

goto next

:label_error
echo 编译FilmoraScreen出错，请纠正后重新编译... 
pause

:next
echo FilmoraScreen编译完毕

rem 打包
set COMPIL32="%systemdrive%\Program Files\Inno Setup 5\ISCC.exe"
if not exist %COMPIL32% set COMPIL32="%systemdrive%\Program Files (x86)\Inno Setup 5\ISCC.exe"
set errorlevel=0
echo ========打包FilmoraScreen(x64)安装程序 ......%time%
%COMPIL32% .\filmorascreen.iss /dCPUX64
if not %errorlevel%==0 echo 错误:打包完整版本出错 &pause

echo 写入代理商xml文件 ...
..\..\Tools\WSOverlay.exe /P "Output\Filmora Scrn(x64).exe" "..\..\Tools\Customization.xml"

echo ========================================
echo 完成
echo ========================================

pause


