@echo off
setlocal EnableDelayedExpansion
set EXE=nei.exe
set DIR=tests\examples
set ERR_DIR=tests\errors
set OUT=%TEMP%\nei_tests
set /a pass=0, fail=0, total_ms=0

echo === Building ===
call build.bat
if errorlevel 1 (
    echo Build failed, not running tests.
    exit /b 1
)
echo.

if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%OUT%"

echo === Correctness (small tests) ===
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
echo === Compile errors ===
set /a err_pass=0, err_fail=0
for %%f in ("%ERR_DIR%\*.nei") do (
    "%EXE%" "%%f" < nul > "%OUT%\%%~nf.err.actual" 2>&1
    fc "%OUT%\%%~nf.err.actual" "%%~dpnf.expected" > nul 2>&1
    if errorlevel 1 (
        echo FAIL %%~nxf
        set /a err_fail+=1, fail+=1
    ) else (
        echo PASS %%~nxf
        set /a err_pass+=1, pass+=1
    )
)
if %err_fail%==0 (
    echo All %err_pass% error tests passed, every error was caught.
) else (
    echo %err_fail% error test^(s^) failed, some errors were not reported as expected.
)

echo.
echo === Benchmarks (large tests) ===
rem %%~nf of "bfs.large.in" is "bfs.large", and %%~ng of that is "bfs"
for %%f in ("%DIR%\*.large.in") do (
    for %%g in ("%%~nf") do (
        "%EXE%" "%DIR%\%%~ng.nei" --time < "%%f" > "%OUT%\%%~nf.raw" 2>&1
        findstr /v /b /c:"Runtime: " "%OUT%\%%~nf.raw" > "%OUT%\%%~nf.actual"
        set ms=?
        for /f "tokens=2" %%t in ('findstr /b /c:"Runtime: " "%OUT%\%%~nf.raw"') do set ms=%%t
        if not "!ms!"=="?" set /a total_ms+=ms
        rem pad the name to 24 chars and right-align the time to 6 chars
        set "name=%%~nf                        "
        set "time=      !ms!"
        fc "%OUT%\%%~nf.actual" "%DIR%\%%~nf.expected" > nul 2>&1
        if errorlevel 1 (
            echo FAIL !name:~0,24! !time:~-6! ms
            set /a fail+=1
        ) else (
            echo PASS !name:~0,24! !time:~-6! ms
            set /a pass+=1
        )
    )
)
rem "Total" padded to 29 chars = width of "PASS " plus the 24-char name column
set "time=      %total_ms%"
echo Total                         !time:~-6! ms

echo.
echo %pass% passed, %fail% failed
exit /b %fail%
