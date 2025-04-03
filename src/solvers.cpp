#include "solvers.hpp"
#define VALIDATE false

Neighbourhood::Neighbourhood(unsigned int n){
    length = ((n*n)-n)/2;
    order = new int[length];
    generate_random_permutation(order, length);
    pairs = new unsigned int*[length];
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            unsigned int k = (i * (2 * n - i - 1)) / 2 + (j - i - 1);
            pairs[k] = new unsigned int[2];
            pairs[k][0] = i;
            pairs[k][1] = j;
        }
    }
}

Neighbourhood::~Neighbourhood(){
    delete order;
    for(int i = 0; i < length; i++){
        delete pairs[i];
    }
    delete pairs;
}

unsigned int Neighbourhood::get(unsigned int k, unsigned int l){
    return pairs[order[(k+offset)%length]][l];
}

unsigned int Neighbourhood::first(unsigned int k){
    return get(k,0);
}

unsigned int Neighbourhood::second(unsigned int k){
    return get(k,1);
}

unsigned int Neighbourhood::get_length(){
    return length;
}

void Neighbourhood::shuffle(){
    offset = generate_random_number(length);
}

void local_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, bool is_steepest){
    Neighbourhood* neighbourhood = new Neighbourhood(problem->get_n());
    int best_delta;
    int delta;
    unsigned int i, j;
    unsigned int best_i, best_j;
    bool improvement;
    stats->evaluations = 0;
    stats->steps = 0;
    do{
        improvement = false;
        best_delta = 0;
        // check neighbourhood
        for(unsigned int k = 0; k < neighbourhood->get_length(); k++){
            i = neighbourhood->first(k);
            j = neighbourhood->second(k);
            delta = problem->calculate_solution_value_change(out_solution,i,j);
            stats->evaluations++;
            if(delta < best_delta){ // not in local optimum
                best_i = i;
                best_j = j;
                improvement = true;
                best_delta = delta;
                if (!is_steepest) break; // end if greedy
            }
        }
        if(improvement){
            swap(out_solution, best_i, best_j);
        }
        neighbourhood->shuffle();
        stats->steps++;
    }while(improvement); // hits local optimum
    delete neighbourhood;
}

void random_walk(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, std::chrono::nanoseconds duration){
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    QAP_Solution best_solution = new int[problem->get_n()];
    //copy_array(best_solution, out_solution, problem->get_n());
    //generate_random_permutation(out_solution, problem->get_n());
    unsigned int current_value = problem->calculate_solution_value(out_solution);
    unsigned int best_value = current_value;
    stats->evaluations = 1;
    unsigned int i, j;
    stats->steps = 0;
    do{
        i = generate_random_number(problem->get_n());
        j = generate_different_random_number(i, problem->get_n());
        current_value += problem->calculate_solution_value_change(out_solution,i,j);
        stats->evaluations++;
        swap(out_solution, i, j);
        
        // current_value += delta;
        if(current_value < best_value){
            copy_array(best_solution, out_solution, problem->get_n());
            best_value = current_value;
        }
        end = std::chrono::high_resolution_clock::now();
        stats->steps++;
    }while(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) < duration);
    copy_array(out_solution, best_solution, problem->get_n());
    delete best_solution;
}

void random_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, std::chrono::nanoseconds duration){
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    QAP_Solution best_solution = new int[problem->get_n()];
    //copy_array(best_solution, out_solution, problem->get_n());
    unsigned int best_value = UINT_MAX;
    unsigned int current_value;
    stats->steps = 0;
    stats->evaluations = 0;
    do{
        current_value = problem->calculate_solution_value(out_solution);
        stats->evaluations++;
        if(current_value < best_value){
            copy_array(best_solution, out_solution, problem->get_n());
            best_value = current_value;
        }
        shuffle(out_solution, problem->get_n());
        end = std::chrono::high_resolution_clock::now();
        stats->steps++;
    }while(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) < duration);
    copy_array(out_solution, best_solution, problem->get_n());
}

void heuristic(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats){

}