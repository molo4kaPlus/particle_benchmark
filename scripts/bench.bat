@echo off
cd /d "%~dp0" 
for /l %%x in (1, 1, 50) do (
    echo executing %%x of 50...
    .\particle.exe
)
echo all tests were done
pause