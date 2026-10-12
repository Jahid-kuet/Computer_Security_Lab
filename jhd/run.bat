@echo off
setlocal EnableExtensions EnableDelayedExpansion

:: -----------------------------------------------------------------------------
:: Directory containing this batch file (with trailing backslash)
:: -----------------------------------------------------------------------------
set "SCRIPT_DIR=%~dp0"

:: -----------------------------------------------------------------------------
:: Automatically discover all .cpp files in the security lab folder
:: This dynamically numbers them (1..N) in alphabetical order
:: -----------------------------------------------------------------------------
set "TOTAL_PROGS=0"
for %%F in ("%SCRIPT_DIR%*.cpp") do (
    set /a TOTAL_PROGS+=1
    set "PROG_!TOTAL_PROGS!=%%~nF"
)

:: -----------------------------------------------------------------------------
:: Read Arguments
:: %1: Number (1..N), Name (e.g. rsa_ED_sign_2key), or Command (all, list, help, run)
:: %2: Destination folder (e.g. "C:\Users\HP\OneDrive\Desktop\test" or ".")
:: %3: Optional flags (e.g. --run)
:: -----------------------------------------------------------------------------
set "ARG1=%~1"
set "ARG2=%~2"
set "ARG3=%~3"

:: If no arguments, show interactive menu
if "%ARG1%"=="" goto :interactive_menu

:: Check Help / Info commands
if /i "%ARG1%"=="help"       goto :show_help
if /i "%ARG1%"=="/?"         goto :show_help
if /i "%ARG1%"=="-h"         goto :show_help
if /i "%ARG1%"=="--help"     goto :show_help
if /i "%ARG1%"=="list"       goto :list_programs
if /i "%ARG1%"=="menu"       goto :interactive_menu

:: Direct execution without copying (e.g. run.bat run 14)
if /i "%ARG1%"=="run" (
    set "TARGET_INPUT=%ARG2%"
    if "!TARGET_INPUT!"=="" (
        echo [ERROR] Please specify a program number or name. Example: run.bat run 11
        exit /b 1
    )
    call :map_program "!TARGET_INPUT!"
    goto :execute_original
)
if /i "%ARG1%"=="exec" (
    set "TARGET_INPUT=%ARG2%"
    if "!TARGET_INPUT!"=="" (
        echo [ERROR] Please specify a program number or name. Example: run.bat exec 11
        exit /b 1
    )
    call :map_program "!TARGET_INPUT!"
    goto :execute_original
)

:: Recompiling in source directory
if /i "%ARG1%"=="build-all"   goto :build_all_sources
if /i "%ARG1%"=="compile-all" goto :build_all_sources
if /i "%ARG1%"=="build" (
    set "TARGET_INPUT=%ARG2%"
    if "!TARGET_INPUT!"==""       goto :build_all_sources
    if /i "!TARGET_INPUT!"=="all" goto :build_all_sources
    call :map_program "!TARGET_INPUT!"
    goto :build_single_source
)
if /i "%ARG1%"=="compile" (
    set "TARGET_INPUT=%ARG2%"
    if "!TARGET_INPUT!"==""       goto :build_all_sources
    if /i "!TARGET_INPUT!"=="all" goto :build_all_sources
    call :map_program "!TARGET_INPUT!"
    goto :build_single_source
)

:: Copy ALL files to destination (e.g. run.bat all "C:\path\to\test")
if /i "%ARG1%"=="all"      goto :copy_all_programs
if /i "%ARG1%"=="copy-all" goto :copy_all_programs

:: -----------------------------------------------------------------------------
:: Main Workflow: Map program and copy to target directory
:: -----------------------------------------------------------------------------
call :map_program "%ARG1%"

set "TARGET_CPP=%SCRIPT_DIR%%PROG%.cpp"
if not exist "%TARGET_CPP%" (
    echo.
    echo [ERROR] Source code file '%PROG%.cpp' was not found in:
    echo         %SCRIPT_DIR%
    echo Run '%~nx0 list' to see available programs.
    exit /b 1
)

:: Determine Destination Directory:
:: Priority 1: 2nd argument passed by user (e.g. "C:\Users\HP\OneDrive\Desktop\test" or ".")
:: Priority 2: If no 2nd argument and caller is in another directory, default to current working directory (%CD%)
:: Priority 3: If caller is inside %SCRIPT_DIR% and gave no 2nd arg, execute the program directly
set "DEST_DIR=%ARG2%"

