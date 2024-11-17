@echo off
REM Compile the project

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra

REM List of source files
set SOURCES=EnemyController.c Main.c GameObject.c Graphics.c WorldObject.c PlayerController.c Utils.c WeaponSystem.c 

REM Output executable name
set OUTPUT=project.exe

REM Compile the project
%CC% %CFLAGS% %SOURCES% -o %OUTPUT%

echo Compilation finished.
pause
