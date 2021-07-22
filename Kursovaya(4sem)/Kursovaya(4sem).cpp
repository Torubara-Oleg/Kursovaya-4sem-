#include "framework.h"
#include "Kursovaya(4sem).h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <functional>
#include <condition_variable>



#define MAX_LOADSTRING 100
#define ITER 5


const int MEMCOUNT = 5000000000;
void* VirtualMemoryAdress;
int offsetY = 0;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HINSTANCE newInst;                                

HPEN White_pen = CreatePen(PS_SOLID, 1, RGB(121,124,1));
HBRUSH newBrush = CreateSolidBrush(RGB(130, 124, 121));

HANDLE thread_1;
HANDLE thread_2;
HANDLE thread_3;

WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hWnd;//Главное окно

HWND Button_Info;
HWND Button_RegInfo;
HWND Button_Process;
HWND Button_Memory;
HWND Button_Thread;

HWND Info_Course;
HWND Reg_Info;
HWND Process_Info;
HWND Memory_Info;
//HWND Thread_Info;

RECT Crect;

POINT A; //Конечная точка отрезка первого графика
POINT B; //Начальная точка отрезка первого графика
POINT C; //Начальная точка отрезка второго графика
POINT D; //Конечная точка отрезка второго графика

std::vector<int> vY;
std::vector<int> SecGr;

HBITMAP myFace; //handle битмапа моего лица

HDC hdc;
HDC memDC;
HBITMAP memBTM;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CALLBACK Timer_Proc(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime); //Процедура таймера

LRESULT CALLBACK Information_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void GraphTemplate(HDC hdc);

void AuthorClass();
void RegClass();

void Graph_Info(HDC hdc);
void Task5();
void MemoryAlloc(HDC hdc);
void ProcessResult(HDC hdc);
void Extension(HDC hdc);
void Processing();
void handleProgramName(DWORD index, TCHAR* _name, std::vector<std::wstring>& AV);
void ExtensionOutput(std::vector<std::wstring>& AV,HDC hdc);
void Read_Values(HDC hdc);
void Info_Disk(HDC hdc);
void DrawFirstGraph(HDC hdc);
void DrawSecondGraph(HDC hdc);
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KURSOVAYA4SEM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KURSOVAYA4SEM));
    // SetTimer(child, 1, 4000, Timer_Proc);
    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }



    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KURSOVAYA4SEM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(246, 173, 66));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KURSOVAYA4SEM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void AuthorClass()
{
    WNDCLASS Author = {};

    Author.lpfnWndProc = Information_Proc;
    Author.hInstance = hInst;
    Author.hbrBackground = CreateSolidBrush(RGB(246, 173, 66));
    Author.lpszClassName = L"Info";
    Author.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_KURSOVAYA4SEM));
    RegisterClass(&Author);
}

