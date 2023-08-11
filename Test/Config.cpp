#include "Config.h"
#include<string>
#include<Windows.h>
#include <direct.h>
int  ReadINI()
{
    return 0;
}

int ReadINI(std::string name, int def)
{

    char fName[255];
    char* val = _getcwd(fName, sizeof(fName));
    //fName += "\\config.ini";

    MessageBoxA(NULL, fName, "ÌבÊ¾", 0);
    sprintf_s(fName, "%s\\config.ini", fName);
    return GetPrivateProfileIntA("MyFive", name.c_str(), def, fName);
    //return GetPrivateProfileInt(L"MyFive", name, def, L".\\config.ini");
}

class Config
{

};

