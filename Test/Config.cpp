#include "Config.h"
#include<string>
#include<Windows.h>
#include <direct.h>


/// <summary>
/// ��ȡini�����ļ�
/// </summary>
/// <param name="name">�������ƣ�Ӣ�ģ�</param>
/// <param name="def">Ĭ��ֵ(�����ò�����ʱ��Ч)</param>
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

