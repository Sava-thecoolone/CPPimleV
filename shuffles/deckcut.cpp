#include <variant>
#include <iostream>
#include <cmath>
#include <random>
#include "../varray.cpp"

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Shuffle: Deck cut";
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(5, arr.len/4);
    for (int t = 0; t < 15; t++) {
        std::vector<int> temp (arr.len);
        for (int i = 0; i < arr.len; i++) {
            temp[i] = arr[i];
        }
        int cur = arr.len-1;
        int tempC = 0;
        while (cur > 0) {
            int next = std::max(cur-(int)(dist(rng)), 0);
            for (int j = next; j < cur; j++) {
                arr[j] = temp[tempC];
                tempC++;
            }
            cur = next;
        }
    }
}