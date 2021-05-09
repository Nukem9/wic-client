[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
$compression = [System.IO.Compression.CompressionLevel]::Optimal

$projConfig = "Release"
$finalDir = ".\build\"
$tempDir = ".\build_temp\"

Remove-Item -Path $finalDir -Recurse
Remove-Item -Path $tempDir -Recurse
New-Item -Path $finalDir -ItemType Directory
New-Item -Path $tempDir -ItemType Directory


# Generate and package the installer in a zip file
dotnet publish `
    --configuration ${projConfig} `
    --framework net5.0-windows `
    --output ${tempDir} `
    --runtime win-x86 `
    --self-contained true `
    -p:PublishReadyToRun=false `
    -p:PublishSingleFile=true `
    -p:PublishTrimmed=true `
    -p:IncludeAllContentForSelfExtract=true `
    -p:CopyOutputSymbolsToPublishDirectory=false `
    wic_hook.sln

[System.IO.Compression.ZipFile]::CreateFromDirectory($tempDir, "${finalDir}MultiplayerFixInstaller.zip", $compression, $false)
Remove-Item -Path "${tempDir}*" -Recurse


# Package the raw DLLs for those who don't want to use the installer
copy ".\Win32\${projConfig}\dbghelp.dll" "${tempDir}dbghelp.dll"
copy ".\resources\dbghelp_old.dll" "${tempDir}dbghelp_old.dll"
copy ".\Win32\${projConfig}\wic_cl_hook.dll" "${tempDir}wic_cl_hook.dll"
copy ".\Win32\${projConfig}\wic_ds_hook.dll" "${tempDir}wic_ds_hook.dll"
copy ".\Win32\${projConfig}\wic_bt_hook.dll" "${tempDir}wic_bt_hook.dll"
copy ".\Win32\${projConfig}\wic_ed_hook.dll" "${tempDir}wic_ed_hook.dll"

[System.IO.Compression.ZipFile]::CreateFromDirectory($tempDir, "${finalDir}MultiplayerFix.zip", $compression, $false)
Remove-Item -Path $tempDir -Recurse