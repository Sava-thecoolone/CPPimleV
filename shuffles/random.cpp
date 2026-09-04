#include <variant>
#include <iostream>
#include <random>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Random";
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, arr.len-1);
    for (int i = 0; i < arr.len; i++) {
        std::swap(arr[i], arr[dist(rng)]);
    }
}