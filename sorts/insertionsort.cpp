#include <variant>
#include <iostream>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Insertion Sort";
    for (int i = 0; i < arr.len; i++) {
        int pos = -1;
        for (int j = i-1; j >= 0; j--) {
            if (arr[j] <= arr[i]) {
                pos = j;
                break;
            }
        }
        for (int j = i-1; j > pos; j--) {
            std::swap(arr[j+1], arr[j]);
        }
    }
}