#pragma once

#include <array>

namespace local
{
    // ALL maps will be of the form (v, i, j, k)
    struct rect_map_t
    {
        std::array<int, 4> min, max;
        int size_at(int idx) const {return max[idx] - min[idx]; }
        int operator() (int i0, int i1, int i2, int i3) const
        {
            int j0 = i0 - min[0];
            int j1 = i1 - min[1];
            int j2 = i2 - min[2];
            int j3 = i3 - min[3];
            
            int output = 0;
            output += j3;
            output *= size_at(2);
            output += j2;
            output *= size_at(1);
            output += j1;
            output *= size_at(0);
            output += j0;
            
            return output;
        }
        
        rect_map_t(std::array<int, 4> sizes)
        {
            max = sizes;
            min = std::array{0,0,0,0};
        }
        
        rect_map_t(std::array<int, 2> r0, std::array<int, 2> r1, std::array<int, 2> r2, std::array<int, 2> r3)
        {
            max = std::array{r0[1], r1[1], r2[1], r3[1]};
            min = std::array{r0[0], r1[0], r2[0], r3[0]};
        }
    };
    
    struct tiled_map_t
    {
        std::array<int, 4> min, max;
        int size_at(int idx) const {return max[idx] - min[idx]; }
        
        int tpow1 = 2;
        int tpow2 = 2;
        int tpow3 = 2;
        
        int operator() (int i0, int i1, int i2, int i3) const
        {
            int j0 = i0 - min[0];
            int j1 = i1 - min[1];
            int j2 = i2 - min[2];
            int j3 = i3 - min[3];
            
            int tsize1 = 1 << tpow1;
            int tsize2 = 1 << tpow2;
            int tsize3 = 1 << tpow3;
            
            int jj1 = j1 & (tsize1-1);
            j1 = j1 >> tpow1;
            
            int jj2 = j2 & (tsize2-1);
            j2 = j2 >> tpow2;
            
            int jj3 = j3 & (tsize3-1);
            j3 = j3 >> tpow3;
            
            int output = 0;
            output += jj1;
            output += jj2*tsize1;
            output += jj3*tsize1*tsize2;
            output += j1*tsize1*tsize2*tsize3;
            output += j2*tsize1*tsize2*tsize3*size_at(1);
            output += j3*tsize1*tsize2*tsize3*size_at(1)*size_at(2);
            output += j0*tsize1*tsize2*tsize3*size_at(1)*size_at(2)*size_at(3);
            
            return output;
        }
        
        tiled_map_t(std::array<int, 4> sizes)
        {
            max = sizes;
            min = std::array{0,0,0,0};
        }
        
        tiled_map_t(std::array<int, 2> r0, std::array<int, 2> r1, std::array<int, 2> r2, std::array<int, 2> r3)
        {
            max = std::array{r0[1], r1[1], r2[1], r3[1]};
            min = std::array{r0[0], r1[0], r2[0], r3[0]};
        }
    };
}