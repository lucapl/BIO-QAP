#include "solvers.hpp"
#include <algorithm>
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
    delete[] order;
    for(int i = 0; i < length; i++){
        delete[] pairs[i];
    }
    delete[] pairs;
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

Tabu_List::Tabu_List(QAP_Problem* problem, unsigned int tabu_tenure){
    length = problem->get_n()-1;
    tabu_list = new unsigned int*[length];
    for(int i = 0; i < length; i++){
        tabu_list[i] = new unsigned int[length-i];
        for(int j = 0; j < length-i; j++){
            tabu_list[i][j] = 0;
        }
    }
    this->tabu_tenure = tabu_tenure;
}

Tabu_List::~Tabu_List(){
    for(int i = 0; i < length; i++){
        delete[] tabu_list[i];
    }
    delete[] tabu_list;
    // std::cout << "tabu freed" <<std::endl;
}

unsigned int Tabu_List::get_tabu(unsigned int i, unsigned int j){
    unsigned int first, second;
    first = std::min(i,j);
    second = std::max(i,j);
    return tabu_list[first][second-first-1];
}

unsigned int Tabu_List::get_tabu(Move* move){
    return get_tabu(move->i,move->j);
}

bool Tabu_List::is_tabu(unsigned int i, unsigned int j){
    return get_tabu(i,j) > 0;
}

bool Tabu_List::is_tabu(Move* move){
    return is_tabu(move->i, move->j);
}

void Tabu_List::make_tabu(unsigned int i, unsigned int j){
    unsigned int first, second;
    first = std::min(i,j);
    second = std::max(i,j);
    tabu_list[first][second-first-1] = tabu_tenure;
}

void Tabu_List::update(unsigned int i, unsigned int j){
    if (is_tabu(i,j)){
        unsigned int first, second;
        first = std::min(i,j);
        second = std::max(i,j);
        tabu_list[first][second-first-1]--;
    }
}

void Tabu_List::update(Move* move){
    update(move->i,move->j);
}


bool move_comparator(Move* a, Move* b){
    return a->delta < b->delta;
}

Candidate_Neighbourhood::Candidate_Neighbourhood(QAP_Problem* problem, SolverStats* stats, double top_percent){
    this->problem = problem;
    this->stats = stats;
    unsigned int n = problem->get_n();
    total_length = ((n*n)-n)/2;
    k = (unsigned int)std::ceil(n * top_percent);
    moves = new Move*[total_length];
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            unsigned int l = (i * (2 * n - i - 1)) / 2 + (j - i - 1);
            moves[l] = new Move();
            moves[l]->i = i;
            moves[l]->j = j;
        }
    }
}

Candidate_Neighbourhood::~Candidate_Neighbourhood(){
    for(int i = 0; i < total_length; i++){
        delete[] moves[i];
    }
    delete[] moves;
    // std::cout << "cand freed" <<std::endl;
}

void Candidate_Neighbourhood::construct(QAP_Solution solution, unsigned int* locations){
    // std::cout << "Preval" << std::endl;
    for(int i = 0; i < total_length; i++){
        Move* move = moves[i];
        move->delta = problem->calculate_solution_value_change(solution,locations[move->i],locations[move->j]);
        stats->evaluations++;
    }

    // std::cout << "Presort" << std::endl;
    std::sort(moves, moves + total_length, move_comparator);
    // std::cout << "Sorted" << std::endl;
}