if "%DEST_DIR%"=="" (
    if /i not "%CD%\"=="%SCRIPT_DIR%" (
        set "DEST_DIR=%CD%"
    ) else (
        goto :execute_original
    )
)

:: Check if 2nd arg was explicitly "--run"
if /i "%DEST_DIR%"=="--run" goto :execute_original
if /i "%DEST_DIR%"=="-r"    goto :execute_original

:: Standardize Destination Directory (remove trailing slash)
if "%DEST_DIR:~-1%"=="\" set "DEST_DIR=%DEST_DIR:~0,-1%"

:: Create target directory if it does not exist
if not exist "%DEST_DIR%" (
    echo [INFO] Destination folder does not exist. Creating:
    echo        "%DEST_DIR%"
    mkdir "%DEST_DIR%" 2>nul
    if errorlevel 1 (
        echo [ERROR] Failed to create folder: "%DEST_DIR%"
        exit /b 1
    )
)

set "DEST_FILE=%DEST_DIR%\%PROG%.cpp"

:: Copy the source file to destination
copy /y "%TARGET_CPP%" "%DEST_FILE%" >nul
if errorlevel 1 (
    echo [ERROR] Failed to copy '%PROG%.cpp' to '%DEST_DIR%'
    exit /b 1
)

echo ============================================================
echo  [SUCCESS] Code file created successfully!
echo ============================================================
echo   File Name   : %PROG%.cpp
echo   Destination : %DEST_FILE%
echo   Source      : %TARGET_CPP%
echo ============================================================
echo.

:: If user asked to also compile/run with 3rd argument (e.g. run.bat 1 "C:\path" --run)
if /i "%ARG3%"=="--run" goto :compile_and_run_target
if /i "%ARG3%"=="-r"    goto :compile_and_run_target

echo You can now open '%PROG%.cpp' directly in Code::Blocks or VS Code!
echo.
echo To compile and run it in your folder:
echo   g++ -O2 "%DEST_FILE%" -o "%DEST_DIR%\%PROG%.exe"
echo   "%DEST_DIR%\%PROG%.exe"
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: Compile & Run in destination folder
:: -----------------------------------------------------------------------------
:compile_and_run_target
echo [COMPILING] g++ -O2 -std=c++17 "%DEST_FILE%" -o "%DEST_DIR%\%PROG%.exe"
g++ -O2 -std=c++17 "%DEST_FILE%" -o "%DEST_DIR%\%PROG%.exe"
if errorlevel 1 (
    echo [ERROR] Compilation failed in destination folder!
    exit /b 1
)
echo [OK] Compilation successful!
echo ============================================================
echo  RUNNING : %PROG%
echo ============================================================
echo.
"%DEST_DIR%\%PROG%.exe"
exit /b %ERRORLEVEL%


:: -----------------------------------------------------------------------------
:: Subroutine: Copy ALL C++ files to target folder
:: -----------------------------------------------------------------------------
:copy_all_programs
set "DEST_DIR=%ARG2%"
if "%DEST_DIR%"=="" set "DEST_DIR=%CD%"
if "%DEST_DIR:~-1%"=="\" set "DEST_DIR=%DEST_DIR:~0,-1%"

if not exist "%DEST_DIR%" (
    echo [INFO] Destination folder does not exist. Creating: "%DEST_DIR%"
    mkdir "%DEST_DIR%" 2>nul
)

echo ============================================================
echo  Copying all %TOTAL_PROGS% Security Lab programs
echo  From: %SCRIPT_DIR%
echo  To  : %DEST_DIR%
echo ============================================================
echo.

set "COUNT=0"
for %%F in ("%SCRIPT_DIR%*.cpp") do (
    set /a COUNT+=1
    copy /y "%%F" "%DEST_DIR%\%%~nxF" >nul
    echo   [!COUNT!] Created %%~nxF
)

echo.
echo ============================================================
echo  [SUCCESS] Copied %COUNT% files to: %DEST_DIR%
echo ============================================================
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: Execute program directly from security lab directory
:: -----------------------------------------------------------------------------
:execute_original
set "TARGET_EXE=%SCRIPT_DIR%%PROG%.exe"
set "TARGET_CPP=%SCRIPT_DIR%%PROG%.cpp"

if not exist "%TARGET_EXE%" (
    if exist "%TARGET_CPP%" (
        echo [INFO] '%PROG%.exe' not found. Compiling '%PROG%.cpp'...
        g++ -O2 -std=c++17 "%TARGET_CPP%" -o "%TARGET_EXE%"
        if errorlevel 1 (
            echo [ERROR] Compilation failed!
            exit /b 1
        )
    ) else (
        echo [ERROR] '%PROG%' not found in %SCRIPT_DIR%
        exit /b 1
    )
)

