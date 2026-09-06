#pragma once
#include <variant>
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include "run.cpp"
#include "raylib.h"
#include "varray.cpp"
#include "renderer.cpp"
#include "sounds.cpp"

std::string nextarg(std::string &str, std::unordered_map<std::string, const char *> &suiteargs) {
    int pos = str.find_first_of(" ");
    std::string ret = str.substr(0, pos);
    str.erase(0, pos+1);
    for (auto &[from, to] : suiteargs) {
        int pos = 0;
        while ((pos = ret.find(from)) < ret.length()) {
            ret.replace(pos, from.length(), to);
        }
    }
    return ret;
}

struct visualizer {
    std::unordered_map<std::string, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)>> cache;
    std::vector<run> cases;
    std::unordered_map<char, const char *> args;
    std::unordered_map<std::string, const char *> suiteargs;
    renderer rend;
    varray arr;
    std::string curname;

    visualizer(int len, std::string rendname, std::unordered_map<char, const char *> a, std::unordered_map<std::string, const char *> s) : arr(len), rend(rendname, cache, a.contains('d')), args(a), suiteargs(s) {curname = "Awaiting input...";}

    void loadcases(std::string name) {
        std::ifstream file(name);
        std::string line;
        while (std::getline(file, line)) {
            std::string action = nextarg(line, suiteargs);
            if (action == "shuf") cases.push_back(loadrunfromdll("shuffles", nextarg(line, suiteargs), cache, false, args.contains('d')));
            else if (action == "sort") cases.push_back(loadrunfromdll("sorts", nextarg(line, suiteargs), cache, true, args.contains('d')));
            else if (action == "delay") arr.high.delayMult = std::stod(nextarg(line, suiteargs));
            else if (action == "new") cases.push_back(loadrunfromfunc("__new", [&] (varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {arr.resize(*std::get_if<int>(&args[0]));}, cache, {std::stoi(nextarg(line, suiteargs))}, false, args.contains('d')));
            else if (action == "default") {const char *var = nextarg(line, suiteargs).c_str(); if (!suiteargs.contains(var)) {suiteargs[var] = nextarg(line, suiteargs).c_str();}}
        } 
    }

    void setupcasethread() {
        std::thread arraythread([&] {
            while (!IsMouseButtonDown(MOUSE_BUTTON_LEFT));
            for (auto &c : cases) {
                c.exec(arr, curname, cache);
            }
            curname = "Done!";
        });
        arraythread.detach();
    }

    void start() {
        int curmon = GetCurrentMonitor(); 
        InitWindow(GetMonitorWidth(curmon), GetMonitorHeight(curmon), "cimplev");
        AudioStream stream = setupsound(arr);
        SetTargetFPS(GetMonitorRefreshRate(curmon));
        HideCursor();
        setupaudiothread(arr);
        setupcasethread();
        rend.run.exec(arr, curname, cache);
        UnloadAudioStream(stream);
        tsf_close(soundfont);
        CloseAudioDevice();
        CloseWindow();
    }
};