void Candidate_Neighbourhood::find_best(Tabu_List* tabu_list, unsigned int current_value, unsigned int best_value){
    Move* best_value_move = nullptr;
    Move* best_least_tabu_move = nullptr;

    for(int i = 0; i < k; i++){
        Move* move = moves[i];

        if (best_value_move == nullptr || move->delta < best_value_move->delta){
            best_value_move = move;
            // std::cout << "better ";
        }

        if (best_least_tabu_move == nullptr || tabu_list->get_tabu(move) < tabu_list->get_tabu(best_least_tabu_move)){
            best_least_tabu_move = move;
        } else if (tabu_list->get_tabu(move) == tabu_list->get_tabu(best_least_tabu_move) &&
            best_least_tabu_move->delta > move->delta){
            best_least_tabu_move = move;
        }
        tabu_list->update(move);
    }

    best_move = best_value_move;

    if(best_value_move->delta + current_value < best_value){
        //std::cout << "Best ever seen!" << std::endl;
        return;
    }

    if(tabu_list->is_tabu(best_value_move)){
        best_move = best_least_tabu_move;
        //std::cout << "Best least tabu!" << std::endl;
    }
}

void Candidate_Neighbourhood::reevaluate(QAP_Solution solution, unsigned int* locations){
    for(int i = 0; i < k; i++){
        Move* move = moves[i];
        move->delta = problem->calculate_solution_value_change(solution,locations[move->i],locations[move->j]);
        stats->evaluations++;
    }
}

int Candidate_Neighbourhood::get_best_delta(){
    return best_move->delta;
}

unsigned int Candidate_Neighbourhood::best_first(){
    return best_move->i;
}

unsigned int Candidate_Neighbourhood::best_second(){
    return best_move->j;
}

void tabu_search(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, unsigned int no_improv_iters, unsigned int tabu_tenure, double top_percent, double quality_drop_limit){
    QAP_Solution current_solution = new int[problem->get_n()];
    copy_array(current_solution, out_solution, problem->get_n());

    EarlyStopper stopper = EarlyStopper(no_improv_iters);
    stats->evaluations = 1;
    unsigned int best_value = problem->calculate_solution_value(out_solution);
    unsigned int current_value = best_value;

    unsigned int* locations = new unsigned int[problem->get_n()];

    Tabu_List tabu_list = Tabu_List(problem, tabu_tenure);
    for(int i = 0; i < problem->get_n(); i++){
        int value = current_solution[i];
        locations[value] = i;
    }

    Candidate_Neighbourhood neighbourhood = Candidate_Neighbourhood(problem, stats, top_percent);

    bool stop = false;
    unsigned int i,j,value_i,value_j, iters;

    unsigned int k = 0;
    stats->steps = 0;
    while(!stop){
        neighbourhood.construct(current_solution, locations);
        iters = 0;
        do{
            neighbourhood.find_best(&tabu_list, current_value, best_value);
            value_i = neighbourhood.best_first();
            value_j = neighbourhood.best_second();
            tabu_list.make_tabu(value_i,value_j);
            i = locations[value_i];
            j = locations[value_j];
            swap(current_solution,i,j);
            locations[value_i] = j;
            locations[value_j] = i;
            current_value += neighbourhood.get_best_delta();
            if(current_value < best_value){
                best_value = current_value;
                copy_array(out_solution, current_solution, problem->get_n());
            }
            stop = stopper.should_stop(current_value);
            iters++;
            stats->steps++;
            neighbourhood.reevaluate(current_solution, locations);
            //std::cout<<neighbourhood.get_best_delta()<< " " << neighbourhood.best_first() << " " << neighbourhood.best_second() << std::endl;
        }while(!stop && ((neighbourhood.get_best_delta()/current_value) >= quality_drop_limit));
        k++;
        //std::cout << k <<std::endl;
    };
}

double accept_prob(int delta, double c){
    if(delta <= 0){
        return 1.0;
    }
    return exp(-((double)delta)/c);
}

double calculate_average_acceptance(int* deltas, unsigned int length, double c){
    double expectation = 0.0;
    for (int i = 0; i < length; i++){
        expectation += accept_prob(deltas[i], c);
    }
    return expectation / length;
}

