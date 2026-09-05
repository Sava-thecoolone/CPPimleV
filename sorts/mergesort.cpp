#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

std::vector<int> merge(varray &arr, int l, int mid, int r) {
    int p1 = l;
    int p2 = mid;
    std::vector<int> temp (r-l+1);
    int i = 0;
    while (p1 < mid && p2 < r) {
        if (arr[p2] < arr[p1]) {
            temp[i++] = arr[p2++];
        } else {
            temp[i++] = arr[p1++];
        }
    }
    while (p1 < mid) {
        temp[i++] = arr[p1++];
    }
    while (p2 < r) {
        temp[i++] = arr[p2++];
    }
    return temp;
}

void mergeSort(varray &arr, int l, int r) {
    if (l >= r-1) return;
    int mid = l+(r-l)/2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid, r);
    std::vector<int> temp = merge(arr, l, mid, r);
    for (int i = l; i < r; i++) {
        arr[i] = temp[i-l];
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Merge sort";
    mergeSort(arr, 0, arr.len);
}