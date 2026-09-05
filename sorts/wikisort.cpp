#include <cmath>
#include <variant>
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

void rotateMerge(varray &arr, int l, int mid, int r) {
    while (true) {
        int pos = binSearch(arr, mid, r, arr[l]);
        l = rotate(arr, l, mid, pos);
        if (mid >= r) break;
        mid = pos;
        while (l < mid && arr[l] <= arr[mid]) l = binSearchLast(arr, l+1, mid, arr[l]);
        if (l >= mid) break;
    }
}

void bufferMerge(varray &arr, int l, int mid, int r, int bufferl) {
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

void blockSwap(varray &arr, int b1, int b2, int blockSize) {
    for (int i = 0; i < blockSize; i++) {
        std::swap(arr[i+b1], arr[i+b2]);
    }
}

void wikiMerge(varray &arr, int la, int mid, int ra, int blockSize, int tagBufferL, int tagBufferR, int mergeBufferL, int mergeBufferR) {
    if (arr[mid-1] <= arr[mid]) return;
    if (arr[la] >= arr[ra-1]) {
        rotate(arr, la, mid, ra);
        return;
    }
    int l = la, r = ra;
    int numA = (mid-l)/blockSize;
    int unmergedL = l;
    l += (mid-l)%blockSize;
    for (int i = 0; i < numA; i++) {
        std::swap(arr[l+i*blockSize+1], arr[tagBufferL+i]);
    }
    int unmergedR = l;
    int lastB = -1;
    int curTag = tagBufferL;
    for (int i = l; i <= r-blockSize; i += blockSize) {
        int smallestA = l;
        if (i+numA*blockSize <= r) {
            smallestA = i;
            for (int j = i+blockSize; j < i+numA*blockSize; j += blockSize) {
                if (arr[j] < arr[smallestA] ||
                    (arr[j] == arr[smallestA] && arr[j+1] < arr[smallestA+1])) smallestA = j;
            }
        }
        if (i+numA*blockSize >= r || (lastB != -1 && arr[smallestA] <= arr[lastB])) {
            if (i+numA*blockSize <= r) {blockSwap(arr, i, smallestA, blockSize);}
            numA--;
            if (unmergedR-unmergedL == blockSize) {
                std::swap(arr[unmergedL+1], arr[curTag++]);
            }
            int pos = binSearch(arr, unmergedR, i, arr[i]);
            if (unmergedR-unmergedL > mergeBufferR-mergeBufferL) rotateMerge(arr, unmergedL, unmergedR, pos);
            else bufferMerge(arr, unmergedL, unmergedR, pos, mergeBufferL);
            rotate(arr, pos, i, i+blockSize);
            unmergedL = pos;
            unmergedR = unmergedL+blockSize;
            lastB = -1;
        } else {
            int bSize = std::min(i+numA*blockSize+blockSize, r)-(i+numA*blockSize);
            if (bSize == blockSize) blockSwap(arr, i, i+numA*blockSize, blockSize);
            else rotate(arr, i, i+numA*blockSize, std::min(i+numA*blockSize+blockSize, r));
            lastB = i+bSize-1;
            i += bSize-blockSize;
        }
    }
    if (unmergedR-unmergedL == blockSize) {
        std::swap(arr[unmergedL+1], arr[curTag++]);
    }
    if (unmergedR-unmergedL > mergeBufferR-mergeBufferL) rotateMerge(arr, unmergedL, unmergedR, r);
    else bufferMerge(arr, unmergedL, unmergedR, r, mergeBufferL);
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    name = "Sort: Wiki Sort";
    int len = arr.len;
    int minrun = (int)ceil(binlog(len))-5;
    for (int j = 0; j <pow(2, minrun); j++) {
        int start = ((int)(j*(len/pow(2, minrun))));
        int end = ((int)((j+1)*(len/pow(2, minrun))));
        insertionSort(arr, start, end);
    }
    for (int i = minrun-1; i >= 0; i--) {
        double scale = len/pow(2, i);
        int lenA = ((int)(scale/2));
        int blockSize = (int)sqrt((int)(lenA));
        int bufferSize = (int)(lenA)/blockSize+1;
        int found = 1;
        int last = 0;
        while (found < bufferSize*2) {
            int pos = binSearchLast(arr, last, lenA, arr[last]);
            if (pos >= lenA) break;
            last = pos;
            found++;
        }
        int mergeBuff = bufferSize;
        if (found < bufferSize*2) {
            mergeBuff = 0;
            found = 1;
            last = 0;
            while (found < bufferSize) {
                int pos = binSearchLast(arr, last, lenA, arr[last]);
                if (pos >= lenA) break;
                last = pos;
                found++;
            }
        }
        found = 1;
        while (last > 0) {
            int pos = binSearch(arr, 0, last, arr[last-1]);
            if (pos+1 != last) rotate(arr, pos+1, last, last+found);
            last = pos;
            found++;
        }
        int tagBuff = std::min(found, bufferSize);
        blockSize = (int)(lenA)/(tagBuff-1)+1;
        
        for (int j = 0; j < pow(2, i); j++) {
            int start = ((int)(j*scale));
            int end = ((int)((j+1)*scale));
            int mid = ((int)(j*scale+scale/2));
            if (j == 0) start += tagBuff+mergeBuff;
            wikiMerge(arr, start, mid, end, blockSize, 0, tagBuff, tagBuff, tagBuff+mergeBuff);
        }
        insertionSort(arr, 0, tagBuff+mergeBuff);
        rotateMerge(arr, 0, tagBuff+mergeBuff, (int)scale);
    }
}