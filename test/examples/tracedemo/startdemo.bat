@echo off
start traceserver -v
start traceclient
start tracedemo


echo start your tests
pause


taskkill /F /IM traceserver.exe /IM traceclient.exe /IM tracedemo.exe