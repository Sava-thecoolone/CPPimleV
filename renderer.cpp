#include <variant>
#include <thread>
#include <iostream>
#include "varray.cpp"
#include "run.cpp"

struct renderer {
    std::string name;
    run run;
    HINSTANCE hLib;

    renderer() {}

    renderer(std::string name) {
        this->name = name;
        run = loadrun("renderers", name, false);
    }
};