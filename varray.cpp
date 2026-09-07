#pragma once
#include <vector>
#include "highlight.cpp"

struct vval {
    int val;
    int idx;
    highlight *high;

    vval() {}
    vval(int v) : val(v) {}
    vval(int v, int i, highlight *h) noexcept : val(v), idx(i), high(h) {}
    vval &operator=(vval &other) {high->doHigh(idx, other.val); this->val = other.val; return *this;}
    vval &operator=(int val) {high->doHigh(idx, val); this->val = val; return *this;}
    vval(vval &&other) noexcept : val(other.val), idx(other.idx), high(other.high) {}
    operator int() noexcept {return val;}
};

struct varray {
    vval *values;
    int len;
    highlight high;

    varray() {}
    
    varray(int l) : len(l), high(l) {
        values = (vval*)malloc(len*sizeof(vval));
        for (int i = 0; i < len; i++) new (&values[i]) vval(i, i, &high);
    };

    varray(const varray &other) : len(other.len), high(other.high) {
        values = (vval*)malloc(len*sizeof(vval));
        for (int i = 0; i < len; i++) new (&values[i]) vval(other.values[i].val, i, &high);
    }

    ~varray() {
        free(values);
    }

    void resize(int len) {
        free(values);
        this->len = len;
        high.reset(len);
        values = (vval*)malloc(len*sizeof(vval));
        for (int i = 0; i < len; i++) new (&values[i]) vval(i, i, &high);
    }

    varray &operator=(const varray &other) {
        if (this == &other) return *this;
        free(values);
        len = other.len;
        high.reset(len);
        values = (vval*)malloc(len*sizeof(vval));
        for (int i = 0; i < len; i++) new (&values[i]) vval(other.values[i], i, &high);
        return *this;
    }

    vval &operator[](int idx) {
        if (idx < 0 || idx >= len) throw std::out_of_range(std::to_string(idx));
        high.doHigh(idx, values[idx]);
        return values[idx];
    };
};