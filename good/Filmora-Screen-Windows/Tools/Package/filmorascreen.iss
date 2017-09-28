#pragma option -v+
#pragma verboselevel 9

#define RecorderExeName "FSRecorder.exe"

#ifdef CPUX64
  #define x64Folder " (x64)"
  #define x64SubFolder "\x64"
  #define Win64Folder "\..\Win64"
  #define x6432Node "\Wow6432Node"
#else
  #define x64Folder
  #define x64SubFolder
  #define Win64Folder
  #define x6432Node
#endif

#include <.\FSHeader.iss>

#define BinPath "..\..\bin\x64\Release"

[Setup]


AppName={cm:AppName}

AppVerName={cm:AppName}(Build {code:GetAppShortVersion|{#AppVersion}})
AppMutex={#AppMutex}
AppPublisher={code:GetAppPublisher|Wondershare Software}
AppPublisherURL={code:GetAgentValue|HomePage}
AppSupportURL={code:GetAgentValue|SupportPage}
AppUpdatesURL={#UpdatePage}

DefaultDirName={pf}\{cm:AppPath}
DefaultGroupName={cm:AppPath}

Compression=lzma2
SolidCompression=yes
OutputDir=.\Output
SetupIconFile=inst.ico
WizardImageFile=WizModernImage.bmp
VersionInfoVersion={#AppVersion}
#ifdef CPUX64
OutputBaseFilename={#OutputName}(x64)
#else
OutputBaseFilename={#OutputName}(x86)
#endif
LicenseFile=license_en.txt
WizardSmallImageFile=WizModernSmallImage.bmp
;控制面板显示图标
UninstallDisplayIcon={uninstallexe}
UsePreviousLanguage=no
ShowLanguageDialog=yes
LanguageDetectionMethod=locale
ShowUndisplayableLanguages=yes
ChangesAssociations=yes
;简化安装步骤
DisableWelcomePage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DisableFinishedPage=yes
VersionInfoDescription={#AppName} Setup
VersionInfoProductName={#AppName}
VersionInfoCopyright=Copyright (C) 2017 Wondershare. All rights reserved.



#ifdef CPUX64
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#endif

[LangOptions]
DialogFontName=Tahoma
DialogFontSize=8
;ESM.LanguageName=Espa<00F1>ol (Latinoamérica)
PTG.LanguageName=Portugu<00EA>s
;ZHH.LanguageName=繁體中文 (香港特別行政區)
;CHT.LanguageName=繁體中文 (台湾)

[Languages]
Name: ENG; MessagesFile: InnoSetup\English.isl; LicenseFile: license_en.txt
Name: JPN; MessagesFile: InnoSetup\Japanese.isl; LicenseFile: license_jp.txt
Name: DEU; MessagesFile: InnoSetup\German.isl; LicenseFile: license_de.txt
Name: ESP; MessagesFile: InnoSetup\Spanish.isl; LicenseFile: license_es.txt
;Name: ESM; MessagesFile: InnoSetup\Spanish.isl; LicenseFile: license_es.txt
Name: PTG; MessagesFile: InnoSetup\Portuguese.isl; LicenseFile: license_pt.txt
Name: FRA; MessagesFile: InnoSetup\French.isl; LicenseFile: license_fr.txt
Name: ITA; MessagesFile: InnoSetup\Italian.isl; LicenseFile: license_it.txt
;Name: ZHH; MessagesFile: InnoSetup\ChineseTrad.isl; LicenseFile: license_hk.txt
;Name: CHT; MessagesFile: InnoSetup\ChineseTrad.isl; LicenseFile: license_hk.txt
;Name: NLD; MessagesFile: InnoSetup\Dutch.isl; LicenseFile: license_du.txt
;Name: RUS; MessagesFile: InnoSetup\Russian.isl; LicenseFile: license_ru.txt
;Name: ARA; MessagesFile: InnoSetup\Arabic.isl;

[CustomMessages]
ENG.ProgramOrderOnTheWeb=Order %1 On the Web
ENG.ProgramHelp=How to Use %1
ENG.ProgramOnTheWeb=%1 On the Web
ENG.AppName={#AppName}
ENG.AppPath={#AppPath}
ENG.UninstMsg1=The Setup has detected that another version is already on your computer.
ENG.UninstMsg2=To install current version, it has to be removed first.   
ENG.AcceptLicense=I have read and &accept the agreement
ENG.Buy=&Buy Now
ENG.Launch=Launch
ENG.TakePlan=Join the User Experience Improvement Program.
ENG.UninstMsg3=Congratulations, you have installed Wondershare Filmora Scrn successfully.
ENG.UninstMsg4=You are using a trial version, a watermark will be displayed on the output video.
ENG.UninstMsg5=The benefits of a registered user include:
ENG.UninstMsg6=1. No watermark on the output video.
ENG.UninstMsg7=2. Free technical support.
ENG.UninstMsg8=3. Free upgrades.   
ENG.UninstMsg9=Do you want to remove all the effect resource files？
ENG.PlatformError=Sorry, this program can not be installed on Win32.

JPN.ProgramOrderOnTheWeb=購入
JPN.ProgramHelp=オンラインヘルプ
JPN.ProgramOnTheWeb=製品ページ
JPN.AppName={#AppName}
;Wondershare Filmora 動画編集
JPN.AppPath={#AppPath}     
JPN.UninstMsg1=お客様のパソコンに別のバージョンがインストールされています。
JPN.UninstMsg2=最新版をインストールするには、旧バージョンをアンインストールしてください。  
JPN.AcceptLicense=契約書は、確認とおり同意します
JPN.Buy=今すぐご購入
JPN.Launch=スタート
JPN.TakePlan=ユーザー体験を向上するプログラムに加入します。
JPN.UninstMsg3=Wondershare Filmoraが正常にインストールされました。
JPN.UninstMsg4=試用版を利用する場合、変換後の出力ビデオに透かしが表示されましたが、
JPN.UninstMsg5=ご購入後、製品登録することで、下記のメリットがあります：
JPN.UninstMsg6=1. 出力ビデオに透かしの表示が無くなります。
JPN.UninstMsg7=2. 無償でテクニカルサポートが受けられます。
JPN.UninstMsg8=3. 無償で最新版へアップグレート可能です。 
JPN.UninstMsg9=エフェクトのリソースファイルを全部削除しますか？
JPN.PlatformError=Sorry, this program can not be installed on Win32.

DEU.ProgramOrderOnTheWeb=Online bestellen
DEU.ProgramHelp=Handbuch
DEU.ProgramOnTheWeb=Produktseite
DEU.AppName={#AppName}
DEU.AppPath={#AppPath}
DEU.UninstMsg1=Es ist bereits eine andere Version der Software auf Ihrem Rechner installier.
DEU.UninstMsg2=Um die aktuelle Version zu installieren, muss diese zun?chst entfernt werden.   
DEU.AcceptLicense=Ich habe die Vereinbarung gelesen und stimme ihr zu
DEU.Buy=Jetzt kaufen
DEU.Launch=Starten
DEU.TakePlan=Treten Sie dem Programm zur Verbesserung der Benutzerfreundlichkeit bei.
DEU.UninstMsg3=Herzlichen Glückwunsch! Sie haben Wondershare Filmora erfolgreich installiert.
DEU.UninstMsg4=Sie benutzen eine Test-Version, daher wird im Ausgabe-Video ein Wasserzeichen angezeigt.
DEU.UninstMsg5=Zu den Vorteilen für angemeldete Benutzer z?hlen:
DEU.UninstMsg6=1. Kein Wasserzeichen im Ausgabe-Video.
DEU.UninstMsg7=2. Gratis technischer Support.
DEU.UninstMsg8=3. Gratis Upgrades. 
DEU.UninstMsg9=Wollen Sie alle Effekt-Ressource-Dateien zu entfernen?	
DEU.PlatformError=Sorry, this program can not be installed on Win32.

ESP.ProgramOrderOnTheWeb=Comprar
ESP.ProgramHelp=Ayuda en línea
ESP.ProgramOnTheWeb=Página principal
ESP.AppName={#AppName}
ESP.AppPath={#AppPath}
ESP.UninstMsg1=Se ha detectado otra versión instalada en su PC.
ESP.UninstMsg2=Debe desinstalar la versión anterior para poder continuar con la instalación. 
ESP.AcceptLicense=He leído y acepto el contrato
ESP.Buy=Comprar ahora
ESP.Launch=Iniciar
ESP.TakePlan=Unirse al programa de mejora de la experiencia del usuario.
ESP.UninstMsg3=Enhorabuena. Ha instalado Wondershare Filmora correctamente.
ESP.UninstMsg4=Está utilizando una versión de prueba, por lo que se mostrará una marca de agua en el vídeo de salida.
ESP.UninstMsg5=A continuación se indican las ventajas de ser un usuario registrado:
ESP.UninstMsg6=1. No hay marca de agua en el vídeo de salida.
ESP.UninstMsg7=2. Soporte técnico gratuito.
ESP.UninstMsg8=3. Actualizaciones gratuitas.  
ESP.UninstMsg9=Quieres eliminar todos los archivos de recursos de efecto?	
ESP.PlatformError=Sorry, this program can not be installed on Win32.

;ESM.ProgramOrderOnTheWeb=Comprar
;ESM.ProgramHelp=Ayuda en línea
;ESM.ProgramOnTheWeb=Página principal
;ESM.AppName={#AppName}
;ESM.AppPath={#AppPath}
;ESM.UninstMsg1=Se ha detectado otra versión instalada en su PC.
;ESM.UninstMsg2=Debe desinstalar la versión anterior para poder continuar con la instalación.  
;ESM.AcceptLicense=He leído y acepto el contrato
;ESM.Buy=Comprar ahora
;ESM.Launch=Iniciar
;ESM.TakePlan=Unirse al programa de mejora de la experiencia del usuario.
;ESM.UninstMsg3=Enhorabuena. Ha instalado Wondershare Filmora correctamente.
;ESM.UninstMsg4=Está utilizando una versión de prueba, por lo que se mostrará una marca de agua en el vídeo de salida.
;ESM.UninstMsg5=A continuación se indican las ventajas de ser un usuario registrado:
;ESM.UninstMsg6=1. No hay marca de agua en el vídeo de salida.
;ESM.UninstMsg7=2. Soporte técnico gratuito.
;ESM.UninstMsg8=3. Actualizaciones gratuitas. 
;ESM.UninstMsg9=Quieres eliminar todos los archivos de recursos de efecto?	

PTG.ProgramOrderOnTheWeb=Comprar
PTG.ProgramHelp=Ajuda online
PTG.ProgramOnTheWeb=Página do produto
PTG.AppName={#AppName}
PTG.AppPath={#AppPath}
PTG.UninstMsg1=O processo de configura??o detectou uma vers?o já existente no seu computador.
PTG.UninstMsg2=Para instalar a vers?o actual deverá primeiro remover a vers?o anterior.       
PTG.AcceptLicense=Li e aceito o acordo
PTG.Buy=Comprar agora
PTG.Launch=Executar
PTG.TakePlan=Participar no Programa de melhoria da experiência do utilizador.
PTG.UninstMsg3=Parabéns, instalou com êxito o Wondershare Filmora.
PTG.UninstMsg4=Está a utilizar uma vers?o de teste, será exibida uma marca de água no vídeo de saída.
PTG.UninstMsg5=As vantagens de um utilizador registado incluem:
PTG.UninstMsg6=1. Sem marca de água no vídeo de saída.
PTG.UninstMsg7=2. Suporte técnico grátis.
PTG.UninstMsg8=3. Actualiza??es grátis.
PTG.UninstMsg9=Você quer remover todos os arquivos de recursos de efeito?
PTG.PlatformError=Sorry, this program can not be installed on Win32.

FRA.ProgramOrderOnTheWeb=Acheter
FRA.ProgramHelp=Aide en ligne
FRA.ProgramOnTheWeb=Page produit
FRA.AppName={#AppName}
FRA.AppPath={#AppPath}
FRA.UninstMsg1=Le programme d'installation a détecté qu'une autre version de programme est en cours d'exécution sur votre ordinateur.
FRA.UninstMsg2=Pour installer la version actuelle, vous avez besoin de déplacer la version antérieure d’abord.    
FRA.AcceptLicense=J'ai bien lu et j'accepte les conditions d'utilisation
FRA.Buy=Acheter
FRA.Launch=Lancer
FRA.TakePlan=Rejoignez notre programme d'amélioration de l'expérience utilisateur.
FRA.UninstMsg3=Félicitations, vous avez bien installé Wondershare Filmora!
FRA.UninstMsg4=Vous utilisez une version d'essai; un filigrane sera appliqué sur les vidéos générées.
FRA.UninstMsg5=Les avantages dont bénéficient les utilisateurs enregistrés comprennent:
FRA.UninstMsg6=1. Pas de filigrane sur les vidéos générées.
FRA.UninstMsg7=2. Assistance technique gratuite.
FRA.UninstMsg8=3. Mises à jour gratuite.
FRA.UninstMsg9=Voulez-vous supprimer tous les fichiers de ressources de l'effet?
FRA.PlatformError=Sorry, this program can not be installed on Win32.

ITA.ProgramOrderOnTheWeb=Acquista
ITA.ProgramHelp=Aiuto online
ITA.ProgramOnTheWeb=Pagina Prodotto
ITA.AppName={#AppName}
ITA.AppPath={#AppPath}
ITA.UninstMsg1=E' stata rilevata un'altra versione installata sul tuo computer.
ITA.UninstMsg2=Per installare questa versione, dovrà essere rimossa quella attuale.     
ITA.AcceptLicense=Ho letto e accetto l'accordo
ITA.Buy=Acquista ora
ITA.Launch=Lancia
ITA.TakePlan=Aderisci al programma di miglioramento dell'esperienza utente.
ITA.UninstMsg3=Congratulazioni, hai installato Wondershare Filmora con successo.
ITA.UninstMsg4=Stai utilizzando una versione prova, una filigrana comparirà nel video risultato.
ITA.UninstMsg5=I vantaggi di un utente registrato comprendono:
ITA.UninstMsg6=1. Nessuna filigrana sul video risultato.
ITA.UninstMsg7=2. Assistenza tecnica gratuita.
ITA.UninstMsg8=3. Aggiornamenti gratuiti. 
ITA.UninstMsg9=Vuoi rimuovere tutti i file di risorse effetto?	
ITA.PlatformError=Sorry, this program can not be installed on Win32.

;ZHH.ProgramOrderOnTheWeb=購買
;ZHH.ProgramHelp=在線幫助
;ZHH.ProgramOnTheWeb=產品頁
;ZHH.AppName={#AppName}
;ZHH.AppPath={#AppPath}
;ZHH.UninstMsg1=安裝程式檢測到另一個版本已經在您的電腦上。
;ZHH.UninstMsg2=繼續安裝最新版本，請先卸下舊版本。  
;ZHH.AcceptLicense=我已閱讀並接受協議
;ZHH.Buy=購買
;ZHH.Launch=運行
;ZHH.TakePlan=加入用戶體驗改進計劃。
;ZHH.UninstMsg3=恭喜，您已成功安裝Wondershare Filmora。
;ZHH.UninstMsg4=您現在使用的是試用版，水印將顯示在輸出的視頻上。
;ZHH.UninstMsg5=註冊用戶可獲得：
;ZHH.UninstMsg6=1. 輸出視頻無水印影響。
;ZHH.UninstMsg7=2. 免費技術支持。
;ZHH.UninstMsg8=3. 免費產品升級。  
;ZHH.UninstMsg9=是否刪除已安裝的效果資源文件以節約磁碟空間？

;CHT.ProgramOrderOnTheWeb=購買
;CHT.ProgramHelp=在線幫助
;CHT.ProgramOnTheWeb=產品頁
;CHT.AppName={#AppName}
;CHT.AppPath={#AppPath}
;CHT.UninstMsg1=安裝程式檢測到另一個版本已經在您的電腦上。
;CHT.UninstMsg2=繼續安裝最新版本，請先卸下舊版本。   
;CHT.AcceptLicense=我已閱讀並接受協議
;CHT.Buy=購買
;CHT.Launch=運行
;CHT.TakePlan=加入用戶體驗改進計劃。
;CHT.UninstMsg3=恭喜，您已成功安裝Wondershare Filmora。
;CHT.UninstMsg4=您現在使用的是試用版，水印將顯示在輸出的視頻上。
;CHT.UninstMsg5=註冊用戶可獲得：
;CHT.UninstMsg6=1. 輸出視頻無水印影響。
;CHT.UninstMsg7=2. 免費技術支持。
;CHT.UninstMsg8=3. 免費產品升級。    
;CHT.UninstMsg9=是否刪除已安裝的效果資源文件以節約磁碟空間？

;NLD.ProgramOrderOnTheWeb=Bestel %1 online
;NLD.ProgramHelp=Hoe gebruik je %1
;NLD.ProgramOnTheWeb=%1 online
;NLD.AppName={#AppName}
;NLD.AppPath={#AppPath}
;NLD.UninstMsg1=De set-up detecteerde dat er een eerdere versie op je computer is ge?nstalleerd.
;NLD.UninstMsg2=On de huidige versie te installeren, verwijder de huidige versie. 
;NLD.AcceptLicense=Ik heb de overeenkomst gelezen & accepteer het
;NLD.Buy=Koop nu
;NLD.Launch=Lancering
;NLD.TakePlan=Meld je aan bij het ervaring verbetering programma.
;NLD.UninstMsg3=Gefeliciteerd, je hebt zonet Wondershare Filmora succesvol ge?nstalleerd.
;NLD.UninstMsg4=Je gebruikt momenteel de test versie, een watermark zal worden getoond op je output video.
;NLD.UninstMsg5=De voordelen van een geregistreerde gebruiker zijn:
;NLD.UninstMsg6=1. Geen watermark op de output video.
;NLD.UninstMsg7=2. Gratis technische ondersteuning.
;NLD.UninstMsg8=3. Gratis upgrades.         
;NLD.UninstMsg9=Wilt u alle effect bestanden te verwijderen?	

;RUS.ProgramOrderOnTheWeb=Закажите %1 в Интернете
;RUS.ProgramHelp=Как использовать %1
;RUS.ProgramOnTheWeb=%1 в Интернете
;RUS.AppName={#AppName}
;RUS.AppPath={#AppPath}
;RUS.UninstMsg1=Программа установки обнаружила, что на этом компьютере уже установлена другая версия.
;RUS.UninstMsg2=Чтобы установить данную версию, надо сначала удалить старую. 
;RUS.AcceptLicense=Я прочитал(а) и &принимаю соглашение
;RUS.Buy=Купить сейчас
;RUS.Launch=Запустить
;RUS.TakePlan=Присоединиться к программе улучшения пользовательских возможностей.
;RUS.UninstMsg3=Поздравляем! Вы успешно установили Wondershare Filmora.                   VideoEditor
;RUS.UninstMsg4=Вы используете пробную версию, поэтому к выходному видео будет добавлен водяной знак.
;RUS.UninstMsg5=Преимущества регистрации заключаются в следующем:
;RUS.UninstMsg6=1. Нет водяного знака на полученных видео.
;RUS.UninstMsg7=2. Бесплатная техническая поддержка.
;RUS.UninstMsg8=3. Бесплатные обновления.    
;RUS.UninstMsg9=Вы хотите, чтобы удалить все файлы ресурсов эффект?	

;ARA.ProgramOrderOnTheWeb=شراء
;ARA.ProgramHelp=البرنامج التعليمي
;ARA.ProgramOnTheWeb=صفحة المنتج
;ARA.AppName={#AppName}
;ARA.AppPath={#AppPath}
;ARA.UninstMsg1=تم الكشف عن وجود نسخة مثبتة على جهاز الكومبيوتر الخاص بك أثناء التنصيب
;ARA.UninstMsg2=.لتقوم بتثبيت الأصدار الحالي،عليك أزالة الأصدار القديم
;ARA.AcceptLicense=لقد قرأت ووافقت على الأتفاقية
;ARA.Buy=اشتر الآن
;ARA.Launch=إطلاق
;ARA.TakePlan=أنضم لبرنامج تحسين تجربة المستخدم
;ARA.UninstMsg3=بنجاح wondershare Filmoraمبروك،لقد قمت بتثبيت
;ARA.UninstMsg4=أنت تستخدم نسخة تجريبية،سيتم عرض علامة مائية في الفيديو النهائي
;ARA.UninstMsg5=:تشمل فوائد المستخدم المُسجل
;ARA.UninstMsg6=أزالة العلامة المائية من الفيديو النهائي
;ARA.UninstMsg7=دعم فني مجاني
;ARA.UninstMsg8=تحديثات مجانية
;ARA.UninstMsg9=هل ترغب بأزالة جميع ملفات مصدر المؤثرات؟

[Messages]
;BeveledLabel=Wondershare Software

[Dirs]
Name: {app}\Log;
Name: {commonappdata}\{#OldAppName}\Resources;  Permissions: users-full

[Files]
;日语安装图片,放在第一位
Source: "WizModernImage_jp.bmp"; Flags: dontcopy

Source: "{#BinPath}\AudioPlugins\*"; DestDir: "{app}\AudioPlugins"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\bearer\*"; DestDir: "{app}\bearer"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Captions\*"; DestDir: "{app}\Captions"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\DecPlugins\*"; DestDir: "{app}\DecPlugins"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Effect\*"; DestDir: "{app}\Effect"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Fonts\*"; DestDir: "{app}\Fonts"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\imageformats\*"; DestDir: "{app}\imageformats"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Mask\*"; DestDir: "{app}\Mask"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\MontageTemplate\*"; DestDir: "{app}\MontageTemplate"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\MotionTemplate\*"; DestDir: "{app}\MotionTemplate"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\platforms\*"; DestDir: "{app}\platforms"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\PlugIns\*"; DestDir: "{app}\PlugIns"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Sounds\*"; DestDir: "{app}\Sounds"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\UIFonts\*"; DestDir: "{app}\UIFonts"; Flags:recursesubdirs createallsubdirs ignoreversion
Source: "{#BinPath}\Lang\*"; DestDir: "{app}\Lang"; Flags:recursesubdirs createallsubdirs ignoreversion

Source: "{#BinPath}\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BinPath}\*.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BinPath}\*.xml"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-full
Source: "{#BinPath}\*.inf"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-full
Source: "{#BinPath}\*.ini"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-full
Source: "{#BinPath}\Format.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BinPath}\HotkeyParma.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BinPath}\CFDecode64.ax"; DestDir: "{app}"; Flags: ignoreversion
;代理商定制文件
Source: "{#BinPath}\{#MyAppCustomizedFileName}"; DestDir: "{app}"; Flags: ignoreversion uninsneveruninstall; check: ProcessAgentData();

;代理商文件处理工具...ProcessAgentData()有用，所以置在下句之前
Source: "{#BinPath}\WSUtilities.dll"; DestDir: {app}; Flags: ignoreversion
;************ 平台客户端程序, Source要根据具体项目的路径修改,DestDir 固定不能修改, Flags使用ignoreversion，因为需要比较版本
;Source: "{#BinPath}\{#CBSHelperExeName}"; DestDir: {app}; Flags: ignoreversion;

Source: "{#BinPath}\WSUtilities_Setup.dll";  DestDir: {app}; Flags: ignoreversion
Source: "{#BinPath}\WS_Log_Setup.dll";  DestDir: {app}; Flags: ignoreversion

;下面文件，虽由程序生成，但需要开权限，所以加入
;Source: "{#BinPath}\{#ConfigName}"; DestDir: {app}; Flags: ignoreversion uninsneveruninstall onlyifdoesntexist; Permissions: users-full
;Source: "{#BinPath}\VideoEditor.exe"; DestDir: {app}; Flags: ignoreversion;

;往系统目录安装一些字体库，安装UIFonts下的字体文件
Source: "{#BinPath}\UIFonts\Lato-Regular.ttf"; DestDir: "{fonts}"; FontInstall: "Lato"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "{#BinPath}\UIFonts\Lato-Black.ttf"; DestDir: "{fonts}"; FontInstall: "Lato Black"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "{#BinPath}\UIFonts\Lato-Bold.ttf"; DestDir: "{fonts}"; FontInstall: "Lato"; Flags: onlyifdoesntexist uninsneveruninstall

[Tasks]
Name: desktopicon; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
;Name: quicklaunchicon; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]
;Name: "{commondesktop}\{#AppName}"; Filename:"{app}\{#RecorderExeName}"; Tasks: desktopicon
;Name: "{group}\{#AppName}"; Filename: "{app}\{#RecorderExeName}"
;Name: "{group}\{cm:UninstallProgram,{#AppName}}"; Filename: "{uninstallexe}"
Name: "{group}\{cm:AppName}"; Filename: "{app}\{#NewExeName}"; WorkingDir: "{app}"
;Name: "{group}\{cm:ProgramOnTheWeb,{cm:AppName}}"; Filename: "{app}\{#ProductShortName}_Homepage.url"; iconfilename: {sys}\url.dll; iconindex: 0
;Name: "{group}\{cm:ProgramHelp,{cm:AppName}}"; Filename: "{app}\{#ProductShortName}_Help.url"; iconfilename: {sys}\url.dll; iconindex: 0
;Name: "{group}\{cm:ProgramOrderOnTheWeb,{cm:AppName}}"; Filename: "{app}\{#ProductShortName}_Order.url"; iconfilename: {sys}\url.dll; iconindex: 0
Name: "{group}\{cm:UninstallProgram,{cm:AppName}}"; Filename: "{uninstallexe}"
;liujw于2011-09-20去掉了Comment: "{#AppName}";这句，在有些系统如Vista、Win7，一刷新，会同时显示两个{#AppName}出来。所以，一个足矣
Name: "{commondesktop}\{cm:AppName}"; Filename: "{app}\{#NewExeName}"; Tasks: desktopicon; WorkingDir: "{app}"
;Name: "{commonappdata}\Microsoft\Internet Explorer\Quick Launch\{cm:AppName}"; Filename: "{app}\{#ExeName}"; Tasks: quicklaunchicon; WorkingDir: "{app}"

[INI]
;直接向Icons中写地址，在Vista、Win 7中写不进去，所以用url来做
Filename: "{app}\{#ProductShortName}_Homepage.url"; Section: "InternetShortcut"; Key: "URL"; String: "{code:GetAgentValue|ProductPage}"
Filename: "{app}\{#ProductShortName}_Help.url"; Section: "InternetShortcut"; Key: "URL"; String: "{code:GetAgentValue|HelpOnlinePage}"
Filename: "{app}\{#ProductShortName}_Order.url"; Section: "InternetShortcut"; Key: "URL"; String: "{code:GetAgentValue|OrderPage}"

;[Registry]
;Root: HKCU; Subkey: "SOFTWARE\Wondershare\FilmoraScreen"; ValueType: string; ValueName: "Path"; ValueData: "{app}\{#RecorderExeName}"; Flags: uninsdeletekey
;Root: HKCU; Subkey: "SOFTWARE\Wondershare\FilmoraScreen"; ValueType: string; ValueName: "Unins"; ValueData: "{app}\unins000.exe"; Flags: uninsdeletekey

[Registry]
;工程文件关联
Root: HKCR; Subkey: "{#ProjectExt}"; ValueType: string; ValueName: ""; ValueData: "{#ProjNodeName}"; Flags: uninsdeletekey
Root: HKCR; Subkey: "{#ProjNodeName}"; ValueType: string; ValueName: ""; ValueData: "{#ProductName} Project"; Flags: uninsdeletekey
Root: HKCR; Subkey: "{#ProjNodeName}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#NewExeName},1"
Root: HKCR; Subkey: "{#ProjNodeName}\Shell\Open"; ValueType: string; ValueName: ""; ValueData: "&Open"
Root: HKCR; Subkey: "{#ProjNodeName}\Shell\Open\Command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#NewExeName}"" ""%1"""
Root: HKCR; Subkey: "{#ProjNodeName}"; ValueType: string; ValueName: "path"; ValueData: {app}; Flags: uninsdeletekey
Root: HKCR; Subkey: "{#ProjNodeName}"; ValueType: string; ValueName: "Version"; ValueData: {#AppVersion}; Flags: uninsdeletekey
;为2.0安装时能判断此版本而加
Root: HKLM; SubKey: "SOFTWARE\Wondershare\3235"; ValueType: string; Valuename: "ExePath"; ValueData: "{app}\{#NewExeName}"
;打开注册组件权限
#ifdef CPUX64
Root: HKCR; SubKey: "Wow6432Node\CLSID\{#SGUID}\Info"; Permissions: EveryOne-full
#else
Root: HKCR; SubKey: "CLSID\{#SGUID}\Info"; Permissions: EveryOne-full
#endif
Root: HKCR; SubKey: SOFTWARE\CineForm\DecoderProperties; Permissions: EveryOne-full

Root: HKLM; Subkey: SOFTWARE\{#Publisher}\{#ProductID}; Permissions: EveryOne-full
Root: HKLM; Subkey: SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}; Permissions: EveryOne-full
Root: HKLM; Subkey: SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}; ValueType: DWORD; ValueName: PID; ValueData: {#ProductID}
Root: HKLM; Subkey: SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}; ValueType: string; ValueName: Version; ValueData: {#AppVersion}; Flags: uninsdeleteValue
Root: HKLM; Subkey: SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}; ValueType: string; ValueName: InstallPath; ValueData: {app}; Flags: uninsdeleteValue

;指定内置IE控件需要的IE版本
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION"; ValueType: DWORD; ValueName: {#NewExeName}; ValueData: 9999; Flags: deletevalue
;以下一项针对实现广告部提醒消息需求的产品
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: {#CBSHelperExeName}; ValueData: {pf}\Common Files\{#Publisher}\{#CBSHelperAppName}\{#CBSHelperShortExeName}; Permissions: everyone-modify;
[Dirs]
;不设置Permissions，不避免在Vista、Win 7下其会显示为共享文件夹从而不让删除的问题。因为日志等原因，安装目录共享为users-full
;2012-02-01验证发现，只有[我的文档]中文件夹若EveryOne-Full权限，则有共享标记。测试删除Log运行不能重建Log文件夹，因此开放其权限
Name: {app}; Permissions: users-full
;Name: {app}\ActionCam; Permissions: users-full
Name: {code:GetPersonalFolder}\{cm:AppName}

[Run]
Filename: {sys}\regsvr32.exe; WorkingDir: {sys}; Parameters: /s atimpenc.dll
Filename: {sys}\regsvr32.exe; WorkingDir: {sys}; Parameters: /s atixcode.dll
#ifdef CPUX64
Filename: {sys}\regsvr32.exe; WorkingDir: {sys}; Parameters: /s CFDecode64.ax
#else
Filename: {sys}\regsvr32.exe; WorkingDir: {sys}; Parameters: /s CFDecode2.ax
#endif
;去除runascurrentuser flag，因为它会继承setup file之Administrator权限，会导致由安装程序启动，在Vista、Win 7中不能拖入文件的问题
;runascurrentuser 解决W10，Launch无效启动应用程序的问题
Filename: "{app}\{#NewExeName}"; Description: "{cm:LaunchProgram,{cm:AppName}}"; Flags: nowait postinstall skipifsilent runascurrentuser
Filename: "{app}\{#CBSHelperExeName}"; Parameters: "/VERYSILENT /SP-";
;Filename: {app}\ImageHost.exe; Parameters: "/RegServer";

;为处理升级定制等消息，覆盖安装不删定制相关的xml及dll文件
[UninstallDelete]
;Type: files; Name: "{app}\*.url"
;Type: filesandordirs; Name: "{app}\Contents"
;Type: filesandordirs; Name: "{app}\EffectPlug"
;Type: filesandordirs; Name: "{app}\Transitions"
;Type: filesandordirs; Name: "{app}\Captions"
;Type: filesandordirs; Name: "{app}\Templates"
;Type: filesandordirs; Name: "{app}\Path"
;Type: filesandordirs; Name: "{app}\Update"

[Code]

function InitializeSetup(): Boolean;
var
  ErrorCode: Integer;
  sMsg, sUnsFile, sCfgFile, sParams: string;
begin
  // 判断系统位数
  if not IsWin64 then
     begin
     MsgBox(ExpandConstant('{cm:PlatformError}'), mbConfirmation, MB_OK);
     Result := False;
     end
  else
     begin
     //静默安装
     if IsVERYSILENTSetup then
        begin
        KillProcess('FSRecorder.exe'); 
        KillProcess('FSEditor.exe');
        sleep(3000);
     end;
     Result := True;
     //sMsg := 'The Setup has detected that another version is already on your computer. ' + #13 +
     //        'To install current version, it has to be removed first.';
     sMsg := ExpandConstant('{cm:UninstMsg1}') + #13 + ExpandConstant('{cm:UninstMsg2}');

     sUnsFile := UninstallExeFile();
     if FileExists(sUnsFile) then
        begin
        //自动更新，不提示
        if IsUpdateSetup() or IsDownloaderSetup() or (MsgBox(sMsg, mbConfirmation, MB_YESNO) = IDYES) then
           begin
           //重安装
           g_overInstall := True;
           //若用户改安装目录，或是在线升级后安装，从原配置文件中取注册信息
           sCfgFile := ExtractFilePath(sUnsFile) + ExpandConstant('{#UpdateRegisterConfigName}');
           if FileExists(sCfgFile) then
              begin
              g_regMail := GetIniString('System', 'Email', '', sCfgFile);
              //2.0版本，其Email不在System中，而在UpdateRegister中
              if g_regMail = '' then g_regMail := GetIniString('UpdateRegister', 'Email', '', sCfgFile);
              g_regCode := GetIniString('UpdateRegister', 'RegkeyCode', '', sCfgFile);
           end;
           //结束正在运行的程序
           //TerminateProgram();
           sParams := '/VERYSILENT';
           //非覆盖安装，以/REINSTALL标记之，使卸载更为完全
           sParams := sParams + ' /REINSTALL';
           ShellExec('open', sUnsFile, sParams, '', SW_SHOWNORMAL, ewWaitUntilTerminated, ErrorCode);
           //if b = true then MsgBox('True', mbConfirmation, MB_YESNO);
           //MsgBox(IntToStr(ErrorCode), mbConfirmation, MB_YESNO);   // test
           //MsgBox(SysErrorMessage(ErrorCode), mbConfirmation, MB_YESNO);   // test
           //if ErrorCode = 0 then
           //begin
           //repeat
	       //Sleep(100);
           //Inc(ErrorCode);
	       //until not DirExists(ExtractFilePath(sUnsFile)) or (ErrorCode >= 50);
           //end;
           //下面一句，避免在安装时，已安装过且正在运行中处理编辑状态，点击[Cancel]后仍能安装的问题
           //Result := not CheckProgramRunning();
           //Result := True;
           //MsgBox(IntToStr(ErrorCode), mbConfirmation, MB_YESNO);   // test
           //CheckProgramRunning();
           Result := True;
        end
        else Result := False;
     end;
  end;
end;


procedure InitializeWizard();
begin
  with WizardForm do
  begin
    Caption := ExpandConstant('{cm:AppName}');
	//设置顶部Panel颜色，以与LOGO图片背景色一致，避免在非White窗体色中颜色不一致问题
    MainPanel.Color := clWhite;                                                                     

    //调整LOGO位置大小
    WizardSmallBitmapImage.Width := ScaleX(141);
    WizardSmallBitmapImage.Height := ScaleY(50);
    WizardSmallBitmapImage.Left := width - WizardSmallBitmapImage.Width - ScaleX(10);
    PageDescriptionLabel.Width := width - WizardSmallBitmapImage.Width - PageDescriptionLabel.Left - ScaleX(5);
    PageNameLabel.Width := width - WizardSmallBitmapImage.Width - PageNameLabel.Left - ScaleX(5);
    //替换向导图片
    if ExpandConstant('{language}')='JPN' then
    begin
      ExtractTemporaryFile('WizModernImage_jp.bmp');
      WizardBitmapImage.Bitmap.LoadFromFile(ExpandConstant('{tmp}\WizModernImage_jp.bmp'));
      WizardBitmapImage2.Bitmap.LoadFromFile(ExpandConstant('{tmp}\WizModernImage_jp.bmp'));
    end;
    //修改文字字体
    WelcomeLabel1.Font.Name := 'Tahoma';

    //定制License页面...
    LicenseMemo.Height := LicenseMemo.Height + 24;
    //隐藏是否同意License的RadioButton
    LicenseAcceptedRadio.Visible := False;
    LicenseNotAcceptedRadio.Visible := False;
    //创建是否同意License的CheckBox
    CreateLicenseCheckBox(LicensePage, LicenseAcceptedRadio.Left, LicenseAcceptedRadio.Top + 20);

    //设置Title标签
    PageNameLabel.Font.Size := 15;
    PageNameLabel.Font.Color := $00594E36;
    PageNameLabel.Top := PageNameLabel.Top + 6;
    PageNameLabel.Height := ScaleY(28);
    PageDescriptionLabel.Visible := False;
  end;

  //制作Finished页面
  CreateFinishedPage();
end;


function ShouldSkipPage(PageID: Integer): Boolean;
begin
  //在Setup中没有Disable Task的选项，此处略之
  if PageID = wpSelectTasks then Result := True;
end;

function BackButtonClick(CurPageID: Integer): Boolean;
var
  ErrorCode: Integer;
  sUrl: string;
begin
  //执行[立即购买]功能
  if CurPageID = 100 then
  begin
    Result := False;
    sUrl := GetAgentValue('OrderPage');
    if Pos(':', sUrl) = 0 then sUrl := 'http://' + sUrl;
    //MsgBox(sUrl, mbConfirmation, MB_YESNO);       // test
    //sUrl := 'http://cbs.wondershare.com/go.php?pid=3235&m=db';
    ShellExec('open', sUrl, '', '', SW_SHOW, ewNoWait, ErrorCode);
  end
  else Result := True;
end;


procedure CurPageChanged(CurPageID: Integer);
var
  sInstallPatsh: string;
  ExeFile, sParam: string;
  ErrorCode: integer;
  temp: string;
  LibFileName, WbemFileName: string;
begin
  with WizardForm do
  begin
    PageNameLabel.Caption := ExpandConstant('{cm:AppName}');
    if CurPageID = wpSelectDir then
    begin
      if IsSilentInstall then
      begin
        sInstallPatsh:= GetParamByName('/installpath:');
        if sInstallPatsh <> '' then
          DirEdit.text := sInstallPatsh ;
      end;
    end;
  #ifndef CPUX64
    //加载字体缓存
    if CurPageID = wpInstalling then
    begin                                 
      ExtractTemporaryFile('libkernaldec.dll');
      ExtractTemporaryFile('libMPKernal.dll');
      ExtractTemporaryFile('MediaInfo.dll');
      ExtractTemporaryFile('MPDecSrc.dll');
      ExtractTemporaryFile('pthreadGC2.dll');
      //ExtractTemporaryFile('NLEBuildFontProcess.exe');
      ExtractTemporaryFile('WS_Log.dll');
      ExtractTemporaryFile('BugSplat.dll');
      temp := ExpandConstant('{tmp}\');
      temp := temp + '..\NLEBuildFontProcessVE\';
      if not DirExists(temp) then
        CreateDir(temp);
      FileCopy(ExpandConstant('{tmp}\') + 'BugSplat.dll', temp + 'BugSplat.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'WS_Log.dll', temp + 'WS_Log.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'libkernaldec.dll', temp + 'libkernaldec.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'libMPKernal.dll', temp + 'libMPKernal.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'MediaInfo.dll', temp + 'MediaInfo.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'MPDecSrc.dll', temp + 'MPDecSrc.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'pthreadGC2.dll', temp + 'pthreadGC2.dll', false);
      FileCopy(ExpandConstant('{tmp}\') + 'NLEBuildFontProcess.exe', temp + 'NLEBuildFontProcess.exe', false);
      ExeFile := temp + 'NLEBuildFontProcess.exe'
      sParam := '"'+ExpandConstant('{app}')+ '"';
      if FileExists(ExeFile) then
        ShellExec('', ExeFile,  sParam, '', SW_SHOWNORMAL, ewNoWait, ErrorCode);
      //XP 下卸载程序报TaskKill 缺少framedyn.dll
     LibFileName :=ExpandConstant('{sys}') + '\framedyn.dll' ;
     if not FileExists(LibFileName) then     //如果Sys32不存在这个库
     begin
       WbemFileName := ExpandConstant('{sys}') + '\wbem\framedyn.dll';
       if FileExists(WbemFileName) then
         FileCopy(WbemFileName, LibFileName, False);
     end;  
    end;
#endif
    //定制的Finished页面
    if CurPageID = 100 then
    begin
      BackButton.Left := 10;
      BackButton.Visible := True;
      BackButton.Caption := ExpandConstant('{cm:Buy}');
      BackButton.Width := 100;
      if ExpandConstant('{language}')<>'ARA' then
      NextButton.Left := CancelButton.Left;
      NextButton.Caption := ExpandConstant('{cm:Launch}') + '>>';
    end;
  end;
end;


procedure CurStepChanged(CurStep: TSetupStep);
var
  RegPath, InstallTime: string;
  ErrorCode: Integer;
begin
  //安装完成
  if CurStep = ssPostInstall then
  begin
    //写配置ini文件
    WriteConfigFile();
    //安装winBurnEngine
    InstallWinBurnEngine();
    //Copy MPDecVid.dll->MPDecAud.dll  不需要这个库
    //FileCopy(ExpandConstant('{app}\MPDecVid.dll'), ExpandConstant('{app}\MPDecAud.dll'), False);
    
    //打开跳转链接...
    if IsUpdateSetup() then
      OpenJumpPage(cptOnlineUpdate)
    else if not g_overInstall then OpenJumpPage(cptInstall);
    
    //ShellExec('open', 'http://cbs.wondershare.com/go.php?pid=3235&m=i', '', '', SW_SHOW, ewNoWait, ErrorCode);  //http://cbs.wondershare.com/go.php?pid=3235&m=i; //OpenJumpPage(cptInstall);
	
    //启动检测显卡类型程序CheckGraphicsType.exe
    ShellExec('open',ExpandConstant('{app}') + '\CheckGraphicsType.exe','','',SW_HIDE,ewNoWait,ErrorCode);
  end
  else if CurStep = ssDone then
  begin
    RegPath:= 'SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}';
    InstallTime:= GetDateTimeString('dd/mm/yyyy hh:nn:ss AM/PM', '/', ':');
    RegWriteStringValue(HKEY_LOCAL_MACHINE, RegPath, 'InstallInfo', InstallTime);
    RegWriteDWordValue(HKEY_LOCAL_MACHINE, RegPath, 'PID', StrToInt(GetProductID('{#ProductID}')));
    RegDeleteValue(HKEY_LOCAL_MACHINE, RegPath, 'LastRemindInfo');
    if IsUpdateSetup() then
      ShellExec('', ExpandConstant('{app}\{#NewExeName}'), 'SetupRun', '', SW_SHOW, ewNoWait, ErrorCode);
  end;
end;


//不是自动更新的时候删除APP目录
//如果自动更新的话，则不删除以前安装的菜单模板包
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  ErrorCode: Integer;
  HelperUnistallExe, RegPath, sMsg: string;
begin
  //卸载开始
  if CurUninstallStep = usUninstall then
  begin
#ifdef CPUX64
    ShellExec('open', ExpandConstant('{sys}') + '\regsvr32.exe', '/s /u CFDecode64.ax', ExpandConstant('{sys}'), SW_HIDE, ewWaitUntilTerminated, ErrorCode);
#else
    ShellExec('open', ExpandConstant('{sys}') + '\regsvr32.exe', '/s /u CFDecode2.ax', ExpandConstant('{sys}'), SW_HIDE, ewWaitUntilTerminated, ErrorCode);
#endif
    //ShellExec('open', ExpandConstant('{sys}') + '\TASKKILL.exe', '/F /IM WSResDownloader.exe','', SW_HIDE, ewWaitUntilTerminated, ErrorCode);

    //定制卸载处理，正常卸载则弹出CBS链接...其中已有卸载弹链接处理。写入此处，以保证卸载时文件能删除完全
    if not (IsUpdateSetup() or IsOverUnInstall()) then UninstallCustomizedProcess;    //卸载跳转
    UnloadDLL(ExpandConstant('{app}\WSUtilities_Setup.dll'));
  end
  else if CurUninstallStep = usPostUninstall then
  begin
    //删除字体缓存
    if not (IsUpdateSetup() or IsOverUnInstall()) then
    begin
      DeleteFile(ExpandConstant('{app}\fontcache.dat'));
    end;
    //非升级安装删除安装目录
    if not IsUpdateSetup() or IsOverUnInstall() then
    begin
      //为避免用户安装到其它目录如c:\windows\等而导致误删除，故如此处理
      DeleteFile(ExpandConstant('{app}\{#MyAppCustomizedFileName}'));
      DeleteFile(ExpandConstant('{app}\WSUtilities.dll'));
      DeleteFile(ExpandConstant('{app}\WSUtilities_Setup.dll'));
      DeleteFile(ExpandConstant('{app}\{#ConfigName}'));
      DeleteFile(ExpandConstant('{app}\{#UpdateRegisterConfigName}'));
      //DeleteFile(ExpandConstant('{app}\AVFileResource.xml'));
      //DeleteFile(ExpandConstant('{app}\Favorites.xml'));
	    //DeleteFile(ExpandConstant('{app}\NewConfig.ini'));
	    //DeleteFile(ExpandConstant('{app}\Contents43.wszip'));
	    //DeleteFile(ExpandConstant('{app}\Contents169.wszip'));
	    //DeleteFile(ExpandConstant('{app}\EffectPlug.wszip'));
	    //DeleteFile(ExpandConstant('{app}\Transitions.wszip'));
      //DeleteFile(ExpandConstant('{app}\VideoEditor.map'));
      DeleteFile(ExpandConstant('{app}\fontcache.dat'));
      //DeleteFile(ExpandConstant('{app}\SMTP-xml.txt'));
      DelTree(ExpandConstant('{app}\Log'), True, True, True);
      DelTree(ExpandConstant('{app}\cache'), True, True, True);
      DelTree(ExpandConstant('{app}'), True, True, True);
      //RemoveDir安全，它只移除空目录
      RemoveDir(ExpandConstant('{app}'));
      RemoveDir(ExtractFileDir(ExpandConstant('{app}')));
    end;
    if not IsUpdateSetup() and not IsDownloaderSetup() and not IsOverUnInstall() then
    //begin      //删除effect resources
      //sMsg := ExpandConstant('{cm:UninstMsg9}')  
      //if (MsgBox(sMsg, mbConfirmation, MB_YESNO) = IDYES) then
        //DelTree(ExpandConstant('{commonappdata}\{#OldAppName}'), True, True, True);
    //end;
  end
  else if CurUninstallStep = usDone then
  begin
    RegPath := 'SOFTWARE\{#Publisher}\{#AppName}\';
    RegDeleteKeyIncludingSubkeys(HKEY_LOCAL_MACHINE, RegPath);      // test
    //if res = false then MsgBox('未成功删注册表', mbConfirmation, MB_YESNO) = IDYES);   // test
    //RegPath := 'SOFTWARE\{#Publisher}\{#CBSHelperAppName}';
    //不卸载数据跟踪工具
    //if RegQueryStringValue(HKEY_LOCAL_MACHINE, RegPath, 'Uninstallexe', HelperUnistallExe) then
    //  ShellExec('', HelperUnistallExe, '/VERYSILENT /SP-', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
    //为防止卸不净，做此处理
    //SetRemindSwitch(False);
  end;
end;

end.




