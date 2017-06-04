#ifndef ALGOR_SHIP_PARKING_SOL_INFO_HPP
#define ALGOR_SHIP_PARKING_SOL_INFO_HPP

#include "std_inc.hpp"
#include "vessel_info.hpp"

using namespace std;

struct sol_map {
private:
    vector<vector<int>> alloc_map;

public:
    size_t berth_axis;
    size_t time_axis;

    sol_map(size_t berth_axis, size_t time_axis)
            : berth_axis(berth_axis), time_axis(time_axis) {
        alloc_map.assign(this->berth_axis, {vector<int>(this->time_axis, -1)});
    }

    sol_map(const sol_map &sm)
            : alloc_map(sm.alloc_map),
              berth_axis(sm.berth_axis), time_axis(sm.time_axis) {};

    bool try_alloc(size_t b_idx, size_t b_len, size_t t_idx, size_t t_len) {
        if (b_idx + b_len > berth_axis || t_idx + t_len > time_axis)
            return false;
        for (size_t i = b_idx; i < b_idx + b_len; ++i)
            for (size_t j = t_idx; j < t_idx + t_len; ++j)
                if (~alloc_map[i][j])
                    return false;

        return true;
    }

    void alloc(size_t b_idx, size_t b_len, size_t t_idx, size_t t_len, int num) {
        if (b_idx + b_len > berth_axis || t_idx + t_len > time_axis)
            return;
        for (size_t i = b_idx; i < b_idx + b_len; ++i)
            for (size_t j = t_idx; j < t_idx + t_len; ++j)
                alloc_map[i][j] = num;
    }

    void free(size_t b_idx, size_t b_len, size_t t_idx, size_t t_len) {
        for (size_t i = b_idx; i < b_idx + b_len; ++i)
            for (size_t j = t_idx; j < t_idx + t_len; ++j)
                alloc_map[i][j] = -1;
    }

    sol_map &operator=(const sol_map &sm) {
        this->alloc_map = sm.alloc_map;
        this->berth_axis = sm.berth_axis;
        this->time_axis = sm.time_axis;

        return *this;
    }

    vector<int> &operator[](int b_idx) {
        return alloc_map[b_idx];
    }
};

ostream &operator<<(ostream &os, sol_map &sm) {
    for (int i = 0; i < sm.berth_axis; ++i) {
        for (int j = 0; j < sm.time_axis; ++j)
            os << setw(4) << sm[i][j];
        os << "\n";
    }

    return os;
}

struct sol_info {
    struct eval_info {
        static const int w_vessel = 100;
        static const int w_wait = 2;
        static const int w_departure = 1;

        int unassigned_vessel;
        int total_wait_time;
        int last_departure_time;
        int score;

        eval_info()
                : unassigned_vessel(0), total_wait_time(0),
                  last_departure_time(0), score(0) {}

        eval_info(const eval_info &ei)
                : unassigned_vessel(ei.unassigned_vessel),
                  total_wait_time(ei.total_wait_time),
                  last_departure_time(ei.last_departure_time),
                  score(ei.score) {}

        eval_info &operator=(const eval_info &ei) {
            this->unassigned_vessel = ei.unassigned_vessel;
            this->total_wait_time = ei.total_wait_time;
            this->last_departure_time = ei.last_departure_time;
            this->score = ei.score;

            return *this;
        }
    };

    vector<vessel_info> vessel;
    vector<int> start_time;
    vector<int> start_berth;

    eval_info eval;
    size_t vessel_cnt;

    sol_info() {}

    sol_info(vector<vessel_info> &vessel)
            : vessel(vessel), vessel_cnt(vessel.size()) {
        start_berth.assign(this->vessel_cnt, -1);
        start_time.assign(this->vessel_cnt, -1);
        evaluate();
    }

    sol_info(const sol_info &si)
            : vessel(si.vessel), start_time(si.start_time),
              start_berth(si.start_berth), eval(si.eval), vessel_cnt(si.vessel_cnt) {}

    sol_info &operator=(const sol_info &si) {
        this->vessel = si.vessel;
        this->vessel_cnt = si.vessel_cnt;

        this->start_time = si.start_time;
        this->start_berth = si.start_berth;
        this->eval = si.eval;

        return *this;
    }

    eval_info &evaluate() {
        eval.unassigned_vessel = 0;
        eval.total_wait_time = 0;
        eval.last_departure_time = 0;
        eval.score = 0;

        for (size_t i = 0; i < vessel_cnt; ++i) {
            if (start_time[i] == -1 && start_berth[i] == -1)
                ++eval.unassigned_vessel;
            else {
                eval.total_wait_time += start_time[i] - vessel[i].arri_time;
                eval.last_departure_time = max(eval.last_departure_time,
                                               start_time[i] + vessel[i].serv_time);
            }
        }

        eval.score = eval_info::w_departure * eval.last_departure_time +
                     eval_info::w_vessel * eval.unassigned_vessel +
                     eval_info::w_wait * eval.total_wait_time;

        return eval;
    }
};

bool operator<(sol_info &a, sol_info &b) {
    return a.evaluate().score < b.evaluate().score;
}

bool operator<(const sol_info::eval_info &a, const sol_info::eval_info &b) {
    return a.score < b.score;
}

ostream &operator<<(ostream &os, const sol_info &si) {
    vector<int> vi_idx(si.vessel_cnt);

    for (int i = 0; i < si.vessel_cnt; ++i)
        vi_idx[si.vessel[i].index] = i;

    os << "pair(berth,time): ";
    for (int i = 0; i < si.vessel_cnt; ++i) {
        os << si.start_berth[vi_idx[i]] << "," << si.start_time[vi_idx[i]];
        if (i != si.vessel_cnt - 1)
            os << ";";
    }
    os << "\ntotal_wait_time: "
       << si.eval.total_wait_time
       << "\nlast_departure_time: "
       << si.eval.last_departure_time
       << "\nunassigned_vessel: "
       << si.eval.unassigned_vessel
       << "\nscore: "
       << si.eval.score;

    return os;
}

#endif //ALGOR_SHIP_PARKING_SOL_INFO_HPP
