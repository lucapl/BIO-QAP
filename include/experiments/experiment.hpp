#ifndef EXPERIMENT_HPP_
#define EXPERIMENT_HPP_
#define DEBUG false
#include "qap.hpp"
#include <chrono>

enum SolverType {
    LOCAL_SEARCH,
    RANDOM_WALK,
    RANDOM_SEARCH,
    HEURISTIC,
    INVALID,
    TABU_SEARCH,
    SIMULATED_ANNEALING,
    NONE
};

struct TS_args{
    unsigned int no_improv_iters;
    double tabu_tenure;
    double top_percent;
    double quality_drop_limit;
};

struct SA_args{
    unsigned int no_improv_iters;
    double temperature;
    double temperature_decrease;
    double chain_lenght_percent;
};

class Experiment{
    private:
        QAP_Problem* problem;
        int repetitions;
        SolverType selected_solver;
        std::chrono::nanoseconds duration;
        bool is_ls_steepest;
        TS_args* ts_args;
        SA_args* sa_args;

    public:
        int parse_arguments(int argc, char** argv);
        int run();
};

#endif