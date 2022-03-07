@echo off

SET ExecutableFile=main.exe

SET LOGONSERVER=\\LOCALHOST
SET MyCommand=%ExecutableFile% %*
ECHO RUN: %MyCommand%
%MyCommand%