echo ============================================================
echo  RUNNING : %PROG%
echo  PATH    : %TARGET_EXE%
echo ============================================================
echo.
"%TARGET_EXE%"
set "EXIT_CODE=%ERRORLEVEL%"
echo.
echo ============================================================
echo  Program finished with exit code %EXIT_CODE%
echo ============================================================
exit /b %EXIT_CODE%


:: -----------------------------------------------------------------------------
:: Subroutine: Compile single source in security lab folder
:: -----------------------------------------------------------------------------
:build_single_source
set "TARGET_CPP=%SCRIPT_DIR%%PROG%.cpp"
set "TARGET_EXE=%SCRIPT_DIR%%PROG%.exe"
echo [COMPILING] g++ -O2 -std=c++17 "%TARGET_CPP%" -o "%TARGET_EXE%"
g++ -O2 -std=c++17 "%TARGET_CPP%" -o "%TARGET_EXE%"
if errorlevel 1 (
    echo [ERROR] Compilation failed for %PROG%!
    exit /b 1
)
echo [SUCCESS] Built %PROG%.exe
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: Compile all sources in security lab folder
:: -----------------------------------------------------------------------------
:build_all_sources
echo ============================================================
echo  Compiling all programs in %SCRIPT_DIR%
echo ============================================================
set "COUNT=0"
for %%F in ("%SCRIPT_DIR%*.cpp") do (
    set /a COUNT+=1
    echo [!COUNT!] Compiling %%~nxF ...
    g++ -O2 -std=c++17 "%%F" -o "%SCRIPT_DIR%%%~nF.exe"
)
echo [DONE] All files compiled.
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: Map number (1..N) or raw filename to file identifier
:: -----------------------------------------------------------------------------
:map_program
set "RAW=%~1"
:: Strip extensions if user typed them
if /i "%RAW:~-4%"==".cpp" set "RAW=%RAW:~0,-4%"
if /i "%RAW:~-4%"==".exe" set "RAW=%RAW:~0,-4%"

:: Check if RAW is a valid number mapped to a program
if defined PROG_%RAW% (
    set "PROG=!PROG_%RAW%!"
    exit /b 0
)

:: Otherwise treat RAW directly as program name
set "PROG=%RAW%"
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: Display two-column list of all programs
:: -----------------------------------------------------------------------------
:display_programs_table
set /a "HALF=(TOTAL_PROGS + 1) / 2"
for /l %%I in (1,1,%HALF%) do (
    set "NUM_L=%%I"
    if %%I lss 10 (set "PAD_L= %%I") else (set "PAD_L=%%I")
    set "PROG_L=!PROG_%%I!"
    set "NAME_L=!PROG_L!                               "
    set "COL_L=  [!PAD_L!]  !NAME_L:~0,26!"
    
    set /a "NUM_R=%%I + HALF"
    if !NUM_R! leq %TOTAL_PROGS% (
        if !NUM_R! lss 10 (set "PAD_R= !NUM_R!") else (set "PAD_R=!NUM_R!")
        for %%R in (!NUM_R!) do (
            set "PROG_R=!PROG_%%R!"
            echo !COL_L!  [!PAD_R!]  !PROG_R!
        )
    ) else (
        echo !COL_L!
    )
)
exit /b 0


:: -----------------------------------------------------------------------------
:: Subroutine: List all programs
:: -----------------------------------------------------------------------------
:list_programs
echo.
echo ============================================================
echo           SECURITY LAB PROGRAMS LIST (%TOTAL_PROGS% Available)
echo  Directory: %SCRIPT_DIR%
echo ============================================================
call :display_programs_table
echo ============================================================
echo.
echo Examples:
echo   Create code in 'test' folder:
echo     "%~nx0" 7 "C:\Users\HP\OneDrive\Desktop\test"
echo     "%~nx0" 14 "C:\Users\HP\OneDrive\Desktop\test"
echo   Copy ALL %TOTAL_PROGS% codes to 'test' folder:
echo     "%~nx0" all "C:\Users\HP\OneDrive\Desktop\test"
echo   Run directly without copying:
echo     "%~nx0" run 14
exit /b 0


