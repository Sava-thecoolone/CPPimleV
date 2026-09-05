#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

int rotate(varray &arr, int l, int mid, int r) {
    int a = l;
    int b = mid;
    int c = mid;
    int d = r;
    if (mid-l > r-mid) {
        int loop = (r-mid)/2;
        while (loop-- > 0) {
            b--; d--;
            int temp = arr[b];
            arr[b] = arr[a];
            arr[a] = arr[c];
            arr[c] = arr[d];
            arr[d] = temp;
            a++; c++;
        }
        loop = (b-a)/2;
        while (loop-- > 0) {
            b--; d--;
            int temp = arr[b];
            arr[b] = arr[a];
            arr[a] = arr[d];
            arr[d] = temp;
            a++;
        }
        loop = (d-a)/2;
        while (loop-- > 0) {
            d--;
            int temp = arr[a];
            arr[a] = arr[d];
            arr[d] = temp;
            a++;
        }
    } else if (mid-l < r-mid) {
        int loop = (mid-l)/2;
        while (loop-- > 0) {
            b--; d--;
            int temp = arr[b];
            arr[b] = arr[a];
            arr[a] = arr[c];
            arr[c] = arr[d];
            arr[d] = temp;
            a++; c++;
        }
        loop = (d-c)/2;
        while (loop-- > 0) {
            d--;
            int temp = arr[c];
            arr[c] = arr[d];
            arr[d] = arr[a];
            arr[a] = temp;
            a++; c++;
        }
        loop = (d-a)/2;
        while (loop-- > 0) {
            d--;
            int temp = arr[a];
            arr[a] = arr[d];
            arr[d] = temp;
            a++;
        }
    } else {
        int loop = mid-l;
        while (loop-- > 0) {
            int temp = arr[a];
            arr[a] = arr[b];
            arr[b] = temp;
            a++; b++;
        }
    }
    return l+r-mid;
}


int binlog(int bits) {
    int log = 0;
    if ((bits & 0xffff0000) != 0) {bits >>= 16; log = 16;}
    if (bits >= 256) {bits >>= 8; log += 8;}
    if (bits >= 16) {bits >>= 4; log += 4;}
    if (bits >= 4) {bits >>= 2; log += 2;}
    return log + (bits >> 1);
}

int binSearch(varray &arr, int l, int r, int val) {
    while (l < r) {
        int pos = l+(r-l)/2;
        if (arr[pos] < val) l = pos+1;
        else r = pos;
    }
    return l;
}

void merge(varray &arr, int l, int mid, int r) {
    if (l >= mid || mid >= r) return;
    if (arr[mid-1] <= arr[mid]) return;
    int split = l+(mid-l)/2;
    int pos = binSearch(arr, mid, r, arr[split]);
    int midSplit = rotate(arr, split, mid, pos);
    merge(arr, l, split, midSplit);
    merge(arr, midSplit, pos, r);
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Rotate merge sort";
    for (int i = (int)ceil(binlog(arr.len)); i >= 0; i--) {
        double scale = arr.len/pow(2, i);
        
        for (int j = 0; j < pow(2, i); j++) {
            int start = ((int)(j*scale));
            int end = ((int)((j+1)*scale));
            int mid = ((int)(j*scale+scale/2));
            merge(arr, start, mid, end);
        }
    }
}