#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "qap.hpp"
#include "util.hpp"

int main(){
    srand(time(0));
    std::string filepath = ".\\qap\\qapdatsol\\";
    std::string instance = "bur26b";
    QAP_Problem* problem = new QAP_Problem(filepath, instance);
    QAP_Solution solution = new int[problem->get_n()];

    for (int k = 0; k<10;k++){
        generate_random_permutation(solution, problem->get_n());
        unsigned int initial_value = problem->calculate_solution_value(solution);

        unsigned int i = generate_random_number(problem->get_n());
        unsigned int j = generate_different_random_number(i, problem->get_n());
        int delta = problem->calculate_solution_value_change(solution, i, j);

        int temp = solution[i];
        solution[i] = solution[j];
        solution[j] = temp;

        unsigned int end_value = problem->calculate_solution_value(solution);
        int actual_delta = (int)(end_value - initial_value);

        std::cout << "before " << initial_value << " after " << end_value << std::endl;
        std::cout << "actual diff "<< actual_delta << std::endl;
        std::cout << "calculated diff " << delta << std::endl;
        std::cout << "are equal? " << (actual_delta == delta) << std::endl; 
    }

    delete problem;
    delete solution;
    return 0;
}