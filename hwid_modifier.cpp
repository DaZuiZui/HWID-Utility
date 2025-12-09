#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

// 获取硬件信息
class HWIDManager {
private:
    // 获取CPU信息
    std::string getCPUID() {
        int cpuInfo[4] = { 0 };
        __cpuid(cpuInfo, 0);
        
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        ss << std::setw(8) << cpuInfo[3];
        ss << std::setw(8) << cpuInfo[0];
        ss << std::setw(8) << cpuInfo[2];
        ss << std::setw(8) << cpuInfo[1];
        
        return ss.str();
    }
    
    // 获取磁盘序列号
    std::string getDiskSerial() {
        DWORD serialNum = 0;
        DWORD maxComponentLen = 0;
        DWORD fileSystemFlags = 0;
        
        if (GetVolumeInformationA(
            "C:\\",
            nullptr,
            0,
            &serialNum,
            &maxComponentLen,
            &fileSystemFlags,
            nullptr,
            0)) {
            std::stringstream ss;
            ss << std::hex << std::uppercase << serialNum;
            return ss.str();
        }
        return "UNKNOWN";
    }
    
    // 获取MAC地址
    std::string getMACAddress() {
        IP_ADAPTER_INFO adapterInfo[16];
        DWORD bufferSize = sizeof(adapterInfo);
        
        DWORD status = GetAdaptersInfo(adapterInfo, &bufferSize);
        if (status == ERROR_SUCCESS) {
            std::stringstream ss;
            PIP_ADAPTER_INFO adapter = adapterInfo;
            
            ss << std::hex << std::setfill('0');
            for (UINT i = 0; i < adapter->AddressLength; i++) {
                if (i > 0) ss << ":";
                ss << std::setw(2) << static_cast<int>(adapter->Address[i]);
            }
            return ss.str();
        }
        return "UNKNOWN";
    }
    
public:
    // 显示当前HWID信息
    void displayHWID() {
        std::cout << "\n========================================\n";
        std::cout << "当前硬件信息 | Current Hardware Info (HWID):\n";
        std::cout << "========================================\n";
        std::cout << "CPU ID | CPU标识:           " << getCPUID() << "\n";
        std::cout << "Disk Serial | 磁盘序列号:   " << getDiskSerial() << "\n";
        std::cout << "MAC Address | MAC地址:      " << getMACAddress() << "\n";
        std::cout << "========================================\n\n";
    }
    
    // 修改注册表中的硬件信息
    bool modifyHWID(const std::string& newValue) {
        std::cout << "正在修改HWID信息... | Modifying HWID...\n";
        
        // 注意: 实际修改HWID需要管理员权限
        // 这里演示修改注册表中的自定义HWID值
        
        HKEY hKey;
        const char* subKey = "SOFTWARE\\HWID_Utility";
        const char* valueName = "CustomHWID";
        
        // 创建或打开注册表项
        LONG result = RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            subKey,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_WRITE,
            NULL,
            &hKey,
            NULL
        );
        
        if (result == ERROR_SUCCESS) {
            // 写入新的HWID值
            result = RegSetValueExA(
                hKey,
                valueName,
                0,
                REG_SZ,
                (const BYTE*)newValue.c_str(),
                newValue.length() + 1
            );
            
            RegCloseKey(hKey);
            
            if (result == ERROR_SUCCESS) {
                std::cout << "✓ HWID修改成功! | HWID Modified Successfully!\n";
                return true;
            }
        }
        
