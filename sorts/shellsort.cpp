#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Shell sort";
    int gap = arr.len;
    while (gap > 1) {
        gap = std::max((int)(gap/2.3), 1);
        for (int i = 0; i < arr.len; i++) {
            int pos = -1;
            for (int j = i-gap; j >= 0; j -= gap) {
                if (arr[j] <= arr[i]) {
                    pos = j;
                    break;
                }
            }
            for (int j = i-gap; j > pos; j -= gap) {
                std::swap(arr[j+gap], arr[j]);
            }
        }
    }
}