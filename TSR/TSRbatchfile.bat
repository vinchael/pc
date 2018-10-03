@echo off

set directory=%~dp0

for %%f in (*.dat) do (
	echo Creating .txt for: %%~nf
	%directory%Fd_SbrAdtfDebug.exe %%~nf.dat>%%~nf.txt
)
echo Creating .txt Completed

for %%f in (*.txt) do (
	echo Creating valid data for: %%~nf
	python %directory%parseinfofile.py -v %%~nf.txt
)

pause