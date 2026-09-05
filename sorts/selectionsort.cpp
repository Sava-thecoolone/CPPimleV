#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Selection sort";
    for (int i = 0; i < arr.len; i++) {
        int pos = i;
        for (int j = i+1; j < arr.len; j++) {
            if (arr[j] < arr[pos]) {
                pos = j;
            }
        }
        std::swap(arr[i], arr[pos]);
    }
}