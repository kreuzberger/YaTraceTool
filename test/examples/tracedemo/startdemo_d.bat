@echo off
start traceserverd -v
start traceclientd
start tracedemo


echo start your tests
pause


taskkill /F /IM traceserverd.exe /IM traceclientd.exe /IM tracedemo.exe