@echo off

SETLOCAL EnableDelayedExpansion
SET ExecutableFile=main.exe

SET Filename=%1
IF NOT EXIST %Filename%.cpp IF NOT EXIST %Filename% (
  ECHO File %Filename% does not exist!
  EXIT /b
)

IF EXIST %ExecutableFile% DEL /F %ExecutableFile%

IF "%Filename:~-4%" NEQ ".cpp" (
  SET Filename=%Filename%.cpp
)

SET LOGONSERVER=\\LOCALHOST
SET MyCommand=g++ -std=c++2a -pedantic -Ofast %Filename% -o %ExecutableFile%
ECHO MAKE: %MyCommand%
%MyCommand%


