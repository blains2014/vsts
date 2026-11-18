[Setup]
AppName=OSOTRUXE (SnoopyChu)
AppVersion=1.0.0
DefaultDirName={commoncf64}\VST3
DisableProgramGroupPage=yes
OutputBaseFilename=OSOTRUXE_Windows_Installer
Compression=lzma
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Files]
Source: "..\build\OSOTRUXE_artefacts\Release\VST3\OSOTRUXE.vst3\*"; DestDir: "{commoncf64}\VST3\OSOTRUXE.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
