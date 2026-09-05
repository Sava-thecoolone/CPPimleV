#include <variant>
#include <iostream>
#include <random>
#include "../varray.cpp"

bool isSorted(varray &arr) {
    for (int i = 0; i < arr.len-1; i++) {
        if (arr[i] > arr[i+1]) return false;
    }
    return true;
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Bogo sort";
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, arr.len-1);
    while (true) {
        for (int i = 0; i < arr.len; i++) {
            std::swap(arr[i], arr[dist(rng)]);
        }
        if (isSorted(arr)) break;
    }
}