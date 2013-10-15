@echo off
start yatraceserverd -v
start yatraceclientd
start yatracedemo


echo start your tests
pause


taskkill /F /IM yatraceserverd.exe /IM yatraceclientd.exe /IM yatracedemo.exe