#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


std::vector<int> countingSort(varray &arr, int l, int r, int exp, int base) {
    std::vector<int> count (base);
    for (int i = l; i < r; i++) {
        count[(int)(arr[i]/exp)%base]++;
    }
    return count;
}

void sortRecurse(varray &arr, int l, int r, int base, int exp) {
    if (l >= r-1 || exp < 1) return;
    std::vector<int> temp (r-l);
    for (int i = 0; i < r-l; i++) {
        temp[i] = arr[i+l];
    }
    std::vector<int> count = countingSort(arr, l, r, exp, base);
    for (int i = 1; i < base; i++) {
        count[i] += count[i-1];
    }
    std::vector<int> countTemp = count;
    for (int i = r-1; i >= l; i--) {
        int digit = (temp[i-l]/exp)%base;
        arr[--count[digit]+l] = temp[i-l];
    }
    sortRecurse(arr, l, countTemp[0]+l, base, exp/base);
    for (int i = 1; i < base; i++) {
        sortRecurse(arr, countTemp[i-1]+l, countTemp[i]+l, base, exp/base);
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    int base = (*std::get_if<int>(&args[0]));
    name = "Sort: MSD Radix sort (base "+std::to_string(base)+")";
    int max = 0;
    for (int i = 0; i < arr.len; i++) {
        if (arr[i] > max) max = arr[i];
    }
    int exp = (int)pow(base, (int)(log(max)/log(base)));
    sortRecurse(arr, 0, arr.len, base, exp);
}