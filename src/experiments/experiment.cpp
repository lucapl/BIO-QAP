#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <map>
#include "util.hpp"
#include "solvers.hpp"
#include "experiments/experiment.hpp"


SolverType get_solver_type(const std::string& solver_name) {
    static std::map<std::string, SolverType> solver_map = {
        {"local_search", LOCAL_SEARCH},
        {"random_walk", RANDOM_WALK},
        {"random_search", RANDOM_SEARCH},
        {"heuristic", HEURISTIC},
        {"simulated_annealing", SIMULATED_ANNEALING},
        {"tabu_search", TABU_SEARCH}
    };

    auto it = solver_map.find(solver_name);
    return (it != solver_map.end()) ? it->second : INVALID;
};

void print_usage_message(){
    std::cout << 
    "Usage: <instance_name:string> <solver_type:string> <repetitions:int> <solver_args...>" << 
    "\n\tOptional: -h --help - show usage" <<
    "\n\tOptional: -p --data_path <string> - path to instance folders" <<
    "\n\nOutput is formatted as:" <<
    "\n\tinstance_name instance_size optimal_value" <<
    "\n\toptimal_solution" <<
    "\n\trepetitions" <<
    "\nThen <repetitions> lines of" <<
    "\n\tinitial_solution" << 
    "\n\tfinal_solution" <<
    "\n\tstarting_value final_value"<<
    "\n\tevaluations steps execution_time"<< std::endl;
}

bool is_non_pos_argument(char* argument, char* short_name, char* full_name){
    return strcmp(argument, short_name)==0 || strcmp(argument, full_name)==0;
}

int Experiment::parse_arguments(int argc, char** argv){
    if (argc < 3){
        print_usage_message();
        return 1;
    }
    char* instance_name = NULL;
    selected_solver = NONE;
    repetitions = -1;
    char* data_path = ".\\qap\\qapdatsol\\";
    long long duration_long = 0;
    bool duration_not_init = true;
    bool ls_not_init = true;
    sa_args = new SA_args;
    ts_args = new TS_args;

    for(int i = 1; i < argc;i++){
        if(is_non_pos_argument(argv[i], "-h", "--help")){
            print_usage_message();
            return 1;
        }
        if(is_non_pos_argument(argv[i], "-p", "--data_path")){
            data_path = argv[++i];
            continue;
        }
        if (instance_name == NULL){
            instance_name = argv[i];
            continue;
        }
        if (selected_solver == NONE){
            std::string solvers_str(argv[i]);
            selected_solver = get_solver_type(solvers_str);
            if (selected_solver == INVALID){
                std::cerr << "Invalid solver name: "+solvers_str<< std::endl;
                return 1;
            }
            continue;
        }
        if(repetitions == -1){
            repetitions = atoi(argv[i]);
            if (repetitions < 1){
                std::cerr << "Invalid number of repetitions: "+std::string(argv[i])<< std::endl;
                return 1;
            }
            continue;
        }
        if(selected_solver == LOCAL_SEARCH && ls_not_init){
            is_ls_steepest = (strcmp("steepest",argv[i]) == 0);
            ls_not_init = false;
            continue;
        }
        if((selected_solver == RANDOM_WALK || selected_solver == RANDOM_SEARCH) && duration_not_init){
            duration_long = std::stoll(argv[i]);
            duration = std::chrono::nanoseconds(duration_long);
            duration_not_init = false;
            continue;
        }
        if (selected_solver == TABU_SEARCH && i + 3 < argc) {
            ts_args->no_improv_iters     = std::stoul(argv[i++]);
            ts_args->tabu_tenure         = std::stod(argv[i++]);
            ts_args->top_percent         = std::stod(argv[i++]);
            ts_args->quality_drop_limit  = std::stod(argv[i++]);
            continue;
        }

        if (selected_solver == SIMULATED_ANNEALING && i + 3 < argc) {
            sa_args->no_improv_iters         = std::stoul(argv[i++]);
            sa_args->temperature             = std::stod(argv[i++]);
            sa_args->temperature_decrease    = std::stod(argv[i++]);
            sa_args->chain_lenght_percent    = std::stod(argv[i++]);
            continue;
        }
    }

    std::string instance_str(instance_name);
    std::string data_path_str(data_path);

    try{
        problem = new QAP_Problem(data_path_str, instance_str);
    } catch(std::invalid_argument &e){
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int Experiment::run(){
    QAP_Solution solution = new int[problem->get_n()];

    std::cout << problem->get_name() << ' ' << problem->get_n() << ' ' << problem->get_optimal_value() << std::endl; 
    print_solution(problem->get_optimal_solution(), problem->get_n());
    std::cout << repetitions << std::endl;
    SolverStats stats = SolverStats();
    for (int i = 0; i < repetitions; i++){
        generate_random_permutation(solution, problem->get_n());
        unsigned int initial_value = problem->calculate_solution_value(solution);
        print_solution(solution, problem->get_n());

        auto start = std::chrono::high_resolution_clock::now();
        switch (selected_solver) {
            case LOCAL_SEARCH:
                local_search(solution, problem, &stats, is_ls_steepest);
                break;
            case RANDOM_WALK:
                random_walk(solution, problem, &stats, duration);
                break;
            case RANDOM_SEARCH:
                random_search(solution, problem, &stats, duration);
                break;
            case HEURISTIC:
                heuristic(solution, problem, &stats);
                break;
            case TABU_SEARCH:
                tabu_search(solution, problem, &stats, ts_args->no_improv_iters, ts_args->tabu_tenure, ts_args->top_percent, ts_args->quality_drop_limit);
                break;
            case SIMULATED_ANNEALING:
                simulated_annealing(solution, problem, &stats, sa_args->no_improv_iters, sa_args->temperature, sa_args->temperature_decrease, sa_args->chain_lenght_percent);
                break;
            case INVALID:
                std::cerr << "Invalid solver type!" << std::endl;
                return 1;
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        unsigned int end_value = problem->calculate_solution_value(solution);
        print_solution(solution, problem->get_n());
        std::cout << initial_value << ' ' << end_value << std::endl;
        std::cout << stats.evaluations << ' '<< stats.steps << ' ' << duration.count() << std::endl;
    }
    delete solution;

    return 0;
}