void RegClass()
{
    WNDCLASS regedit = {};

    regedit.lpfnWndProc = Information_Proc;
    regedit.hInstance = newInst;
    regedit.hbrBackground = CreateSolidBrush(RGB(246, 173, 66));
    regedit.lpszClassName = L"Register";
    regedit.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_KURSOVAYA4SEM));
    RegisterClass(&regedit);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
   
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
      CW_USEDEFAULT, 0, 1000, 800, nullptr, nullptr, hInstance, nullptr);

   SetTimer(hWnd, 1, 500, Timer_Proc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case BN_CLICKED:
            {
                if ((HWND)lParam == Button_Info)
                {
                    //Инфомационное окно
                    myFace = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
                    Info_Course = CreateWindow(L"Info", L"Задание курсовой работы и информация об Авторе", WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, 1210, 710, hWnd, NULL, hInst, NULL);
                }
                if ((HWND)lParam == Button_RegInfo)
                {
                    //InitInstance(newInst, SW_NORMAL);
                    Reg_Info = CreateWindow(L"Info", L"Чтение данных о системе", WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, 800, 550, ::hWnd, NULL, newInst, NULL);
                }
                if ((HWND)lParam == Button_Process)
                {
                    Process_Info = CreateWindow(L"Info", L"Запись/чтение из буффера обмена", WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, 450, 300, hWnd, NULL, hInst, NULL);
                }
                if ((HWND)lParam == Button_Memory)
                {
                    Memory_Info = CreateWindow(L"Info", L"Выделение памяти", WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, 400, 200, hWnd, NULL, hInst, NULL);
                }
                if ((HWND)lParam == Button_Thread)
                {
                    Task5();
                    //Thread_Info = CreateWindow(L"Info", L"Потоки", WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, 1210, 710, hWnd, NULL, hInst, NULL);
                }
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        AuthorClass();
        RegClass();

        GetClientRect(hWnd, &Crect);
        B.x = Crect.left;
        B.y = Crect.bottom / 2 - 50;
        A.x = Crect.left;
        C.x = Crect.left;
        C.y = Crect.bottom - 50;
        D.x = Crect.left;

        //Кнопка
        Button_Info = CreateWindow(L"Button", L"О проекте", WS_VISIBLE | WS_CHILD, Crect.right - 250, Crect.top+50, 200, 60, hWnd, NULL, hInst, 0);

        Button_RegInfo = CreateWindow(L"Button", L"Чтение реестра", WS_VISIBLE | WS_CHILD, Crect.right - 250, Crect.top + 150, 200, 60, hWnd, NULL, hInst, 0);

        Button_Process = CreateWindow(L"Button", L"Процессинг", WS_VISIBLE | WS_CHILD, Crect.right - 250, Crect.top + 250, 200, 60, hWnd, NULL, hInst, 0);

        Button_Memory = CreateWindow(L"Button", L"Память", WS_VISIBLE | WS_CHILD, Crect.right - 250, Crect.top + 450, 200, 60, hWnd, NULL, hInst, 0);

        Button_Thread = CreateWindow(L"Button", L"Потоки", WS_VISIBLE | WS_CHILD, Crect.right - 250, Crect.top + 550, 200, 60, hWnd, NULL, hInst, 0);

        break;
    }
    case WM_TIMER:
    {

    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
       
        GraphTemplate(hdc);
        
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CALLBACK Timer_Proc(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
{
    hdc = GetDC(hWnd);

    memDC = CreateCompatibleDC(hdc);
    memBTM = CreateCompatibleBitmap(hdc, 800, 400);

    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH newBrush = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldBTM = SelectObject(memDC, memBTM);

    HGDIOBJ oldPen = SelectObject(memDC, newPen);
    HGDIOBJ oldBrush = SelectObject(memDC, newBrush);

    //Rectangle(memDC, Crect.left, Crect.top, Crect.right,Crect.bottom/2);

    SelectObject(memDC, oldPen);

    //Ellipse(memDC, Main.left + 5, Main.top + 5, Main.right - 5, Main.bottom - 5);

    Graph_Info(hdc);

    DrawFirstGraph(hdc);
    //////////////////////////
    DrawSecondGraph(hdc);

    //BitBlt(hdc, Crect.left, Crect.top, Crect.right, Crect.bottom / 2, memDC, Crect.left, Crect.top, SRCCOPY);

    SelectObject(hdc, oldBTM);
    SelectObject(hdc, oldBrush);


    DeleteObject(newPen);
    DeleteObject(newBrush);
    DeleteObject(memBTM);
    DeleteDC(memDC);

    ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK Information_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (hWnd == Info_Course)
        {
            std::wstringstream Info;
            RECT secondW;
            RECT InfoBorders;
            GetClientRect(hWnd, &secondW);
            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH newBrush = CreatePatternBrush(myFace);
            HGDIOBJ oldBrush = SelectObject(hdc, newBrush);
            int offset = secondW.bottom - 650;

            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            Rectangle(hdc, secondW.right - 500, secondW.top + 10, secondW.right - 10, secondW.bottom - offset);

            InfoBorders.right = 650;
            InfoBorders.bottom = 650;
            InfoBorders.left = 10;
            InfoBorders.top = 10;

            //<< L"Весь тот ужас что вы лицезреете сейчас сотворил вот этот человек ------------------------------------------->" << std::endl
            Info << L"                           Курсова робота " << std::endl
                << L"                студента групи АМ - 192 Торубари Олега" << std::endl
                << L"                             Частина перша:" << std::endl
                << L"1. Створити віконне додаток на базі WinApi" << std::endl
                << L"2. Встановити таймер, що спрацьовує кожні N секунд. Значення N визначається варіантом завдання" << std::endl
                << L"3. Показати два графіка. На кожному з них можна переглянути останні номери" << std::endl
                << L"M значень. Значення зчитуються щодо подій таймера. Вид графіка і його вміст" << std::endl
                << L"визначаються варіантом завдання." << std::endl
                << L"4. Створити діалогове вікно, в якому відображається завдання і інформація про" << std::endl
                << L"автора з фотографіє" << std::endl
                << L"5. Прочитати інформацію з реєстру Windows, що відноситься до самостійно " << std::endl
                << L"обраним апаратному та програмному забезпеченню, а також до налаштувань, що" << std::endl
                << L"належать до комп'ютера в цілому і до окремому користувачеві" << std::endl
                << L"6. Прочитати інформацію з реєстру Windows, визначальну асоціації типу" << std::endl
                << L"документа. Тип документа визначається варіантом завдання" << std::endl
                << L"                               Частина друга" << std::endl
                << L"Створити застосунок, що складається з двох процесів:" << std::endl
                << L"- перший процес записує текстовий файл і растровий малюнок в буфер обміну" << std::endl
                << L"- другий процес зчитує інформацію з буфера обміну і відображає у вікні процесу." << std::endl
                << L"Текстовий файл і файл з растровим малюнком взяти з попередніх завдань свого варіанту. " << std::endl
                << L"                               Частина третя" << std::endl
                << L"Розробити програму, що надає користувачеві можливість управляти" << std::endl
                << L"пам'яттю в Windows, отримувати системну інформацію про стан фізичної і " << std::endl
                << L"віртуальної пам'яті, а також керувати виділенням і звільненням віртуальної пам'яті" << std::endl
                << L"В програмі передбачити використання різних видів пам'яті" << std::endl
                << L"Статично розподіляється." << std::endl
                << L" Регіонів віртуальної пам'яті." << std::endl
                << L"                               Частина четверта" << std::endl
                << L"У буфері є N записів типу Type. До цих записів на читання і на запис " << std::endl
                << L"звертаються m потоків. Кожен з потоків може змінити вміст записe. Існує потік I, " << std::endl
                << L"який відображає на моніторі змінені записи. Кожну запис може змінити тільки I-" << std::endl
                << L"тий потік. Коли потік I відобразить останню змінену запис, інші потоки можуть " << std::endl
                << L"продовжувати модифікувати записи. Потік I запускається після натискання клавіш " << std::endl
                << L"ALT + I" << std::endl;

            DrawTextW(hdc, Info.str().c_str(), Info.str().length(), &InfoBorders, DT_WORDBREAK);

            //SelectObject(hdc, oldPen);
            //SelectObject(hdc, oldBrush);

            EndPaint(hWnd, &ps);
        }

        if (hWnd == Reg_Info)
        {
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            
            Info_Disk(hdc);
            Read_Values(hdc);
           
            Extension(hdc);

            EndPaint(hWnd, &ps);
        }

        if (hWnd == Process_Info)
        {
            //SetBkColor(hdc, RGB(34, 212, 12));
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            Processing();
            
            ProcessResult(hdc);
        }

        if (hWnd == Memory_Info)
        {
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            MemoryAlloc(hdc);
            //EnableWindow(::hWnd, TRUE);
        }

        break;
    }
    case WM_DESTROY:
        EnableWindow(::hWnd, TRUE);
        DestroyWindow(hWnd);
        //if (hWnd == Memory_Info)
           // ShowWindow(::hWnd, SW_NORMAL);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
    return LineTo(hdc, x2, y2);
}
//ГРАФИКИ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void DrawFirstGraph(HDC hdc)
{
    //Расчет виртуальной памяти
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile / 1024 / 1024 / 1024;
    DWORDLONG virtualMemUsed = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024 / 1024 / 1024;
    
    int Deleniya_height = ((Crect.bottom / 2) - 50) / (totalVirtualMem);

    A.y = (Crect.bottom / 2 - 50) - ((virtualMemUsed) * Deleniya_height);
    A.x += (Crect.right-400)/40;

    if (A.x > Crect.right-400)
    {
        //HPEN NewPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        //HGDIOBJ oldPen = SelectObject(hdc, NewPen);

        Rectangle(hdc, Crect.left, Crect.top, Crect.right - 400, Crect.bottom / 2 - 50);
        // ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
        B.x = Crect.left;
        for (int i = 0; i < 19; ++i)
        {
            Line(hdc, B.x, vY[i], B.x + (Crect.right - 400) / 40, vY[i + 1]);
            B.x += (Crect.right - 400) / 40;
        }
        A.x = B.x + ((Crect.right - 400) / 40);
        B.y = vY[19];
        vY.clear();
        //Line(hdc, B.x, B.y, A.x, (Crect.bottom / 2 - 50) - A.y);
    }

    //arr_y[counter] = A.y;
    vY.push_back(A.y);

    Line(hdc, B.x, B.y, A.x, A.y);

    B.x = A.x;
    B.y = A.y;
}

void DrawSecondGraph(HDC hdc)
{
    //Расчет виртуальной памяти
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    __int64 FreeBytesAvailable, TotalNumberOfBytes, TotalNumberofFreeBytes;
    bool success;
    success = GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberofFreeBytes);
    DWORDLONG totalSpace = TotalNumberOfBytes / 1024 / 1024 / 1024;
    DWORDLONG FreeSpace = TotalNumberofFreeBytes / 1024 / 1024 / 1024;

    int Deleniya_height = ((Crect.bottom - 50) - (Crect.bottom / 2)) / totalSpace;

    D.y = (Crect.bottom - 50) - ((FreeSpace)*Deleniya_height);
    D.x += (Crect.right - 400) / 40;

    if (D.x > Crect.right - 400)
    {
        //HPEN NewPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        //HGDIOBJ oldPen = SelectObject(hdc, NewPen);

        Rectangle(hdc, Crect.left, (Crect.top + Crect.bottom / 2), Crect.right - 400, Crect.bottom - 50);
        // ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
        C.x = Crect.left;
        for (int i = 0; i < 19; ++i)
        {
            Line(hdc, C.x, SecGr[i], C.x + (Crect.right - 400) / 40, SecGr[i + 1]);
            C.x += (Crect.right - 400) / 40;
        }
        D.x = C.x + ((Crect.right - 400) / 40);
        C.y = SecGr[19];
        SecGr.clear();
        //Line(hdc, B.x, B.y, A.x, (Crect.bottom / 2 - 50) - A.y);
    }

    //arr_y[counter] = A.y;
    SecGr.push_back(D.y);

    Line(hdc, C.x, C.y, D.x, D.y);

    C.x = D.x;
    C.y = D.y;
}

void GraphTemplate(HDC hdc)
{
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    __int64 FreeBytesAvailable, TotalNumberOfBytes, TotalNumberofFreeBytes;
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile / 1024 / 1024 / 1024;
    DWORDLONG virtualMemUsed = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024 / 1024 / 1024;

    bool success;
    success = GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberofFreeBytes);
    DWORDLONG totalSpace = TotalNumberOfBytes / 1024 / 1024 / 1024;
    DWORDLONG FreeSpace = TotalNumberofFreeBytes / 1024 / 1024 / 1024;

    std::wstring info = std::to_wstring(totalVirtualMem);
    info += L" GB";
    TextOut(hdc, Crect.right - 400, 0, info.c_str(), info.length());

    TextOut(hdc, Crect.right - 400, 300, L"0 GB", 4);
    info = std::to_wstring(totalSpace);
    info += L" GB";
    TextOut(hdc, Crect.right - 400, 370, info.c_str(), info.length());
    TextOut(hdc, Crect.right - 400, 670, L"0 GB", 4);

    success = GlobalMemoryStatusEx(&memInfo);

    success = GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberofFreeBytes);

    info = L"Всего памяти: ";
    info += std::to_wstring((double)TotalNumberOfBytes / 1024 / 1024 / 1024);
    info += L" Gb";
    TextOut(hdc, (Crect.right / 2 - info.length() / 2) - 100, 720, info.c_str(), info.length());

    info = L"Свободно памяти: ";
    info += std::to_wstring((double)TotalNumberofFreeBytes / 1024 / 1024 / 1024);
    info += L" Gb";
    TextOut(hdc, 0, 720, info.c_str(), info.length());
    

    info = L"Всего памяти: ";
    info += std::to_wstring(memInfo.ullTotalPageFile / 1024 / 1024);
    info += L" MB";
    TextOut(hdc, (Crect.right / 2 - info.length() / 2) - 80, 350, info.c_str(), info.length());
    info = L"Свободно памяти: ";
    info += std::to_wstring((memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024 / 1024);
    info += L" MB";
    TextOut(hdc, 0, 350, info.c_str(), info.length());

    TextOut(hdc, Crect.right / 2 - 270, 690, L"Жесткий диск С:", 14);

    TextOut(hdc, Crect.right / 2 - 300, 320, L"Использование виртуальной памяти:", 34);

    A.x = Crect.left;
    Rectangle(hdc, Crect.left, Crect.top, Crect.right - 400, Crect.bottom / 2 - 50);
    Rectangle(hdc, Crect.left, (Crect.top + Crect.bottom / 2), Crect.right - 400, Crect.bottom - 50);
    Line(hdc, Crect.left, Crect.bottom / 2, Crect.right, Crect.bottom / 2);
}

