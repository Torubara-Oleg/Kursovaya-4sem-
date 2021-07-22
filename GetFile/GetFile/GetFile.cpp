#include <iostream>
#include "windows.h"
#include <fstream>

int main()
{
    std::ofstream file; 
    file.open("d:\\Desktop\\Poluchatel.txt", std::ios::out);

    OpenClipboard(NULL);
    std::string stroka;

    HANDLE hData = GetClipboardData(CF_TEXT);
    char* ch1Buffer = (char*)GlobalLock(hData);
    stroka = ch1Buffer;
    GlobalUnlock(hData);
    CloseClipboard();

    std::cout << stroka;
    
    file << stroka; 
    file.close();
}
