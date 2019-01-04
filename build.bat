@echo off
IF NOT EXIST build mkdir build
pushd build
cl /Zi ../source/win32_main.c /link user32.lib gdi32.lib /out:handmade_network_game.exe
popd