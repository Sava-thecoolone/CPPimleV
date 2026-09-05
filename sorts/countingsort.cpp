#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Counting sort";
    std::vector<int> count(arr.len);
    for (int i = 0; i < arr.len; i++) {
        count[arr[i]]++;
    }
    int next = 0;
    for (int i = 0; i < arr.len; i++) {
        arr[i] = next;
        count[next]--;
        if (i == arr.len-1) break;
        while (count[next] == 0) next++;
    }
}