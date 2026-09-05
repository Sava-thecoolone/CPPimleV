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

void insertionSort(varray &arr, int l, int r) {
    for (int i = l; i < r; i++) {
        int pos = l-1;
        for (int j = i-1; j >= l; j--) {
            if (arr[j] <= arr[i]) {
                pos = j;
                break;
            }
        }
        for (int j = i-1; j > pos; j--) {
            std::swap(arr[j+1], arr[j]);
        }
    }
}

void maxheapify(varray &arr, int pos, int r) {
    int max = pos;
    int left = pos*2;
    int right = pos*2+1;
    if (left < r && arr[left] > arr[max]) max = left;
    if (right < r && arr[right] > arr[max]) max = right;
    if (max != pos) {
        std::swap(arr[pos], arr[max]);
        maxheapify(arr, max, r);
    }
}

void heapSort(varray &arr, int l, int r) {
    for (int i = l+(r-l)/2; i >= l; i--) {
        maxheapify(arr, i, r);
    }
    for (int i = r-1; i >= l; i--) {
        std::swap(arr[l], arr[i]);
        maxheapify(arr, l, i);
    }
}

void introSort(varray &arr, int l, int r, int rec) {
    if (r-l < 16) return;
    if (rec <= 0) {
        heapSort(arr, l, r);
        return;
    }
    int pivot = l+(r-l)/2;
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

    if (l < j) {
        introSort(arr, l, j, rec-1);
    }
    if (i < r) {
        introSort(arr, i, r, rec-1);
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Intro sort";
    introSort(arr, 0, arr.len-1, 2*binlog(arr.len));
    insertionSort(arr, 0, arr.len);
}