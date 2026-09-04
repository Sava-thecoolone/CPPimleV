#include <iostream>
#include <thread>
#include <chrono>

struct highlight {
    int len;
    int cur;
    double delayMult;
    int unslept;

    highlight() {
        this->len = 1;
        cur = -1;
        delayMult = 1;
        unslept = 0;
    }

    highlight(int len) {
        this->len = len;
        cur = -1;
        delayMult = 1;
        unslept = 0;
    }

    void doHigh(int idx) {
        cur = idx;
        int delay = (int)((100000000.0*delayMult)/len);
        if (delay+unslept > 1000000) {
            std::this_thread::sleep_for(std::chrono::milliseconds((delay+unslept)/1000000));
            unslept -= (delay+unslept)/1000000*1000000;
        } else unslept += delay;
    }
};