#pragma once

#include <vector>

namespace local
{
    struct thread_t
    {
        std::vector<int> loads;
        int nloads() const { return loads.size(); }
        void load(int i) { loads.push_back(i); }
    };
    
    struct warp_t
    {
        std::array<thread_t, 32> threads;
        thread_t& operator[] (int i) { return threads[i]; }
        const thread_t& operator[] (int i) const { return threads[i]; }
    };
}