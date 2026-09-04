#include <iostream>
#include "visualizer.cpp"

int main(int argc, char **argv) {
    if (argc <= 1) throw std::runtime_error("not enough arguments");
    visualizer vis(1024, "bars");
    vis.loadcases(argv[1]);
    vis.start();
    return 0;
}