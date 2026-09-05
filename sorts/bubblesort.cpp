#include <variant>
#include <iostream>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Bubble sort";
    for (int i = arr.len; i >= 2; i--) {
        for (int j = 0; j < i-1; j++) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
}