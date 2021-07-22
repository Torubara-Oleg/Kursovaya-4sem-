#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <ctime>
#include "windows.h"


#define ITER 20

using namespace std;

mutex mtx, mtx_ed1, mtx_ed2;
condition_variable cv;

bool edit_1 = false, edit_2 = false, admin = false, _var = false;

int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };

bool Ret();
void Edit1();
void Edit2();
void Admin();

void Modify(int arr[], bool key)
{
    mtx.lock();
    if (edit_1)
    {
        //cout << "Поток №" << this_thread::get_id() << "  ";
        for (int i = 0; i < 6; ++i)
        {
            int monetka = rand() % 2;
            if (monetka == 1)
            {
                arr[i] = rand() % 10;
            }
        }
        /*
        for (int i = 0; i < 10; ++i)
        {
            cout << to_string(arr[i]) << " ";
        }
        cout << endl;
        */
    }
    if (edit_2)
    {
        //cout << "Поток №" << this_thread::get_id() << "  ";
        for (int i = 6; i < 10; ++i)
        {
            int monetka = rand() % 2;
            if (monetka == 1)
            {
                arr[i] = rand() % 10;
            }
        }
        /*
        for (int i = 0; i < 10; ++i)
        {
            cout << to_string(arr[i]) << " ";
        }
        cout << endl;
        */
    }
    if (admin)
    {
        std::string str;
        for (int i = 0; i < 10; ++i)
        {
            str += std::to_string(arr[i]);
            str += " ";
        }
        cout << str << endl;
        str = "";
    }
    mtx.unlock();
}

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");

    thread t1(Edit1);
    thread t2(Edit2);
    thread t3(Admin);

    t1.join();
    t2.join();
    t3.detach();

    system("Pause");
}

void Edit1()
{
    unique_lock<mutex> lock1(mtx_ed1);
    cv.wait(lock1, Ret);
    for (int i = 0; i < ITER; ++i)
    {
        edit_1 = true;
        Modify(arr, edit_1);
        edit_1 = false;
        Sleep(1000);
    }
}

void Edit2()
{
    unique_lock<mutex> lock2(mtx_ed2);
    cv.wait(lock2, Ret);
    for (int i = 0; i < ITER; ++i)
    {
        edit_2 = true;
        Modify(arr, edit_2);
        edit_2 = false;
        Sleep(1000);
    }
}

void Admin()
{
    int a;
    for (;;)
    {
        //cin >> a;
        a = GetKeyState(73); //Это чтобы читать кнопку i
        Sleep(100);          //Чтобы увеличить время считывания
        if (HIWORD(a) == 65535)
        {
            //cout << "Прошло!!)))))" << endl;
            _var = true;
            cv.notify_all();
            admin = true;
            Modify(arr, admin);
            admin = false;
            _var = false;
        }
        //else if (a == 0)
          //  exit(0);
        //else
        //{
        //  cout << HIWORD(a) << endl;
       // }

    }
}

bool Ret()
{
    return _var;
}
