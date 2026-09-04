#pragma once
#include <functional>
#include <variant>
#include "fix_win32_compatibility.h"
#include "windows.h"
#include "varray.cpp"

// typedef std::function<void(varray &, std::vector<std::variant<int, double>>)> run;
struct run {
    std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)> func;
    std::vector<std::variant<int, double>> args;
    bool sleep;

    void exec(varray &arr, std::string &name) {
        func(arr, args, name);
        arr.high.cur = -1;
        if (sleep) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};
using funcsignature = void(__stdcall*)(varray &, std::vector<std::variant<int, double>>, std::string &);

run loadrun(std::string from, std::string str, bool sleep) {
    std::string name = str.substr(0, str.find_first_of("("));
    std::string libname = from+"/"+name+".dll";
    std::cout << "loading run from " << libname << "\n";
    HINSTANCE hLib;
#ifndef __INTELLISENSE__ // shut up
    hLib = LoadLibrary(libname.c_str());
#endif
    if (!hLib) {
        throw std::runtime_error("no library");
    }
    funcsignature r = reinterpret_cast<funcsignature>(GetProcAddress(hLib, "run"));
    if (!r) {
        FreeLibrary(hLib);
        throw std::runtime_error("no run function");
    }
    std::string argstr = str.substr(str.find_first_of("(")+1);
    std::vector<std::variant<int, double>> args;
    int pos = argstr.find_first_of(",)");
    while (pos < argstr.length()) {
        std::string arg = argstr.substr(0, pos);
        if (arg.find_first_of(".") != -1) args.push_back(std::stod(argstr.substr(0, pos)));
        else args.push_back(std::stoi(argstr.substr(0, pos)));
        argstr.erase(0, pos+1);
        pos = argstr.find_first_of(",)");
    }
    return run{r, args, sleep};
}