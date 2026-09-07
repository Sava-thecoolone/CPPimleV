#pragma once
#include <thread>
#include <chrono>
#include <atomic>

struct highlight {
    int len;
    std::atomic<int> lastidx;
    std::atomic<int> lastval;
    double delayMult;
    int unslept;

    highlight() {
        this->len = 0;
        lastidx.store(-1);
        lastval.store(-1);
        delayMult = 1;
        unslept = 0;
    }

    highlight(int len) {
        this->len = len;
        lastidx.store(-1);
        lastval.store(-1);
        delayMult = 1;
        unslept = 0;
    }

    void doHigh(int idx, int val) {
        lastidx.store(idx);
        lastval.store(val);
        int delay = (int)((100000000.0*delayMult)/len);
        if (delay+unslept > 1000000) {
            std::this_thread::sleep_for(std::chrono::milliseconds((delay+unslept)/1000000));
            unslept -= (delay+unslept)/1000000*1000000;
        } else unslept += delay;
    }
};