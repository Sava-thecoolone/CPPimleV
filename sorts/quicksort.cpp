#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


void quickSort(varray &arr, int l, int r) {
    int pivot = l;
    int x = arr[pivot];

    int i = l;
    int j = r;

    while (i <= j) {
        while (arr[i] < x) {
            i++;
        }
        while (arr[j] > x) {
            j--;
        }

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if(l < j) {
        quickSort(arr, l, j);
    }
    if(i < r) {
        quickSort(arr, i, r);
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Quick sort";
    quickSort(arr, 0, arr.len-1);
}