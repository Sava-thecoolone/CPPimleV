#pragma once
#include <thread>
#include <chrono>
#include <atomic>

struct highlight {
    int len;
    std::atomic<int> lastidx;
    std::atomic<int> lastval;
    double delaymult;
    int unslept;

    highlight() noexcept : len(0), lastidx(-1), lastval(-1), delaymult(1), unslept(0) {}
    highlight(int l) noexcept : len(l), lastidx(-1), lastval(-1), delaymult(1), unslept(0) {}
    highlight(const highlight &other) noexcept : len(other.len), lastidx(-1), lastval(-1), delaymult(other.delaymult), unslept(0) {}

    void reset() noexcept {
        lastidx.store(-1);
        lastval.store(-1);
        unslept = 0;
    }

    void reset(int len) noexcept {
        this->len = len;
        lastidx.store(-1);
        lastval.store(-1);
        unslept = 0;
    }

    void doHigh(int idx, int val) {
        lastidx.store(idx);
        lastval.store(val);
        int delay = (int)((100000000.0*delaymult)/len);
        if (delay+unslept > 1000000) {
            std::this_thread::sleep_for(std::chrono::milliseconds((delay+unslept)/1000000));
            unslept -= (delay+unslept)/1000000*1000000;
        } else unslept += delay;
    }
};