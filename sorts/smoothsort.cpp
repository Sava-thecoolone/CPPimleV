#include <variant>
#include <iostream>
#include <cmath>
#include "../varray.cpp"

bool presentTrees[32] = {};
const int leonardo[32] = {1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891, 35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457, 1664079, 2692537, 4356617};

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

void insert(varray &arr, int pos) {
    int curRoot = nextTree(-1);
    int curPos = pos;
    while (curRoot != -1) {
        int checkRoot = curRoot;
        int checkPos = curPos;
        while (true) {
            int nextRoot = nextTree(checkRoot);
            int nextPos = checkPos-leonardo[checkRoot];
            if (nextRoot != -1 && arr[nextPos] > arr[checkPos] &&
                (checkRoot < 2 || arr[nextPos] > arr[checkPos-1]) &&
                (checkRoot < 2 || arr[nextPos] > arr[checkPos-leonardo[checkRoot-2]-1])) {
                    std::swap(arr[nextPos], arr[checkPos]);
                    checkPos = nextPos;
                    checkRoot = nextRoot;
            } else break;
        }
        maxheapify(arr, checkRoot, checkPos, 0);
        curPos -= leonardo[curRoot];
        curRoot = nextTree(curRoot);
    }
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Smooth sort";
    presentTrees[0] = true;
    for (int i = 1; i < arr.len; i++) {
        addTree();
        insert(arr, i);
    }
    for (int i = arr.len-1; i >= 1; i--) {
        if (presentTrees[0]) presentTrees[0] = false;
        else if (presentTrees[1]) presentTrees[1] = false;
        else {
            int curTree = nextTree(-1);
            if (curTree == -1) break;
            presentTrees[curTree] = false;
            presentTrees[curTree-1] = true;
            insert(arr, i-leonardo[curTree-2]-1);
            presentTrees[curTree-2] = true;
            insert(arr, i-1);
        }
    }
}