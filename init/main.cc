#include <print.h>
#include "report.hpp"
#include "mmap.hpp"
#include "warp.hpp"
#include "emulate.hpp"

int main(int argc, char** argv)
{
    int nv = 5;
    
    int nx = 32;
    int ny = 32;
    int nz = 32;
    int ng = 2;
    
    // Flowfield map
    // local::rect_map_t q0({0,nv}, {-ng, nx+ng}, {-ng, ny+ng}, {-ng, nz+ng});
    local::tiled_map_t q0({0,nv}, {-ng, nx+ng}, {-ng, ny+ng}, {-ng, nz+ng});
    q0.tpow1 = 2;
    q0.tpow2 = 2;
    q0.tpow3 = 2;
    
    // Residual map
    // local::rect_map_t r0({0,nv}, {0, nx}, {0, ny}, {0, nz});
    
    local::tiled_map_t r0({0,nv}, {0, nx}, {0, ny}, {0, nz});
    r0.tpow1 = 2;
    r0.tpow2 = 2;
    r0.tpow3 = 2;
    
    local::warp_t w0;
    local::warp_t w1;
    
    local::emulate_loads(w0, w1, q0, r0);
    
    local::generate_report("report", w0, w1);
    
    return 0;
}
