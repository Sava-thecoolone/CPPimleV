#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

void maxheapify(varray &arr, int pos, int len) {
    int max = pos;
    int left = pos*2;
    int right = pos*2+1;
    if (left < len && arr[left] > arr[max]) max = left;
    if (right < len && arr[right] > arr[max]) max = right;
    if (max != pos) {
        std::swap(arr[pos], arr[max]);
        maxheapify(arr, max, len);
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Heap sort";
    for (int i = arr.len/2; i >= 0; i--) {
        maxheapify(arr, i, arr.len);
    }
    for (int i = arr.len-1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        maxheapify(arr, 0, i);
    }
}