#include <iostream>
#include <csignal>
#include <stacktrace>
#include <cstdlib>
#include "visualizer.cpp"
#include "config.h"

void crash_handler(int signal) {
    std::cerr << "crash: " << signal << "\n";
    std::cerr << std::stacktrace::current() << '\n';
    std::exit(signal);
}

int main(int argc, char **argv) {
    if (argc <= 1) throw std::runtime_error("not enough arguments");
    std::string rendname = "bars";
    std::unordered_map<char, const char *> args;
    std::unordered_map<std::string, const char *> suiteargs;
    int suiteargc = 0;
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'r') rendname = argv[i]+2;
            else args[argv[i][1]] = argv[i]+2;
        } else {
            suiteargs["$"+std::to_string(suiteargc++)] = argv[i];
        }
    }
#ifdef DEBUG
        std::signal(SIGSEGV, crash_handler);
        std::signal(SIGABRT, crash_handler);
#endif
    visualizer vis(1024, rendname, args, suiteargs);
    vis.loadcases(argv[1]);
    vis.start();
    return 0;
}