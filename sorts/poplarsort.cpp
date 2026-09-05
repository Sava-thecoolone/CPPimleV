#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

bool presentTrees[32] = {};
const int leonardo[] = {1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891, 35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457, 1664079, 2692537, 4356617};

int nextTree(int i) {
    for (int j = i+1; j < 32; j++) {
        if (presentTrees[j]) return j;
    }
    return -1;
}

void maxheapify(varray &arr, int root, int pos, int l) {
    if (root < 2) return;
    int maxPos = pos;
    int maxRoot = root;
    int left = pos-leonardo[root-2]-1;
    int right = pos-1;
    if (left >= l && arr[left] > arr[maxPos]) {maxPos = left; maxRoot = root-1;}
    if (right >= l && arr[right] > arr[maxPos]) {maxPos = right; maxRoot = root-2;}
    if (maxPos != pos) {
        std::swap(arr[pos], arr[maxPos]);
        maxheapify(arr, maxRoot, maxPos, l);
    }
}

void addTree() {
    int curTree = 30;
    while (curTree >= 0 && !(presentTrees[curTree+1] && presentTrees[curTree])) curTree--;
    if (curTree < 0) {
        if (!presentTrees[1]) presentTrees[1] = true;
        else presentTrees[0] = true;
    } else {
        presentTrees[curTree] = false;
        presentTrees[curTree+1] = false;
        presentTrees[curTree+2] = true;
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Poplar sort";
    presentTrees[0] = true;
    for (int i = 1; i < arr.len; i++) {
        addTree();
        maxheapify(arr, nextTree(-1), i, 0);
    }
    for (int i = arr.len-1; i >= 1; i--) {
        int maxRoot = nextTree(-1);
        if (maxRoot == -1) break;
        int maxPos = i;
        int curRoot = maxRoot;
        int curPos = i;
        while (curRoot != -1) {
            if (arr[curPos] > arr[maxPos]) {
                maxPos = curPos;
                maxRoot = curRoot;
            }
            curPos -= leonardo[curRoot];
            curRoot = nextTree(curRoot);
        }
        std::swap(arr[maxPos], arr[i]);
        maxheapify(arr, maxRoot, maxPos, 0);
        int curTree = nextTree(-1);
        presentTrees[curTree] = false;
        if (curTree >= 2) {
            presentTrees[curTree-1] = true;
            maxheapify(arr, curTree-1, i-leonardo[curTree-2]-1, 0);
            presentTrees[curTree-2] = true;
            maxheapify(arr, curTree-2, i-1, 0);
        }
    }
}