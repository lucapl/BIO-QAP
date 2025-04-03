#ifndef QAP_HPP
#define QAP_HPP
#define Matrix2D int**
#define QAP_Solution int*
#include <iostream>
#include <fstream>
#include <string>

class QAP_Problem{
    private:
        Matrix2D A;
        Matrix2D B;
        unsigned int n;
        unsigned int known_optimum_value;
        QAP_Solution known_optimum;
        std::string name;

    public:
        QAP_Problem(std::string filepath, std::string filename);
        ~QAP_Problem();
        unsigned int calculate_solution_value(QAP_Solution solution);
        int calculate_solution_value_change(QAP_Solution solution, unsigned int swap_i, unsigned int swap_j);
        unsigned int get_n();
        std::string& get_name();
        QAP_Solution get_optimal_solution();
        unsigned int get_optimal_value(); 
};

void print_solution(QAP_Solution solution, unsigned int length);

#endif