#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Reversed";
    for (int i = 0; i < arr.len/2; i++) {
        std::swap(arr[i], arr[arr.len-i-1]);
    }
}