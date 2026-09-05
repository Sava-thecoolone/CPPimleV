#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Quadratic";
    for (int i = 0; i < arr.len; i++) {
        arr[i] = (int(((float)i/arr.len)*((float)i/arr.len)*arr.len));
    }
}