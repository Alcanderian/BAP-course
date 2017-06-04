#ifndef ALGOR_BERTH_ALLOCATION_PURE_DFS_HPP
#define ALGOR_BERTH_ALLOCATION_PURE_DFS_HPP

#include "vessel_info.hpp"
#include "sol_info.hpp"
#include "std_inc.hpp"
#include "time_first_fit.hpp"

using namespace std;

sol_info pure_dfs(vector<vessel_info> viv, size_t max_berth, size_t max_time) {
    sol_info best_si(viv), tmp_si;

    do {
        tmp_si = time_first_fit(viv, max_berth, max_time);
        best_si = best_si < tmp_si ? best_si : tmp_si;
    } while (next_permutation(viv.begin(), viv.end()));

    return best_si;
}

#endif //ALGOR_BERTH_ALLOCATION_PURE_DFS_HPP
