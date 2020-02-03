echo id ICON "logo.ico" > logo.rc

windres logo.rc  logo.o

gcc C语言系统管理小程序.c 实用功能.c logo.o -o C语言系统管理小程序.exe
del logo.o logo.rc
start C语言系统管理小程序.exe