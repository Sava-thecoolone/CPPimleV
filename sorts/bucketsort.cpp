#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"


int binSearchLast(varray &arr, int l, int r, int val) {
    while (l < r) {
        int pos = l+(r-l)/2;
        if (arr[pos] <= val) l = pos+1;
        else r = pos;
    }
    return l;
}

void insertionSort(varray &arr, int l, int r) {
    for (int i = l; i < r; i++) {
        int pos = binSearchLast(arr, l, i, arr[i]);
        for (int j = i-1; j >= pos; j--) {
            std::swap(arr[j+1], arr[j]);
        }
    }
}

std::vector<int> countingSort(varray &arr, int buckets, double scale) {
    std::vector<int> count(buckets);
    for (int i = 0; i < arr.len; i++) {
        count[std::min((int)(arr[i]/scale), buckets-1)]++;
    }
    return count;
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    int buckets = arr.len/(*std::get_if<int>(&args[0]));
    name = "Sort: Bucket sort (n/"+std::to_string(*std::get_if<int>(&args[0]))+" buckets)";
    int max = 0;
    std::vector<int> temp (arr.len);
    for (int i = 0; i < arr.len; i++) {
        if (arr[i] > max) max = arr[i];
        temp[i] = arr[i];
    }
    double scale = max/(double)buckets;
    std::vector<int> count = countingSort(arr, buckets, scale);
    for (int i = 1; i < buckets; i++) {
        count[i] += count[i-1];
    }
    std::vector<int> countTemp = count;
    for (int i = arr.len-1; i >= 0; i--) {
        arr[--count[std::min((int)(temp[i]/scale), buckets-1)]] = temp[i];
    }
    insertionSort(arr, 0, countTemp[0]);
    for (int i = 1; i < buckets; i++) {
        insertionSort(arr, countTemp[i-1], countTemp[i]);
    }
}