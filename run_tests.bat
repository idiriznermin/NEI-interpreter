@echo off
set EXE=nei.exe
set DIR=tests\examples
set OUT=%TEMP%\nei_tests
set /a pass=0, fail=0

if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%OUT%"

for %%f in ("%DIR%\*.nei") do (
    if exist "%%~dpnf.in" (
        "%EXE%" "%%f" < "%%~dpnf.in" > "%OUT%\%%~nf.actual" 2>&1
    ) else (
        "%EXE%" "%%f" < nul > "%OUT%\%%~nf.actual" 2>&1
    )
    fc "%OUT%\%%~nf.actual" "%%~dpnf.expected" > nul 2>&1
    if errorlevel 1 (
        echo FAIL %%~nxf
        set /a fail+=1
    ) else (
        echo PASS %%~nxf
        set /a pass+=1
    )
)

echo.
echo %pass% passed, %fail% failed
exit /b %fail%