#pragma once

#include "warp.hpp"

namespace local
{
    template <typename qmap_t, typename rmap_t>
    inline void emulate_loads(warp_t& warp0, warp_t& warp1, const qmap_t& qmap, const rmap_t& rmap)
    {
        const auto loadq = [&] (thread_t& thrd, int ii, int jj, int kk)
        {
            // for (int v = 0; v < 5; ++v)
            // {
                thrd.load(qmap(0, ii, jj, kk));
            // }
        };
        
        const auto loadr = [&] (thread_t& thrd, int ii, int jj, int kk)
        {
            // for (int v = 0; v < 5; ++v)
            // {
                thrd.load(rmap(0, ii, jj, kk));
            // }
        };
        
        int tile_size = 4;
        for (int k = 0; k < tile_size; ++k)
        {
            for (int j = 0; j < tile_size; ++j)
            {
                for (int i = 0; i < tile_size; ++i)
                {
                    int id_1d = i + tile_size*(j + k*tile_size);
                    warp_t& warp = (id_1d < 32)?warp0:warp1;
                    
                    int thread_id = id_1d;
                    if (thread_id >= 32) thread_id -= 32;
                    thread_t& thr = warp[thread_id];
                    loadq(thr, i, j, k);
                    
                    for (int idir = 0; idir < 3; ++idir)
                    {
                        
                    }
                    
                    
                    loadr(thr, i, j, k);
                    loadr(thr, i-1, j, k);
                }
            }
        }
    }
}