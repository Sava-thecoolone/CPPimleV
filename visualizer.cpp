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

std::string nextarg(std::string &str) {
    int pos = str.find_first_of(" ");
    std::string ret = str.substr(0, pos);
    str.erase(0, pos+1);
    return ret;
}

struct visualizer {
    std::vector<run> cases;
    renderer rend;
    varray arr;
    std::string curname;

    visualizer(int len, std::string rendname) : arr(len), rend(rendname) {curname = "Awaiting input...";}

    void loadcases(std::string name) {
        std::ifstream file(name);
        std::string line;
        while (std::getline(file, line)) {
            std::string action = nextarg(line);
            if (action == "shuf") cases.push_back(loadrun("shuffles", nextarg(line), false));
            else if (action == "sort") cases.push_back(loadrun("sorts", nextarg(line), true));
            else if (action == "delay") arr.high.delayMult = std::stod(nextarg(line));
            else if (action == "new") cases.push_back(run{[&] (varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {arr.resize(*std::get_if<int>(&args[0]));}, {std::stoi(nextarg(line))}, false});
        } 
    }

    void setupcasethread() {
        std::thread arraythread([&] {
            while (!IsMouseButtonDown(MOUSE_BUTTON_LEFT));
            for (auto &c : cases) {
                c.exec(arr, curname);
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
        rend.run.exec(arr, curname);
        UnloadAudioStream(stream);
        tsf_close(soundfont);
        CloseAudioDevice();
        CloseWindow();
    }
};