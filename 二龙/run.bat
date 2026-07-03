@echo off
REM 本地开发用，勿放入作业提交压缩包
chcp 65001 >nul
cd /d "%~dp0"
if not exist ProjectScheduler.exe (
    echo 请先运行 build_vs.bat 编译
    pause
    exit /b 1
)
ProjectScheduler.exe
