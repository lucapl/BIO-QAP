#ifndef EXPERIMENT_HPP_
#define EXPERIMENT_HPP_
#include "qap.hpp"
#include <chrono>

enum SolverType {
    LOCAL_SEARCH,
    RANDOM_WALK,
    RANDOM_SEARCH,
    HEURISTIC,
    INVALID,
    SUMILATED_ANNEALING,
    NONE
};

class Experiment{
    private:
        QAP_Problem* problem;
        int repetitions;
        SolverType selected_solver;
        std::chrono::nanoseconds duration;
        bool is_ls_steepest;

    public:
        int parse_arguments(int argc, char** argv);
        int run();
};

#endif