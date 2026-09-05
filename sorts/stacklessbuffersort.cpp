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

int medianidx(varray &arr, int a, int b, int c) {
    if (arr[a] > arr[b]) {
        return arr[a] < arr[c] ? a : (arr[b] < arr[c] ? c : b);
    } else {
        return arr[a] > arr[c] ? a : (arr[b] > arr[c] ? c : b);
    }
}

int getPivot(varray &arr, int l, int r) {
    if (r-l <= 16) return l+(r-l+1)/2;
    int nineth = (r-l+1)/9;
    int m1 = medianidx(arr, l+nineth/2, l+nineth/2+nineth, l+nineth/2+nineth*2);
    int m2 = medianidx(arr, l+nineth/2+nineth*3, l+(r-l+1)/2, l+nineth/2+nineth*5);
    int m3 = medianidx(arr, l+nineth/2+nineth*6, l+nineth/2+nineth*7, l+nineth/2+nineth*8);
    return medianidx(arr, m1, m2, m3);
}

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



int partition(varray &arr, int l, int r) {
    int pivotIdx = getPivot(arr, l, r);
    std::swap(arr[pivotIdx], arr[r-1]);
    int pivot = arr[r-1];
    int p1 = l;
    int p2 = r-2;

    while (p1 <= p2) {
        while (arr[p1] < pivot) p1++;
        while (arr[p2] > pivot) p2--;
        if (p1 <= p2) std::swap(arr[p1++], arr[p2--]);
    }
    std::swap(arr[r-1], arr[p1]);

    return p1;
}

void bufferMerge(varray &arr, int l, int mid, int r, int bufferl) {
    if (arr[mid-1] <= arr[mid]) return;
    if (arr[l] >= arr[r-1]) {
        rotate(arr, l, mid, r);
        return;
    }
    int p1 = 0, p2 = 0, cur = 0;
    for (int i = l; i < mid; i++) {
        std::swap(arr[bufferl+i-l], arr[i]);
    }
    while (p1+l < mid && mid+p2 < r) {
        if (arr[bufferl+p1] <= arr[mid+p2]) {
            std::swap(arr[bufferl+p1], arr[l+cur]);
            p1++;
        } else {
            std::swap(arr[mid+p2], arr[l+cur]);
            p2++;
        }
        cur++;
    }
    while (p1+l < mid) {
        std::swap(arr[bufferl+p1], arr[l+cur]);
        p1++;
        cur++;
    }
}

void quickSelect(varray &arr, int l, int r, int k) {
    int pivot = partition(arr, l, r);
    if (k == pivot) {}
    else if (pivot > k) quickSelect(arr, l, pivot, k);
    else quickSelect(arr, pivot+1, r, k);
}

void iterMerge(varray &arr, int l, int r, int bufferl) {
    int length = r-l;
    for (int i = (int)ceil(binlog(length))-2; i >= 0; i--) {
        double scale = length/pow(2, i);
        
        for (int j = 0; j < pow(2, i); j++) {
            int start = ((int)(j*scale))+l;
            int end = ((int)((j+1)*scale))+l;
            if (scale <= 8) {
                insertionSort(arr, start, end);
            } else {
                int mid = ((int)((j+0.5f)*scale))+l;
                bufferMerge(arr, start, mid, end, bufferl);
            }
        }
    }
}

void buffMergeSort(varray &arr, int l, int r) {
    if (r <= l) return;
    if (r-l <= 32) {
        insertionSort(arr, l, r);
        return;
    }
    int third = (int)ceil((r-l+1)/3);
    quickSelect(arr, l, r, l+third);
    iterMerge(arr, l+third, r, l);
    while (third > 16) {
        iterMerge(arr, l, l+third, r-third/2);
        third /= 2;
        iterMerge(arr, r-third, r, l);
        third /= 2;
    }
    insertionSort(arr, l, l+third);
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Stackless tri-buffer sort";
    buffMergeSort(arr, 0, arr.len);
}