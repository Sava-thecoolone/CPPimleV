#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Final merge";
    std::vector<int> temp (arr.len);
    for (int i = 0; i < arr.len; i++) {
        temp[i] = arr[i];
    }
    for (int i = 0; i < arr.len/2; i++) {
        arr[i] = temp[i*2];
        arr[i+arr.len/2] = temp[i*2+1];
    }
}