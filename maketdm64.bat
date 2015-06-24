
SET mypath=%~dp0
echo %mypath:~0,-1%

echo build tdm 64bits
cd %mypath%build_tdm
mingw32-make.exe install

echo build tdm 64bits debug
cd %mypath%build_tdm_debug
mingw32-make.exe install

cd %mypath%

