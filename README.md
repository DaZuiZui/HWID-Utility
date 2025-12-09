# HWID Utility - 硬件ID修改工具

一个用C++编写的Windows硬件ID(HWID)查看和修改工具。

## 功能特性

- ✅ 查看当前硬件信息(CPU ID、磁盘序列号、MAC地址)
- ✅ 生成随机HWID
- ✅ 自定义HWID设置
- ✅ 对比修改前后的效果
- ✅ 友好的中文界面

## 系统要求

- Windows 7 或更高版本
- 管理员权限(用于修改注册表)
- C++17 编译器(Visual Studio 2017+ 或 MinGW)

## 编译方法

### 方法1: 使用Visual Studio

1. 打开Visual Studio 2019/2022
2. 打开项目文件夹
3. 使用CMake工具编译

### 方法2: 使用命令行和CMake

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 方法3: 使用提供的批处理脚本

```bash
build.bat
```

## 使用说明

1. **以管理员身份运行程序**
   - 右键点击 `hwid_modifier.exe`
   - 选择"以管理员身份运行"

2. **查看当前HWID**
   - 程序启动时会自动显示当前硬件信息

3. **修改HWID**
   - 选项1: 生成随机HWID
   - 选项2: 手动输入自定义HWID
   - 选项3: 查看当前状态

4. **对比效果**
   - 修改前后会显示完整的硬件信息对比

## 文件结构

```
HWID-Utility/
├── hwid_modifier.cpp   # 主程序源代码
├── CMakeLists.txt      # CMake配置文件
├── build.bat           # Windows编译脚本
└── README.md           # 说明文档
```

## 技术说明

本程序通过以下方式获取和修改HWID:

1. **CPU ID**: 使用 `__cpuid` 指令获取CPU唯一标识
2. **磁盘序列号**: 通过 `GetVolumeInformation` API获取
3. **MAC地址**: 使用 `GetAdaptersInfo` 获取网卡物理地址
4. **自定义HWID**: 存储在注册表 `HKLM\SOFTWARE\HWID_Utility\CustomHWID`

## 注意事项

⚠️ **重要提示**:

- 本工具仅用于学习和测试目的
- 修改硬件信息可能违反某些软件的使用条款
- 请确保在法律允许的范围内使用
- 需要管理员权限才能修改注册表
- 某些硬件信息是只读的,无法直接修改

## 安全性

- 程序会检查是否以管理员权限运行
- 所有注册表操作都有错误处理
- 不会修改系统关键配置

## 许可证

MIT License

## 作者

DaZuiZui

## 贡献

欢迎提交Issue和Pull Request!

## 更新日志

### v1.0 (2025-12-09)
- ✨ 初始版本发布
- ✨ 支持查看CPU、磁盘、MAC地址信息
- ✨ 支持生成和设置自定义HWID
- ✨ 显示修改前后对比效果
