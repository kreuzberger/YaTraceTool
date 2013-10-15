@echo off
start yatraceserver -v
start yatraceclient
start yatracedemo


echo start your tests
pause


taskkill /F /IM yatraceserver.exe /IM yatraceclient.exe /IM yatracedemo.exe