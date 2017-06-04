#ifndef ALGOR_BERTH_ALLOCATION_SIMU_ANNEAL_HPP
#define ALGOR_BERTH_ALLOCATION_SIMU_ANNEAL_HPP

#include "vessel_info.hpp"
#include "sol_info.hpp"
#include "std_inc.hpp"
#include "time_first_fit.hpp"

using namespace std;

double lf_rand() {
    return rand() / (RAND_MAX + 1.0001);
}

int ld_rand(int mod) {
    return rand() % mod;
}

//We can ignore
void next_state(vector<vessel_info> &viv, int shift) {
    int len = viv.size();
    int u = shift + ld_rand(len - shift), v = shift + ld_rand(len - shift);

    while (u == v)
        u = shift + ld_rand(len - shift), v = shift + ld_rand(len - shift);
    swap(viv[u], viv[v]);

}

sol_info simu_anneal(vector<vessel_info> viv, size_t max_berth, size_t max_time,
                     int shift = 0, double init_temp = 2.3e3, double fall_exp = 0.999,
                     double end_temp = 1e-32, int parallel = 5) {
    sol_info approx_si = time_first_fit(viv, max_berth, max_time);
    sol_info tmp_si;
    vector<vessel_info> tmp_viv;
    vector<vector<vessel_info>> para_vi(parallel, viv);
    vector<sol_info> para_si(parallel, approx_si);

    for (double e = init_temp; e > end_temp; e *= fall_exp) {
        for (int i = 0; i < parallel; ++i) {
            tmp_viv = para_vi[i];
            next_state(tmp_viv, shift);
            tmp_si = time_first_fit(tmp_viv, max_berth, max_time);

            if (tmp_si < para_si[i] || exp(-(tmp_si.eval.score - para_si[i].eval.score) / e) > lf_rand())
                para_vi[i] = tmp_viv, para_si[i] = tmp_si;
        }
    }

    for (int i = 0; i < parallel; ++i)
        if (para_si[i] < approx_si)
            approx_si = para_si[i];

    return approx_si;
}

#endif //ALGOR_BERTH_ALLOCATION_SIMU_ANNEAL_HPP
