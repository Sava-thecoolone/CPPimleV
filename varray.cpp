#pragma once
#include <vector>
#include "highlight.cpp"

struct vval {
    int val;
    int idx;
    highlight *high;

    vval() {}
    vval(int v) : val(v) {}
    vval(int v, int i, highlight *h) : val(v), idx(i), high(h) {}
    vval &operator=(vval &other) {high->doHigh(idx, other.val); this->val = other.val; return *this;}
    vval &operator=(int val) {high->doHigh(idx, val); this->val = val; return *this;}
    vval(vval &&other) noexcept : val(other.val), idx(other.idx), high(other.high) {}
    operator int() {return val;}
};

struct varray {
    vval *values;
    int len;
    highlight high;
    std::mutex mutex;

    varray() {}
    
    varray(int l) : len(l), high(l) {
        values = new vval[len];
        for (int i = 0; i < len; i++) new (&values[i]) vval(i, i, &high);
    };

    varray(const varray &other) : len(other.len), high(other.len) {
        delete[] values;
        values = new vval[len];
        for (int i = 0; i < len; i++) new (&values[i]) vval(other.values[i], i, &high);
    }

    ~varray() {
        delete[] values;
    }

    void resize(int len) {
        std::lock_guard<std::mutex> lock(mutex);
        delete[] values;
        this->len = len;
        new (&high) highlight(len);
        values = new vval[len];
        for (int i = 0; i < len; i++) new (&values[i]) vval(i, i, &high);
    }

    varray &operator=(const varray &other) {
        if (this == &other) return *this;
        std::lock_guard<std::mutex> lock(mutex);
        delete[] values;
        new (&high) highlight(other.len);
        values = new vval[len];
        len = other.len;
        for (int i = 0; i < len; i++) new (&values[i]) vval(other.values[i], i, &high);
        return *this;
    }

    vval &operator[](int idx) {
        if (idx < 0 || idx >= len) throw std::out_of_range(std::to_string(idx));
        high.doHigh(idx, values[idx]);
        return values[idx];
    };
};