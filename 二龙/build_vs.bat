@echo off
REM 本地开发编译脚本，勿放入作业提交压缩包
chcp 65001 >nul
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%i"

if "%VSINSTALL%"=="" (
    echo VS C++ tools not found. Install "Desktop development with C++" in Visual Studio Installer.
    pause
    exit /b 1
)

call "%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
    echo Failed to init MSVC environment.
    pause
    exit /b 1
)

cd /d "%~dp0"

cl /nologo /EHsc /std:c++17 /utf-8 /W3 /Fe:ProjectScheduler.exe ^
    main.cpp Task.cpp BasicTask.cpp MilestoneTask.cpp DependencyType.cpp ^
    Dependency.cpp Resource.cpp TaskResourceAssignment.cpp Project.cpp ^
    PpmImporter.cpp PpmExporter.cpp ProjectController.cpp ConsoleView.cpp

if errorlevel 1 (
    echo.
    echo BUILD FAILED.
    pause
    exit /b 1
)

del *.obj 2>nul
echo.
echo BUILD OK: ProjectScheduler.exe
echo.
echo Starting program...
echo.
ProjectScheduler.exe
