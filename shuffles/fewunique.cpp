#include <variant>
#include <iostream>
#include <cmath>
#include <random>
#include "../varray.cpp"


extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    int buckets = 8;
    if (args.size() > 0) int buckets = *std::get_if<int>(&args[0]);
    name = "Shuffle: Few unique ("+std::to_string(buckets)+")";
    for (int i = 0; i < arr.len; i++) {
        arr[i] = round(arr[i]/(arr.len/buckets)*(arr.len/buckets));
    }
}