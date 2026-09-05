#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

int binlog(int bits) {
    int log = 0;
    if ((bits & 0xffff0000) != 0) {bits >>= 16; log = 16;}
    if (bits >= 256) {bits >>= 8; log += 8;}
    if (bits >= 16) {bits >>= 4; log += 4;}
    if (bits >= 4) {bits >>= 2; log += 2;}
    return log + (bits >> 1);
}

bool sortRecurse(varray &arr, int l, int r) {
    if (l >= r-1) return false;

    bool swapped = false;
    int end = l+(int)pow(2, binlog(r-l));
    for (int i = l; i < l+(end-l)/2; i++) {
        if (end-i+l-1 < r && arr[i] > arr[end-i+l-1]) {
            std::swap(arr[i], arr[end-i+l-1]);
            swapped = true;
        }
    }
    swapped |= sortRecurse(arr, l, l+(end-l)/2);
    swapped |= sortRecurse(arr, l+(end-l)/2, r);
    return swapped;
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Circle sort";
    while (true) {
        if (!sortRecurse(arr, 0, arr.len)) break;
    }
}