@echo off

set directory=%~dp0

set /p "input=Input file name here: "
if exist "%input%" (
	echo Opening scripts...
	python %directory%MCUAppAutomation.py -v %input%

) else (
	echo "File not exist!"
)


for /R %directory% %%f in (*.c) do (
	echo Creating global variable for: %%~nf.c
	python %directory%MergingTextFile.py -v %%~nf.c -d %directory%DataExtracted.xlsx
)

pause