void Info_Disk(HDC hdc)
{
    int offsetY = 0;
    RECT borders;
    borders.right = 500;
    borders.bottom = 500;
    bool success;
    std::wstringstream Info;
    MEMORYSTATUSEX mem;
    __int64 FreeBytesAvailable, TotalNumberOfBytes, TotalNumberofFreeBytes;
    mem.dwLength = sizeof(mem);
    success = GlobalMemoryStatusEx(&mem);

    std::vector<std::string> disk;

    success = GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberofFreeBytes);

    //HARDDRIVE 'C'
    if (success)
    {
        disk.push_back("Состояние диска:");
        disk.push_back("Всего свободно байт: " + std::to_string(TotalNumberofFreeBytes));
        disk.push_back("Всего байт: " + std::to_string(TotalNumberOfBytes));

        disk.push_back("Всего свободно MB: " + std::to_string((double)TotalNumberofFreeBytes / 1024 / 1024));

        disk.push_back("Всего MB: " + std::to_string((double)TotalNumberOfBytes / 1024 / 1024));

        disk.push_back("Всего свободно GB: " + std::to_string((double)TotalNumberofFreeBytes / 1024 / 1024 / 1024));

        disk.push_back("Всего GB: " + std::to_string((double)TotalNumberOfBytes / 1024 / 1024 / 1024));
    }
    else
        disk.push_back(" Not Present (GetDiskFreeSpace) ");

    //RAM
    if (success)
    {
        disk.push_back("RAM использовано: " + std::to_string(mem.dwMemoryLoad) + " %");
        disk.push_back("Всего Ram: " + std::to_string((double)mem.ullTotalPhys / 1024 / 1024 / 1024));
        disk.push_back("Свободно RAM: " + std::to_string((double)mem.ullAvailPhys / 1024 / 1024 / 1024));
    }
    else
        disk.push_back("ERROR Couldn't get memory info\n");

    //OUTPUT
    DrawText(hdc, Info.str().c_str(), Info.str().length(), &borders, DT_WORDBREAK);
    for (auto el : disk)
    {
        TextOutA(hdc, 0, offsetY, el.c_str(), el.length());
        offsetY += 20;
    }
}

