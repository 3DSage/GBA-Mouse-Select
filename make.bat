path=C:\devkitadv\bin
@ECHO OFF
SET VAR=0

gcc -c -O3 -mthumb -mthumb-interwork main.c
gcc -mthumb -mthumb-interwork -o main.elf main.o
if "%errorlevel%"=="0" SET VAR=0
if "%errorlevel%"=="1" SET VAR=1
objcopy -O binary main.elf main.gba

if "%VAR%"=="1" pause
if "%VAR%"=="0" del main.o main.elf &start main.gba
