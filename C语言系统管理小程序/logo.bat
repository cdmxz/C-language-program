echo id ICON "logo.ico" > logo.rc

windres logo.rc  logo.o

gcc C����ϵͳ����С����.c ʵ�ù���.c logo.o -o C����ϵͳ����С����.exe
del logo.o logo.rc
start C����ϵͳ����С����.exe