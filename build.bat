@echo off
echo ========================================
echo HWID Utility 编译脚本
echo ========================================
echo.

REM 创建build目录
if not exist build mkdir build
cd build

REM 使用CMake生成项目
echo [1/3] 正在配置CMake项目...
cmake .. -G "Visual Studio 17 2022" 2>nul
if errorlevel 1 (
    echo 尝试使用Visual Studio 16 2019...
    cmake .. -G "Visual Studio 16 2019" 2>nul
)
if errorlevel 1 (
    echo 尝试使用MinGW Makefiles...
    cmake .. -G "MinGW Makefiles"
)

echo.
echo [2/3] 正在编译项目...
cmake --build . --config Release

echo.
echo [3/3] 编译完成!
echo 可执行文件位置: build\Release\hwid_modifier.exe
echo.
echo 请以管理员身份运行程序以获得完整功能
pause