:: -----------------------------------------------------------------------------
:: Interactive Menu
:: -----------------------------------------------------------------------------
:interactive_menu
cls
echo ============================================================
echo        SECURITY LAB - CODE EXPORTER & RUNNER (%TOTAL_PROGS% Files)
echo  Source: %SCRIPT_DIR%
echo  Current Working Dir: %CD%
echo ============================================================
echo.
call :display_programs_table
echo.
echo   [A]  Copy ALL %TOTAL_PROGS% files to another folder
echo   [R]  Run a program directly
echo   [H]  Help / Command formats
echo   [Q]  Quit
echo ============================================================
set "CHOICE="
set /p "CHOICE=Enter choice (1-%TOTAL_PROGS%, name, A, R, or Q): "

if "%CHOICE%"=="" goto :interactive_menu
if /i "%CHOICE%"=="q" exit /b 0
if /i "%CHOICE%"=="quit" exit /b 0
if /i "%CHOICE%"=="exit" exit /b 0

if /i "%CHOICE%"=="h" (
    call :show_help
    echo.
    pause
    goto :interactive_menu
)

if /i "%CHOICE%"=="a" (
    echo.
    set "DEST="
    set /p "DEST=Enter destination folder (Press Enter for '%CD%'): "
    if "!DEST!"=="" set "DEST=%CD%"
    call :copy_all_programs "!DEST!"
    echo.
    pause
    goto :interactive_menu
)

if /i "%CHOICE%"=="r" (
    echo.
    set "RUN_NUM="
    set /p "RUN_NUM=Enter program number (1-%TOTAL_PROGS%) or name to RUN: "
    if not "!RUN_NUM!"=="" (
        call :map_program "!RUN_NUM!"
        call :execute_original
    )
    echo.
    pause
    goto :interactive_menu
)

:: Number or name selected
call :map_program "%CHOICE%"
echo.
set "DEST="
set /p "DEST=Enter destination folder (Press Enter for '%CD%'): "
if "%DEST%"=="" set "DEST=%CD%"

set "TARGET_CPP=%SCRIPT_DIR%%PROG%.cpp"
if not exist "%TARGET_CPP%" (
    echo [ERROR] Program '%PROG%' was not found.
    pause
    goto :interactive_menu
)

if "%DEST:~-1%"=="\" set "DEST=%DEST:~0,-1%"
if not exist "%DEST%" mkdir "%DEST%" 2>nul
copy /y "%TARGET_CPP%" "%DEST%\%PROG%.cpp" >nul

echo.
echo ============================================================
echo  [SUCCESS] Created %PROG%.cpp in:
echo  %DEST%\%PROG%.cpp
echo ============================================================
echo.
set "RUN_NOW="
set /p "RUN_NOW=Compile and run it now in that folder? (y/n): "
if /i "%RUN_NOW%"=="y" (
    g++ -O2 -std=c++17 "%DEST%\%PROG%.cpp" -o "%DEST%\%PROG%.exe"
    if not errorlevel 1 (
        echo.
        "%DEST%\%PROG%.exe"
    )
)

echo.
set "AGAIN="
set /p "AGAIN=Export another program? (y/n): "
if /i "%AGAIN%"=="y" goto :interactive_menu
exit /b 0


:: -----------------------------------------------------------------------------
:: Help output
:: -----------------------------------------------------------------------------
:show_help
echo.
echo ============================================================
echo                     COMMAND FORMAT GUIDE
echo ============================================================
echo 1. CREATE/COPY A CODE FILE INTO ANOTHER FOLDER:
echo    In PowerShell (VS Code Terminal):
echo      ^& "%SCRIPT_DIR%run.bat" 7 "C:\Users\HP\OneDrive\Desktop\test"
echo      ^& "%SCRIPT_DIR%run.bat" rsa_ED_sign_2key "C:\Users\HP\OneDrive\Desktop\test"
echo.
echo    If your terminal is ALREADY open in the test folder:
echo      ^& "%SCRIPT_DIR%run.bat" 14 .
echo      or simply:
echo      ^& "%SCRIPT_DIR%run.bat" 14
echo.
echo 2. COPY ALL %TOTAL_PROGS% LAB PROGRAMS INTO A FOLDER AT ONCE:
echo      ^& "%SCRIPT_DIR%run.bat" all "C:\Users\HP\OneDrive\Desktop\test"
echo.
echo 3. RUN DIRECTLY (WITHOUT COPYING):
echo      ^& "%SCRIPT_DIR%run.bat" run 14
echo      ^& "%SCRIPT_DIR%run.bat" run rsa_ED_sign_2key
echo.
echo 4. COPY AND IMMEDIATELY RUN:
echo      ^& "%SCRIPT_DIR%run.bat" 14 "C:\Users\HP\OneDrive\Desktop\test" --run
echo ============================================================
exit /b 0
