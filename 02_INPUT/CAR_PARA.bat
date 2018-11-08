@echo off

set directory=%~dp0

>con set /p "input=Input file name here: "
if exist "%input%" (
>con echo Opening scripts...
>con echo Please check log.txt
	python %directory%Main.py -v %input%
) else (
>con echo "File not exist!"
)
exit /b
pause
