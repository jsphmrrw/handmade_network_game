@echo off



set compiler_options= /GR- /nologo /FC /Zi
set linker_options= user32.lib gdi32.lib



rem ---------------------------------------------------------------------------
rem This will ensure that the script knows about "cl", assuming that it is 
rem installed.
rem ---------------------------------------------------------------------------
set code_home=%~dp0
if %code_home:~-1%==\ (set code_home=%code_home:~0,-1%)
if NOT "%Platform%" == "X64" IF NOT "%Platform%" == "x64" (call "%code_home%\windows_scripts\setup_cl_x64.bat")



rem ---------------------------------------------------------------------------
rem Builds the program.
rem ---------------------------------------------------------------------------
IF NOT EXIST build mkdir build
pushd build
cl %compiler_options% ../source/win32_main.c /link %linker_options% /out:handmade_network_game.exe
popd