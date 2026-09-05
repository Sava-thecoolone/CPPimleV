#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


std::vector<int> countingSort(varray &arr, int exp, int base) {
    std::vector<int> count(base);
    for (int i = 0; i < arr.len; i++) {
        count[(int)(arr[i]/exp)%base]++;
    }
    return count;
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    int base = (*std::get_if<int>(&args[0]));
    name = "Sort: LSD Radix sort (base "+std::to_string(base)+")";
    int max = 0;
    std::vector<int> temp (arr.len);
    for (int i = 0; i < arr.len; i++) {
        if (arr[i] > max) max = arr[i];
        temp[i] = arr[i];
    }
    int exp = 1;
    while (exp < max) {
        std::vector<int> count = countingSort(arr, exp, base);
        for (int i = 1; i < base; i++) {
            count[i] += count[i-1];
        }
        for (int i = arr.len-1; i >= 0; i--) {
            int digit = (temp[i]/exp)%base;
            arr[--count[digit]] = temp[i];
        }
        for (int i = 0; i < arr.len; i++) {
            temp[i] = arr[i];
        }
        exp *= base;
    }
}