void Read_Values(HDC hdc)
{
    const TCHAR* path = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"; //Путь к реестру
    HKEY key;                 //КЛЮЧ
    DWORD status;             //статус выполнения операции
    DWORD valueCount;         //буффер кол-ва значений
    DWORD valueType;          //Определяет тип данных в записи реестра
    DWORD maxValueNameLength; //Хранит максимальную длинну имени записи
    DWORD maxValueDataLength; //Хранит максимальный размер данных хранимых в записи
    DWORD nameCharBuffer;     //Буфер куда передается длина имени
    DWORD valueCharBuffer;    //Буфер куда передается размер данных
    TCHAR* valueName;         //Определяет имя записи 
    TCHAR* valueData;         //Определяет значение в записи 

    std::vector<std::wstring> reg;
    int offsetX = 0, offsetY = 250;


    status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &key);

    if (status == ERROR_SUCCESS)
    {
        status = RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &valueCount, &maxValueNameLength, &maxValueDataLength, NULL, NULL);

        if (status == ERROR_SUCCESS)
        {
            reg.push_back(L"Найдено " + std::to_wstring(valueCount) + L" значения по пути: \"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\:");

            maxValueNameLength += 1;
            maxValueDataLength += 1;
            valueName = (TCHAR*)malloc((maxValueNameLength) * sizeof(TCHAR));
            valueData = (TCHAR*)malloc((maxValueDataLength) * sizeof(TCHAR));

            for (UINT i = 0; i < valueCount; ++i)
            {
                nameCharBuffer = maxValueNameLength;
                valueCharBuffer = maxValueDataLength;

                status = RegEnumValue(key, i, valueName, &nameCharBuffer, NULL, &valueType, (LPBYTE)valueData, &valueCharBuffer);

                if (status == ERROR_SUCCESS)
                {
                    reg.push_back( std::to_wstring(i + 1) +  L".    " + valueName + L"    -    ");

                    if (valueType == REG_BINARY)
                    {
                        for (int j = 0; j < valueCharBuffer; j++)
                        {
                            //wcout << hex << setfill(L'0') << std::setw(2) << (BYTE)valueData[j];

                            if (j % 16 != 15)
                            {
                                //wcout << " ";
                            }
                            else
                            {
                                //wcout << "\n";
                            }
                        }
                    }
                    else if (valueType == REG_DWORD)
                    {
                        //wcout << (DWORD)*valueData << "\n";
                    }
                    else
                    {
                        reg[i+1] += valueData;
                    }
                }

            }
        }
        else
        {
            //cout << "ERROR " << status << ". Couldn't get data from registry key.\n\n";
            return;
        }

    }
    else
    {
        //cout << "ERROR " << status << ". Couldn't open registry key.\n\n";
        return;
    }

    for (auto el : reg)
    {
        TextOutW(hdc, offsetX, offsetY, el.c_str(), el.length());
        offsetY += 20;
    }


    free(valueName);
    free(valueData);
    RegCloseKey(key);
}

