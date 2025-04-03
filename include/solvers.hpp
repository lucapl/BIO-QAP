#ifndef SOLVERS_HPP
#define SOLVERS_HPP
#include <chrono>
#include "qap.hpp"
#include "util.hpp"


class Neighbourhood{
    private:
        unsigned int** pairs;
        int* order;
        unsigned int length; 
        unsigned int offset;
    public:
        Neighbourhood(unsigned int length);
        ~Neighbourhood();
        unsigned int first(unsigned int k);
        unsigned int second(unsigned int k);
        unsigned int get(unsigned int k, unsigned int l);
        unsigned int get_length();
        void shuffle();
};

struct SolverStats{
    unsigned int steps;
    unsigned int evaluations;
};

void local_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, bool is_steepest);
void random_walk(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, std::chrono::nanoseconds duration);
void random_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, std::chrono::nanoseconds duration);
void heuristic(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats);

#endif