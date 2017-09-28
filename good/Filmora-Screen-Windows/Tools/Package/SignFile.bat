@echo off

set SignPath=..\..\..\..\Common\Tools\Signer

if "%1" == "_VE" (
  %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll ..\..\FrontUI\Bin\VideoEditor.exe
  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 ..\..\FrontUI\Bin\VideoEditor.exe
  
  
  %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll ..\..\..\..\Common\Tools\EffectStore.exe
  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 ..\..\..\..\Common\Tools\EffectStore.exe
  
) else (
  %SignPath%\signtool sign /i "VeriSign Class 3 Code Signing 2010 CA" /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /t http://timestamp.verisign.com/scripts/timestamp.dll .\Output\Filmora^(x86^).exe
  %SignPath%\signtool sign /i "Symantec Class 3 SHA256 Code Signing CA" /fd sha256 /as /v /ac "%SignPath%\mscv-vsclass3.cer" /s my /a /n "Wondershare software CO., LIMITED" /tr http://timestamp.globalsign.com/?signature=sha2 /td sha256 .\Output\Filmora^(x86^).exe
)