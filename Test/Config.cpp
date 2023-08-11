#include "Config.h"
#include<string>
#include<Windows.h>
#include <direct.h>


/// <summary>
/// 读取ini配置文件
/// </summary>
/// <param name="name">配置名称（英文）</param>
/// <param name="def">默认值(当配置不存在时生效)</param>
/// <returns></returns>
int ReadINI(std::string name, int def)
{

    char fName[255];
    char* val = _getcwd(fName, sizeof(fName));
    //fName += "\\config.ini";
    sprintf_s(fName, "%s\\config.ini", fName);
    return GetPrivateProfileIntA("MyFive", name.c_str(), def, fName);
    //return GetPrivateProfileInt(L"MyFive", name, def, L".\\config.ini");
}

class Config
{

};

