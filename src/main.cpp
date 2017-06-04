#include "std_inc.hpp"
#include "vessel_info.hpp"
#include "sol_info.hpp"
#include "simu_anneal.hpp"
#include "pure_dfs.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    string postfix;
    while (1) {
        cout << "input test's postfix.(\"exit\" to exit): ";
        cin >> postfix;
        if (postfix == "exit")
            break;

        //Read test file.
        char path[1000];
        sprintf(path, "../stage/stage_%s.txt", postfix.c_str());
        ifstream test(path);

        if (!test.is_open()) {
            cout << "Invalid stage.\n\n";
            continue;
        }

        vector<vessel_info> viv;
        size_t max_berth, max_time, max_vi;

        test >> max_berth >> max_time >> max_vi;

        for (int i = 0; i < max_vi; ++i) {
            int index, arri, serv, berth;
            test >> arri >> serv >> berth;
            viv.push_back(vessel_info(i, arri, serv, berth));
        }
        if (test.eof()) {
            cout << "Invalid stage.\n\n";
            continue;
        }
        test.close();

        //Read current best solution.
        int cur_score = sol_info::eval_info::w_vessel * max_vi;
        memset(path, 0, sizeof(path));
        sprintf(path, "../solution/best_%s.txt", postfix.c_str());
        ifstream cur_sol(path);
        string cur_info, tmp_info;
        for (int i = 0; i < 6; ++i) {
            getline(cur_sol, cur_info);
        }
        if (cur_info != "") {
            stringstream ss(cur_info);
            ss >> tmp_info;
            ss >> cur_score;
        }
        cur_sol.close();

        //Get a solution.
        sol_info next_sol;
        int iteration = 1000;
        do {
            uint32_t seed = time(0) * time(0);
            if (max_vi < 10) {
                //Depth first search if N is small enough.
                next_sol = pure_dfs(viv, max_berth, max_time);
                cout << "seed: no seed\n" << next_sol << endl;
            } else {
                //Otherwise use S.A..
                srand(seed);
                next_sol = simu_anneal(viv, max_berth, max_time);
                cout << "seed: " << seed << endl << next_sol << endl;
            }
            cout << endl;

            //Write best solution.
            if (next_sol.eval.score < cur_score) {
                cur_score = next_sol.eval.score;
                ofstream best_sol_so_far(path);
                if (max_vi < 10)
                    best_sol_so_far << "seed: no seed\n" << next_sol << endl;
                else
                    best_sol_so_far << "seed: " << seed << endl << next_sol << endl;
                best_sol_so_far.close();
            }

            //If next_sol.eval.unassigned_vessel == max_vi, that is, no solution.
        } while ((next_sol.eval.unassigned_vessel != 0 && next_sol.eval.unassigned_vessel != max_vi) || --iteration);
    }
}