void Extension(HDC hdc)
{
    std::wstring fileExtension = L".avi";
    HKEY key;
    const TCHAR* path;
    DWORD status;
    DWORD valueCount;
    DWORD maxValueNameLength;
    DWORD charBuffer;
    TCHAR* valueName;

    std::vector<std::wstring> AV;

    fileExtension += L"\\OpenWithProgids";
    path = fileExtension.c_str();
    status = RegOpenKeyEx(HKEY_CLASSES_ROOT, path, 0, KEY_READ, &key);

    if (status != ERROR_SUCCESS)
    {
        AV.push_back(L"ERROR " + std::to_wstring(status) + L". Couldn't open registry key.");
        return;
    }

    status = RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &valueCount, &maxValueNameLength, NULL, NULL, NULL);

    if (status != ERROR_SUCCESS)
    {
        AV.push_back(L"ERROR " + std::to_wstring(status) + L". Couldn't get data from registry key.");
        return;
    }

    AV.push_back(L" > Найдено " + std::to_wstring(valueCount) + L" приложений которые могу открыть расширение `.avi`");

    maxValueNameLength += 1;
    valueName = (TCHAR*)malloc((maxValueNameLength) * sizeof(TCHAR));

    for (DWORD i = 0; i < valueCount; i++)
    {
        charBuffer = maxValueNameLength;
        status = RegEnumValue(key, i, valueName, &charBuffer, NULL, NULL, NULL, NULL);

        if (status == ERROR_SUCCESS)
        {
            handleProgramName(i + 1, valueName, AV);
        }
        else
        {
            AV.push_back(L"ERROR " + std::to_wstring(status) + L". Couldn't get value name.");
        }
    }

    ExtensionOutput(AV, hdc);

    free(valueName);
    RegCloseKey(key);

}

