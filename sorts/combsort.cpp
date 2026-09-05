#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Comb sort";
    int gap = arr.len;
    while (true) {
        bool sorted = true;
        for (int j = 0; j < arr.len-gap; j++) {
            if (arr[j] > arr[j+gap]) {
                std::swap(arr[j], arr[j+gap]);
                sorted = false;
            }
        }
        gap = (int)(gap/1.3f);
        if (gap < 1) {
            if (sorted) break;
            gap = 1;
        }
    }
}