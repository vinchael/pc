@echo off

set directory=%~dp0

>con set /p "input=Input file name here: "
if exist "%input%" (
>con echo Opening scripts...
	python %directory%MCUAppAutomation.py -v %input%
) else (
>con echo "File not exist!"
)


for /R %directory% %%f in (*.c) do (
>con echo Creating global variable for %%~nf.c
	python %directory%MergingTextFile.py -v %%~nf.c -d %directory%ForMergingTextFile.xlsx
)

for /R %directory% %%f in (*.c) do (
>con echo Creating function call for %%~nf.c
	python %directory%FunctionVariable.py -v %%~nf.c -d %directory%ForFunctionVariable.xlsx
	python %directory%MCUAppMethod.py -v %%~nf.c -d %directory%ForMCUAppMethod.xlsx
)
exit /b
pause