void handleProgramName(DWORD index, TCHAR* _name,std::vector<std::wstring> &AV)
{
    DWORD status;
    TCHAR getResult[128];
    DWORD buffer;
    std::wstring strPathName = _name;
    std::wstring strName;
    const TCHAR* path;


    // Equal
    if (strPathName.compare(0, 4, L"AppX") == 0)
    {
        AV.push_back(L" | " + std::to_wstring(index) + L". " + strPathName + L"    -    Путь не найден. ПРИЧИНА: Родное приложение для Windows.");
        return;
    }

    strName = strPathName.substr(0, strPathName.find(L'.'));
    strPathName += L"\\shell\\open\\command";
    path = strPathName.c_str();
    buffer = 128 * sizeof(TCHAR);
    status = RegGetValue(HKEY_CLASSES_ROOT, path, NULL, RRF_RT_REG_SZ, NULL, (PVOID)&getResult, &buffer);

    if (status != ERROR_SUCCESS)
    {
        AV.push_back(L"ERROR " + std::to_wstring(status) + L" Couldn't get value.");
    }

    AV.push_back(L" | " + std::to_wstring(index) + L". " + strName + L"   -     " +  getResult);
}

void ExtensionOutput(std::vector<std::wstring>& AV,HDC hdc)
{
    int offsetX = 0, offsetY = 400;
    for (auto el : AV)
    {
        TextOut(hdc, offsetX, offsetY, el.c_str(), el.length());
        offsetY += 20;
    }
}