        std::cout << "✗ HWID修改失败! | HWID Modification Failed! 错误代码 | Error Code: " << result << "\n";
        std::cout << "提示 | Hint: 请以管理员权限运行此程序 | Please run as administrator\n";
        return false;
    }
    
    // 读取自定义HWID
    std::string getCustomHWID() {
        HKEY hKey;
        const char* subKey = "SOFTWARE\\HWID_Utility";
        const char* valueName = "CustomHWID";
        char buffer[256] = { 0 };
        DWORD bufferSize = sizeof(buffer);
        
        LONG result = RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            subKey,
            0,
            KEY_READ,
            &hKey
        );
        
        if (result == ERROR_SUCCESS) {
            result = RegQueryValueExA(
                hKey,
                valueName,
                NULL,
                NULL,
                (LPBYTE)buffer,
                &bufferSize
            );
            
            RegCloseKey(hKey);
            
            if (result == ERROR_SUCCESS) {
                return std::string(buffer);
            }
        }
        
        return "未设置 | Not Set";
    }
    
    // 生成随机HWID
    std::string generateRandomHWID() {
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0');
        
        srand(static_cast<unsigned int>(time(nullptr)));
        
        for (int i = 0; i < 8; i++) {
            if (i > 0 && i % 2 == 0) ss << "-";
            ss << std::setw(4) << (rand() % 0xFFFF);
        }
        
        return ss.str();
    }
};

// 检查是否以管理员权限运行
bool isRunAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

int main() {
    // 设置控制台代码页为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║     HWID 修改工具 v1.0                ║\n";
    std::cout << "║     Hardware ID Modifier              ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    // 检查管理员权限
    if (!isRunAsAdmin()) {
        std::cout << "⚠ 警告 | Warning: 程序未以管理员权限运行 | Not running with administrator privileges\n";
        std::cout << "某些功能可能受限 | Some features may be limited\n";
        std::cout << "请右键选择'以管理员身份运行' | Please right-click and select 'Run as administrator'\n\n";
    }
    
    HWIDManager manager;
    
    // 显示修改前的HWID
    std::cout << "【修改前 | BEFORE MODIFICATION】\n";
    manager.displayHWID();
    std::cout << "自定义HWID | Custom HWID: " << manager.getCustomHWID() << "\n\n";
    
    // 主菜单
    while (true) {
        std::cout << "\n请选择操作 | Please select an option:\n";
        std::cout << "1. 生成并设置随机HWID | Generate and set random HWID\n";
        std::cout << "2. 手动输入HWID | Manually enter HWID\n";
        std::cout << "3. 查看当前HWID | View current HWID\n";
        std::cout << "4. 退出程序 | Exit program\n";
        std::cout << "\n请输入选项 | Enter option (1-4): ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
        case 1: {
            std::string newHWID = manager.generateRandomHWID();
            std::cout << "\n生成的新HWID | Generated New HWID: " << newHWID << "\n";
            
            if (manager.modifyHWID(newHWID)) {
                std::cout << "\n【修改后 | AFTER MODIFICATION】\n";
                manager.displayHWID();
                std::cout << "自定义HWID | Custom HWID: " << manager.getCustomHWID() << "\n";
            }
            break;
        }
        case 2: {
            std::cout << "\n请输入新的HWID | Please enter new HWID: ";
            std::string newHWID;
            std::cin.ignore();
            std::getline(std::cin, newHWID);
            
            if (!newHWID.empty()) {
                if (manager.modifyHWID(newHWID)) {
                    std::cout << "\n【修改后 | AFTER MODIFICATION】\n";
                    manager.displayHWID();
                    std::cout << "自定义HWID | Custom HWID: " << manager.getCustomHWID() << "\n";
                }
            } else {
                std::cout << "输入为空,操作取消 | Input is empty, operation cancelled\n";
            }
            break;
        }
        case 3: {
            std::cout << "\n【当前状态 | CURRENT STATUS】\n";
            manager.displayHWID();
            std::cout << "自定义HWID | Custom HWID: " << manager.getCustomHWID() << "\n";
            break;
        }
        case 4:
            std::cout << "\n感谢使用! 再见! | Thank you! Goodbye!\n";
            return 0;
        default:
            std::cout << "\n无效选项,请重新选择 | Invalid option, please select again\n";
        }
    }
    
    return 0;
}
