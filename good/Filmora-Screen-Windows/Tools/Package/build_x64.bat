@echo off

if exist ".\Output" (rmdir /s /q .\Output)

echo ========================================
echo 开始编译FilmoraScreen...
echo ========================================
set VCENV="%VS120COMNTOOLS:"=%..\IDE\devenv.com"
set log="build_x64.txt"
if exist %log% (del %log%)
%VCENV% "..\..\FilmoraScreen.sln" /rebuild "Release|x64" /Out %log%
if %errorlevel% NEQ 0 (
echo 编译FilmoraScreen出错，请纠正后重新编译... 
exit
)

echo FilmoraScreen编译完毕

echo ========FSRecorder.exe签名 ......%time%

call ".\SignFile_x64.bat" _Recorder

rem 打包
set COMPIL32="%systemdrive%\Program Files\Inno Setup 5(u)\ISCC.exe"
if not exist %COMPIL32% set COMPIL32="%systemdrive%\Program Files (x86)\Inno Setup 5(u)\ISCC.exe"

echo ========打包FilmoraScreen(x64)安装程序 ......%time%
%COMPIL32% .\filmorascreen.iss /dCPUX64
if not %errorlevel%==0 (
echo 错误:打包完整版本出错
exit
) 
echo ^>^>^>^>^>^>^>^>FilmoraScreen(x64)安装程序打包完成

echo 写入代理商xml文件 ...
..\WSOverlay.exe /P "Output\Filmora Scrn(x64).exe" "..\Customization.xml"

echo ========安装程序签名 ......%time%
call ".\SignFile_x64.bat" _Ins
if not %errorlevel%==0 (
echo 安装包签名出错
exit
)


echo ========================================
echo 完成
echo ========================================