void Processing()
{
    bool bp1;
    bool bp;
    STARTUPINFO startInf;
    PROCESS_INFORMATION procInf;

    TCHAR ProcessPath[] = LR"(C:\SaveFile\Debug\SaveFile.exe)";

    ZeroMemory(&startInf, sizeof(startInf));
    ZeroMemory(&procInf, sizeof(procInf));
    startInf.cb = sizeof(startInf);

    if (!(bp1 = CreateProcess(NULL, ProcessPath, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startInf, &procInf)))
    {
        MessageBox(NULL, L"Не получилось создать записывающий процесс", L"Ошибка", 0);
    }
    
    if (bp1)
    {
        WaitForSingleObject(procInf.hProcess, INFINITE);
    }

    
    CloseHandle(procInf.hProcess);
    CloseHandle(procInf.hThread);


    STARTUPINFO start;
    PROCESS_INFORMATION proc;
    TCHAR ProcessPath_1[] = LR"(C:\GetFile\Debug\GetFile.exe)";

    ZeroMemory(&start, sizeof(start));
    ZeroMemory(&proc, sizeof(proc));
    start.cb = sizeof(start);

    if (!(bp = CreateProcess(NULL, ProcessPath_1, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &start, &proc)))
    {
        MessageBox(NULL, L"Не получилось создать процесс получатель", L"Ошибка", 0);
    }

    if (bp)
    {
        WaitForSingleObject(proc.hProcess, INFINITE);
    }

    CloseHandle(proc.hProcess);
    CloseHandle(proc.hThread);
}

void ProcessResult(HDC hdc)
{
    std::fstream file;
    std::string line;
    std::string finalLine;

    file.open("C:\\Desktop\\Poluchatel.txt", std::ios::in);

    while (line != "////")
    {
        file >> line;
        if (line == "////")
            break;
        finalLine += line;
        finalLine += " ";
    }

    file >> line;

    HBRUSH newBrush = CreatePatternBrush((HBITMAP)LoadImageA(hInst, line.c_str(), IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE));
    HGDIOBJ oldBrush = SelectObject(hdc, newBrush);

    Rectangle(hdc, 0, 0, 209, 241);


    TextOut(hdc, 250, 50, L"<--------- Котик", 16);
    TextOutA(hdc, 250, 150, finalLine.c_str(), finalLine.length());


    SelectObject(hdc, oldBrush);
}

