#pragma once
#include <functional>
#include <variant>
#include "fix_win32_compatibility.h"
#include "windows.h"
#include "varray.cpp"
#include "config.h"

struct run {
    std::string func;
    std::vector<std::variant<int, double>> args;
    bool sleep;

    void exec(varray &arr, std::string &name, std::unordered_map<std::string, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)>> &cache) {
        if (sleep) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    #ifdef DEBUG
        std::cout << "running " << func << "\n";
    #endif
        cache[func](arr, args, name);
        new (&arr.high) highlight(arr.len);
        if (sleep) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};
using funcsignature = void(__stdcall*)(varray &, std::vector<std::variant<int, double>>, std::string &);

run loadrunfromdll(std::string from, std::string str, std::unordered_map<std::string, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)>> &cache, bool sleep) {
    std::string name = str.substr(0, str.find_first_of("("));
    if (!cache.contains(name)) {
        std::string libname = from+"/"+name+".dll";
    #ifdef DEBUG
        std::cout << "loading lib from " << libname << "\n";
    #endif
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
        cache[name] = r;
    #ifdef DEBUG
        std::cout << "loaded\n";
    #endif
    } else {
    #ifdef DEBUG
        std::cout << "(" << name << " was cached)\n";
    #endif
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
    return run{name, args, sleep};
}

run loadrunfromfunc(std::string str, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)> func, std::unordered_map<std::string, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)>> &cache, std::vector<std::variant<int, double>> args, bool sleep) {
    std::string name = str.substr(0, str.find_first_of("("));
    if (!cache.contains(name)) {
    #ifdef DEBUG
        std::cout << "loading lambda " << name << "\n";
    #endif
        cache[name] = func;
    #ifdef DEBUG
        std::cout << "loaded\n";
    #endif
    } else {
    #ifdef DEBUG
        std::cout << "(" << name << " was cached)\n";
    #endif
    }
    std::string argstr = str.substr(str.find_first_of("(")+1);
    return run{name, args, sleep};
}