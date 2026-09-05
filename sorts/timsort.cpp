#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

void reverse(varray &arr, int l, int r) {
    for (int i = l; i < (l+r)/2; i++) {
        std::swap(arr[i], arr[r-i-1+l]);
    }
}

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


int binSearchLast(varray &arr, int l, int r, int val, int dir) {
    while (l < r) {
        int pos = l+(r-l)/2;
        if (dir < 0 ? arr[pos] > val : arr[pos] <= val) l = pos+1;
        else r = pos;
    }
    return l;
}

void insertionSort(varray &arr, int l, int r, int dir) {
    for (int i = l; i < r; i++) {
        int pos = binSearchLast(arr, l, i, arr[i], dir);
        for (int j = i-1; j >= pos; j--) {
            std::swap(arr[j+1], arr[j]);
        }
    }
}

void merge(varray &arr, int l, int mid, int r) {
    if (arr[mid-1] <= arr[mid]) return;
    if (arr[l] >= arr[r-1]) {
        rotate(arr, l, mid, r);
        return;
    }
    int p1 = 0;
    int p2 = mid;
    std::vector<int> temp (mid-l);
    for (int i = l; i < mid; i++) {
        temp[i-l] = arr[i];
    }
    int i = l;
    while (p1 < mid-l && p2 < r) { 
        if (temp[p1] < arr[p2]) {
            arr[i++] = temp[p1++];
        } else {
            arr[i++] = arr[p2++];
        }
    }
    while (p1 < mid-l) { 
        arr[i++] = temp[p1++];
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Tim sort";
    int minrun = 0;
    int n = arr.len;
    while (n >= 32) {
        minrun |= (n & 1);
        n >>= 1;
    }
    minrun += n;
    std::vector<int> st;
    st.push_back(0);
    int curPos = 0;
    while (curPos < arr.len) {
        int curRun = curPos;
        if (curRun < arr.len-1) {
            int dir = arr[curRun] > arr[curRun+1] ? -1 : 1;
            while (curRun+1 < arr.len && (arr[curRun]-arr[curRun+1])*dir <= 0) curRun++;
            curRun++;
            if (curRun-curPos < minrun) {insertionSort(arr, curPos, std::min(curPos+minrun, arr.len), dir); curRun = std::min(curPos+minrun, arr.len);}
            if (dir < 0) reverse(arr, curPos, curRun);
        } else curRun = arr.len;
        st.push_back(curRun);
        if (st.size() >= 4) {
            int X = curRun;
            int Y = st[st.size()-2];
            int Z = st[st.size()-3];
            int W = st[st.size()-4];
            while (X-Y >= Y-Z || X-Z >= Z-W) {
                if (X-Y < Z-W) {
                    merge(arr, Z, Y, X);
                    st.erase(st.end()-2);
                } else {
                    merge(arr, W, Z, Y);
                    st.erase(st.end()-3);
                }
                if (st.size() < 4) break;
                Y = st[st.size()-2];
                Z = st[st.size()-3];
                W = st[st.size()-4];
            }
        }
        if (st.size() >= 3) {
            std::cout << st.size() << "\n";
            int X = curRun;
            int Y = st[st.size()-2];
            int Z = st[st.size()-3];
            while (X-Y >= Y-Z) {
                merge(arr, Z, Y, X);
                st.erase(st.end()-2);
                if (st.size() < 3) break;
                Y = st[st.size()-2];
                Z = st[st.size()-3];
            }
        }
        curPos = curRun;
    }
    while (st.size() >= 3) {
        merge(arr, st[st.size()-3], st[st.size()-2], st[st.size()-1]);
        st.erase(st.end()-2);
    }
}