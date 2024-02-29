#pragma once

#include "nicefig.h"
#include "warp.hpp"

namespace local
{
    inline void generate_report(const std::string& direc, const warp_t& warp0, const warp_t& warp1)
    {
        print("Generating report file...");
        
        int nloads = warp0[0].nloads();
        
        double max     = 800;
        double aspect  = 1.0;
        double spacing = 1.5;
        
        nicefig::figure_t fig(direc + "/out.tex");
        std::ofstream statfile(direc + "/stats.txt");
        
        std::size_t total_fetch = 0;
        for (int lid = 0; lid < nloads; ++lid)
        {
            statfile << "Load " << lid << ":\n";
            nicefig::curve_t d0;
            nicefig::curve_t d1;
            const auto addw = [&](const auto& wrp, auto& crv, int load_id, int& rmx)
            {
                rmx = 0;
                for (int tid = 0; tid < 32; ++tid)
                {
                    crv.xdat.push_back(tid);
                    crv.ydat.push_back(wrp[tid].loads[load_id] - wrp[0].loads[load_id]);
                    rmx = std::max(rmx, wrp[tid].loads[load_id] - wrp[0].loads[load_id]);
                }
            };
            
            const auto getls = [&](const auto& offsts)
            {
                std::vector<int> output;
                std::vector<bool> loaded;
                for (const auto& e: offsts) loaded.push_back(false);
                
                const auto all_loaded = [&]()
                {
                    bool boutput = true;
                    for (const auto& e: loaded) boutput = boutput && e;
                    return boutput;
                };
                
                const auto vec_contains = [](const auto& vc, const auto& val, auto& idx_out)
                {
                    idx_out = -1;
                    for (int j = 0; j < vc.size(); ++j)
                    {
                        if (vc[j] == val)
                        {
                            idx_out = j;
                            return true;
                        }
                    }
                    return false;
                };
                
                while (!all_loaded())
                {
                    int idx = 0;
                    while (loaded[idx]) idx++;
                    int load_at = round(offsts[idx]);
                    loaded[idx] = true;
                    output.push_back(load_at);
                    for (int srch = 1; srch < 32; ++srch)
                    {
                        int check_elm = load_at + srch;
                        int idxx;
                        if (vec_contains(offsts, check_elm, idxx))
                        {
                            loaded[idxx] = true;
                        }
                    }
                }
                
                return output;
            };
            
            int mx0, mx1;
            addw(warp0, d0, lid, mx0);
            addw(warp1, d1, lid, mx1);
            int mx = std::max(mx0, mx1);
            
            double ymin = max*aspect*spacing*(nloads - lid + 1);
            nicefig::bnd_t ub0{0.0, max,  ymin + 0.0, ymin + aspect*max};
            nicefig::bnd_t ub1{1.4*max, 1.4*max + max,  ymin + 0.0, ymin + aspect*max};
            int rd = mx / 10;
            nicefig::bnd_t ux{-0.5, 32.5, -double(rd), double(mx)+rd};
            nicefig::pen_t p0; p0.style = nicefig::solid_style;
            nicefig::pen_t p1 = p0;
            p0.color = {0.0,0.0,0.8};
            p1.color = {0.8,0.0,0.0};
            nicefig::window_t u0(ub0, ux);
            nicefig::window_t u1(ub1, ux);
            
            u0.add(d0, p0);
            u1.add(d1, p1);
            
            auto ls0 = getls(d0.ydat);
            auto ls1 = getls(d1.ydat);
            
            statfile << "Fetches (w0): " << ls0.size() << "\n";
            statfile << "Fetches (w1): " << ls1.size() << "\n";
            
            total_fetch += ls0.size();
            total_fetch += ls1.size();
            
            nicefig::pen_t xlp;
            xlp.style = nicefig::dash_style;
            for (const auto e0: ls0) u0.add_yline(e0, xlp);
            for (const auto e0: ls1) u1.add_yline(e0, xlp);
            
        
            u0.axis_label(0).enabled = true;
            u0.axis_label(0).text    = "Load offset (relative)";
            u0.axis_label(0).offset  = 0.15;
            u0.axis_label(0).set_major_ticks([](int i){return 100*i;});
            u0.axis_label(2).enabled  = true;
            u0.axis_label(2).text    = "Thread id (Warp 0, Load " + std::to_string(lid) + ", " + std::to_string(ls0.size()) + " fetches)";
            u0.axis_label(2).set_major_ticks([](int i){return i;});
            
            u1.axis_label(0).enabled = true;
            u1.axis_label(0).text    = "Load offset (relative)";
            u1.axis_label(0).offset  = 0.15;
            u1.axis_label(0).set_major_ticks([](int i){return 100*i;});
            u1.axis_label(2).enabled  = true;
            u1.axis_label(2).text    = "Thread id (Warp 1, Load " + std::to_string(lid) + ", " + std::to_string(ls1.size()) + " fetches)";
            u1.axis_label(2).set_major_ticks([](int i){return i;});
            
            
            // nicefig::tikz_fontsize_t font{7};
            // double offset = 0.03*max;
            // nicefig::point_t x_leg{0.5*(ub[0] + tb[1]), ub[3]+offset};
            // auto leg = u.create_legend({"DNS", "$T_0$", "$T_1$"}, nicefig::anchor_south, x_leg);
            
            // leg.linelength = 0.4;
            
            nicefig::pen_t bpen{1.75, nicefig::solid_style, {0,0,0}};
            
            u0.boxpen = bpen;
            u1.boxpen = bpen;
        
            fig << u0;
            fig << u1;
            // fig << leg;
        }
        
        statfile << "Total num fetch: " << total_fetch << "\n";
    }
}
