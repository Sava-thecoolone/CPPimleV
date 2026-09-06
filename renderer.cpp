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

    renderer(std::string name, std::unordered_map<std::string, std::function<void(varray &, std::vector<std::variant<int, double>>, std::string &)>> &cache, bool debug) {
        this->name = name;
        run = loadrunfromdll("renderers", name, cache, false, debug);
    }
};