void MemoryAlloc(HDC hdc)
{
    RECT border;
    std::wstringstream memory;
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile / 1024 / 1024;
    DWORDLONG virtualMemUsed = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024 / 1024;

    int total = totalVirtualMem;
    int used = virtualMemUsed;
    memory << L"Всего виртуальной памяти " << std::to_wstring(total) << L" MB" << std::endl
        << L"Использовано " << std::to_wstring(used) << L" MB" << std::endl;
    
    DrawTextW(hdc, memory.str().c_str(), memory.str().length(), &Crect, DT_WORDBREAK);

    /*
    PAGE_READWRITE - Чтение и запись.
    PAGE_READONLY - Только чтение.
    PAGE_EXECUTE - Только исполнение программного кода.
    PAGE_EXECUTE_READ - Исполнение и чтение.
    PAGE_EXECUTE_READWRITE - Исполнение, чтение и запись.
    PAGE_NOACCESS - Запрещен любой вид доступа.
    PAGE_GUARD - Сигнализация доступа к странице. Это значение можно использовать вместе с любыми другими, кроме PAGE_NOACCESS.
    PAGE_NOCACHE - Отмена кэширования для страницы памяти. Используется драйверами устройств. Это значение можно использовать вместе с любыми другими, кроме PAGE_NOACCESS.
    */

    ///int mem = 500000;
    //std::wstring size = L"500000";
    
    //VirtualMemoryAdress = VirtualAlloc(NULL, _wtoi(size.c_str()), MEM_COMMIT, PAGE_NOACCESS);

    VirtualMemoryAdress = VirtualAlloc(NULL, MEMCOUNT, MEM_COMMIT, PAGE_READWRITE);
    if (VirtualMemoryAdress != NULL)
    {
        MessageBox(NULL, L"OK Alloc ", L"OK", MB_OK);
    }
    else
        MessageBox(NULL, L"error alloc ", L"OK", MB_OK);


    GlobalMemoryStatusEx(&memInfo);
    totalVirtualMem = memInfo.ullTotalPageFile / 1024 / 1024;
    virtualMemUsed = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024 / 1024;

    total = totalVirtualMem;
    used = virtualMemUsed;
    memory << L"Всего виртуальной памяти " << std::to_wstring(total) << L" MB" << std::endl
        << L"Использовано " << std::to_wstring(used) << L" MB" << std::endl;

    DrawTextW(hdc, memory.str().c_str(), memory.str().length(), &Crect, DT_WORDBREAK);

    if (VirtualFree(VirtualMemoryAdress, 0, MEM_RELEASE))
        MessageBox(NULL, L"OK free ", L"OK", MB_OK);
    else
        MessageBox(NULL, L"not free ", L"OK", MB_OK);
}

void Task5()
{
    bool bp;
    STARTUPINFO startInf;
    PROCESS_INFORMATION procInf;

    TCHAR ProcessPath[] = LR"(C:\threads\Debug\threads.exe)";

    ZeroMemory(&startInf, sizeof(startInf));
    ZeroMemory(&procInf, sizeof(procInf));
    startInf.cb = sizeof(startInf);

    if (!(bp = CreateProcess(NULL, ProcessPath, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInf, &procInf)))
    {
        MessageBox(NULL, L"Не получилось создать процесс", L"Ошибка", 0);
    }

    if (bp)
    {
        WaitForSingleObject(procInf.hProcess, INFINITE);
    }


    CloseHandle(procInf.hProcess);
    CloseHandle(procInf.hThread);
}

void Graph_Info(HDC hdc)
{
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    MEMORYSTATUSEX mem;
    __int64 FreeBytesAvailable, TotalNumberOfBytes, TotalNumberofFreeBytes;
    mem.dwLength = sizeof(mem);
    bool success = GlobalMemoryStatusEx(&mem);

    std::wstring info;

    success = GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberofFreeBytes);

    HBRUSH newBrush = CreateSolidBrush(RGB(246, 173, 66));
    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(246, 173, 66));

    HGDIOBJ oldPen = SelectObject(hdc, newPen);
    HGDIOBJ oldBrush = SelectObject(hdc, newBrush);

    Rectangle(hdc, 0, 740, 350, 720);

    info = L"Всего памяти: ";
    info += std::to_wstring((double)TotalNumberOfBytes / 1024 / 1024 / 1024);
    info += L" Gb";
    TextOut(hdc, (Crect.right / 2 - info.length() / 2)-100, 720, info.c_str(),info.length());

    info = L"Свободно памяти: ";
    info += std::to_wstring((double)TotalNumberofFreeBytes / 1024 / 1024 / 1024);
    info += L" Gb";

    //TextOutA(hdc, 0, 700, "", 1);
    TextOut(hdc, 0, 720, info.c_str(), info.length());


    Rectangle(hdc, 0, 350, 600,370);
    info = L"Всего памяти: ";
    info += std::to_wstring(mem.ullTotalPageFile / 1024 / 1024);
    info += L" MB";
    TextOut(hdc, (Crect.right / 2 - info.length() / 2) - 80, 350, info.c_str(), info.length());
    info = L"Свободно памяти: ";
    info += std::to_wstring((mem.ullTotalPageFile - mem.ullAvailPageFile) / 1024 / 1024);
    info += L" MB";
    TextOut(hdc, 0, 350, info.c_str(), info.length());

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
}