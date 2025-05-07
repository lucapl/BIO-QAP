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
void simulated_annealing(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, double temperature, double temperature_decrease, double chain_lenght_percent);

class Tabu_List{
    private:
        unsigned int** tabu_list;
        unsigned int tabu_tenure;
        unsigned int length;
    public:
        Tabu_List(QAP_Problem* problem, unsigned int tabu_tenure);
        ~Tabu_List();
        bool is_tabu(unsigned int i, unsigned int j);
        bool is_tabu(Move* move);
        unsigned int get_tabu(unsigned int i, unsigned int j);
        unsigned int get_tabu(Move* move);
        void make_tabu(unsigned int i, unsigned int j);
        void update(unsigned int i, unsigned int j);
};

struct Move{
    unsigned int i;
    unsigned int j;
    int delta;
};

class Candidate_Neighbourhood{
    private:
        QAP_Problem* problem;
        Move** moves;
        Move* best_move;
        int* order;
        unsigned int total_length;
        unsigned int k;
    public:
        Candidate_Neighbourhood(QAP_Problem* problem, double top_percent);
        ~Candidate_Neighbourhood();
        void construct(QAP_Solution solution, unsigned int* locations);
        void reevaluate(QAP_Solution solution, unsigned int* locations, Tabu_List* tabu_list, unsigned int current_value, unsigned int best_value);
        int get_best_delta();
        unsigned int best_first();
        unsigned int best_second();
};

void tabu_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, ...);

#endif