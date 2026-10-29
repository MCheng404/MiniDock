@echo off
windres -O coff res/app.rc -o app_res.o -Ires
gcc -Os -mwindows -s -static-libgcc -o MiniDock.exe minidock.c app_res.o -lgdiplus -lole32 -loleaut32 -lshell32 -lshlwapi -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -luuid -lurlmon -lversion -ldwmapi -ladvapi32 -lpsapi
