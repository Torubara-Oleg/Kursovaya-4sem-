#include <iostream>
#include "windows.h"
#include <fstream>

#pragma warning(disable : 4996)



int main()
{
    std::fstream file;

    std::string str;

    std::string fullStroka = "";

    HGLOBAL hgBuffer;
    char* chBuffer;

    std::string separator = "////";
    std::string path = "c:\\SaveFile\\BMP_КОТ.bmp";

    file.open("c:\\SaveFile\\file.txt",std::ios::in);

    while (!file.eof())
    {
        file >> str;
        fullStroka += str;
        fullStroka += " ";
    }

    
    fullStroka += separator;
    fullStroka += " ";
    fullStroka += path;
    

    OpenClipboard(NULL);

    EmptyClipboard();
    hgBuffer = GlobalAlloc(GMEM_DDESHARE, fullStroka.length() + 1);
    chBuffer = (char*)GlobalLock(hgBuffer);
    strcpy(chBuffer, fullStroka.c_str());
    GlobalUnlock(hgBuffer);
    SetClipboardData(CF_TEXT, hgBuffer);
    CloseClipboard();

    
    file.close();
}