double select_initial_temperature(QAP_Solution solution, QAP_Problem* problem, Neighbourhood* neigh, SolverStats* stats, double desired_acceptance, double tol){
    // find initial temperature on the neighbourhood of initial solution using secant method
    int* deltas = new int[neigh->get_length()];

    for(int k = 0; k < neigh->get_length(); k++){
        unsigned int i = neigh->first(k);
        unsigned int j = neigh->second(k);
        deltas[k] = problem->calculate_solution_value_change(solution,i,j);
        stats->evaluations++;
    }

    double x0 = 0.0;
    double x1 = 1e3;
    double fx0 = calculate_average_acceptance(deltas,neigh->get_length(),x0) - desired_acceptance;
    double fx1 = calculate_average_acceptance(deltas,neigh->get_length(),x1) - desired_acceptance;
    int i = 0;
    do{
        double x2 = x1 - (fx1 * ((x1-x0)/(fx1-fx0)));
        x0 = x1;
        x1 = x2;
        fx0 = fx1;
        fx1 = calculate_average_acceptance(deltas,neigh->get_length(),x1) - desired_acceptance;
        i++;
    }while(abs(x1-x0) > tol);

    delete deltas;
    return (x0 + x1)/2;
}

unsigned int select_chain_length(QAP_Problem* problem, double alpha){
    unsigned int n = problem->get_n();
    return (((n*n)-n)/2) * alpha;
}

void simulated_annealing(QAP_Solution out_solution, QAP_Problem* problem, SolverStats* stats, unsigned int no_improv_iters, double initial_acceptance, double temperature_decrease, double chain_lenght_percent){
    QAP_Solution current_solution = new int[problem->get_n()];
    copy_array(current_solution, out_solution, problem->get_n());
    unsigned int best_value = problem->calculate_solution_value(out_solution);
    unsigned int current_value = problem->calculate_solution_value(out_solution);
    stats->evaluations = 1;
    Neighbourhood neighbourhood = Neighbourhood(problem->get_n());
    double temperature = select_initial_temperature(current_solution,problem,&neighbourhood,stats,initial_acceptance,1e-4);
    unsigned int chain_length = select_chain_length(problem, chain_lenght_percent);
    EarlyStopper stopper = EarlyStopper(no_improv_iters*chain_length);
    

    int delta;
    unsigned int i, j, k;
    unsigned int iteration;
    bool stop = false;
    iteration = 0;
    stats->steps = 0; // could use instead of iteration
    do{
        for(unsigned int l = 0; l < chain_length; l++){
            k = generate_random_number(neighbourhood.get_length());
            i = neighbourhood.first(k);
            j = neighbourhood.second(k);
            delta = problem->calculate_solution_value_change(current_solution,i,j);
            stats->evaluations++;
            if(accept_prob(delta, temperature) >= generate_random_double()){
                swap(current_solution, i, j);
                current_value += delta;
                stop = stopper.should_stop(current_value);
            }
            if(current_value < best_value){
                best_value = current_value;
                copy_array(out_solution, current_solution, problem->get_n());
            }
            if(stop){
                break;
            }
        }
        temperature *= temperature_decrease;
        stats->steps++;
        iteration++;
    }while(!stop);
    delete current_solution;
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
    unsigned int best_value = UINT_MAX;
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
    stats->evaluations = 0;
    stats->steps = 0;

    bool* assigned = new bool[problem->get_n()];
    for (unsigned int i = 0; i<problem->get_n(); i++){
        assigned[i] = false;
    }

    assigned[out_solution[0]] = true;
    
    for(unsigned int i = 1; i<problem->get_n(); i++){
        int best_j = -1;
        unsigned int best_val = UINT_MAX;
        for(int j = 0; j<problem->get_n(); j++){
            unsigned int value = 0;
            if(assigned[j]){
                continue;
            }
            out_solution[i] = j;
            value = problem->calculate_last_value_added(out_solution, i+1);
            stats->evaluations++;
            if(value < best_val){
                best_j = j;
                best_val = value;
            }
        }
        out_solution[i] = best_j;
        assigned[best_j] = true;
    }

    delete assigned;
}