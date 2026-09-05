#pragma once
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <stacktrace>
#include "highlight.cpp"

struct varray {
    int *values;
    int len;
    highlight high;

    varray() {}
    
    varray(int l) : len(l), high(l) {
        values = (int*)malloc(len*sizeof(int));
        for (int i = 0; i < len; i++) values[i] = i;
    };

    varray(const varray &other) : len(other.len) {
        free(values);
        values = (int*)malloc(len*sizeof(int));
        memcpy(values, other.values, len*sizeof(int));
    }

    ~varray() {
        free(values);
    }

    void resize(int len) {
        free(values);
        this->len = len;
        high.len = len;
        values = (int*)malloc(len*sizeof(int));
        for (int i = 0; i < len; i++) values[i] = i;
    }

    varray &operator=(const varray &other) {
        if (this == &other) return *this;
        free(values);
        len = other.len;
        high.len = len;
        values = (int*)malloc(len*sizeof(int));
        memcpy(values, other.values, len*sizeof(int));
        return *this;
    }

    int &operator[](int idx) {
        if (idx < 0 || idx >= len) throw std::out_of_range(std::to_string(idx));
        high.doHigh(idx);
        return values[idx];
    };
};