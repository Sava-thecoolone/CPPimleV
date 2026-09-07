#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

void reverse(varray &arr, int l, int r) {
    for (int i = l; i < (l+r)/2; i++) {
        std::swap(arr[i], arr[r-i-1+l]);
    }
}


void reverserec(varray &arr, int l, int r, int parity) {
    if (l+1 >= r) return;
    int mid = l+(r-l)/2;
    reverse(arr, mid, r);
    reverserec(arr, l, mid, parity+1);
    reverserec(arr, mid, r, parity+1);
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Recursive reverse";
    reverserec(arr, 0, arr.len, 0);
}