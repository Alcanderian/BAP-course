#ifndef ALGOR_BERTH_ALLOCATION_TIME_FIRST_FIT_HPP
#define ALGOR_BERTH_ALLOCATION_TIME_FIRST_FIT_HPP

#include "std_inc.hpp"
#include "sol_info.hpp"
#include "vessel_info.hpp"

using namespace std;

void time_first_fit_one_step(sol_info &si, sol_map &sm, int vi_idx) {
    vessel_info vi = si.vessel[vi_idx];
    si.start_berth[vi_idx] = -1;
    si.start_time[vi_idx] = -1;
    for (int j = vi.arri_time; j + vi.serv_time <= sm.time_axis; ++j) {
        for (int i = 0; i + vi.berth <= sm.berth_axis; ++i) {
            if (sm.try_alloc(i, vi.berth, j, vi.serv_time)) {
                si.start_berth[vi_idx] = i;
                si.start_time[vi_idx] = j;
                sm.alloc(si.start_berth[vi_idx], vi.berth,
                         si.start_time[vi_idx], vi.serv_time,
                         vi.index);

                return;
            }
        }
    }
}

sol_info time_first_fit(vector<vessel_info> &viv, size_t max_berth, size_t max_time) {
    sol_info si(viv);
    sol_map sm(max_berth, max_time);

    for (int i = 0; i < si.vessel_cnt; ++i)
        time_first_fit_one_step(si, sm, i);

    return si;
}

#endif //ALGOR_BERTH_ALLOCATION_TIME_FIRST_FIT_HPP
