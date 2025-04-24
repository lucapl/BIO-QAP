#include "qap.hpp"

unsigned int QAP_Problem::get_n(){
    return this->n;
}

unsigned int QAP_Problem::get_optimal_value(){
    return known_optimum_value;
}

int QAP_Problem::get_flow(unsigned int i, unsigned int j){
    return A[i][j];
}

int QAP_Problem::get_cost(unsigned int i, unsigned int j){
    return B[i][j];
}

QAP_Solution QAP_Problem::get_optimal_solution(){
    return known_optimum;
}

unsigned int QAP_Problem::calculate_partial_solution_value(QAP_Solution solution, unsigned int length){
    int value = 0;
    for(int i = 0; i<length;i++){
        for(int j = 0; j<length; j++){
            value += this->A[i][j]*this->B[solution[i]][solution[j]];
        }
    }

    return value;
}

unsigned int QAP_Problem::calculate_last_value_added(QAP_Solution solution, unsigned int length){
    int value = 0;
    for(int i = 0; i<length;i++){
        value += this->A[i][length-1]*this->B[solution[i]][solution[length-1]];
        value += this->A[length-1][i]*this->B[solution[length-1]][solution[i]];
    }

    return value;
}

unsigned int QAP_Problem::calculate_solution_value(QAP_Solution solution){
    int value = 0;
    for(int i = 0; i<this->n;i++){
        for(int j = 0; j<this->n; j++){
            value += this->A[i][j]*this->B[solution[i]][solution[j]];
        }
    }

    return value;
}

int QAP_Problem::calculate_solution_value_change(QAP_Solution solution, unsigned int r, unsigned int s){
    // https://www.cs.put.poznan.pl/mkomosinski/lectures/optimization/extras/QAP-fast-update.pdf
    // assuming assymetry (more general)
    int pi_r = solution[r];
    int pi_s = solution[s];

    int value = A[r][r] * (B[pi_s][pi_s] - B[pi_r][pi_r]);
    value += A[r][s] * (B[pi_s][pi_r] - B[pi_r][pi_s]);
    value += A[s][r] * (B[pi_r][pi_s] - B[pi_s][pi_r]);
    value += A[s][s] * (B[pi_r][pi_r] - B[pi_s][pi_s]);

    for(unsigned int k = 0; k<n; k++){
        if (k == r || k == s){
            continue;
        }
        int pi_k = solution[k];
        value += A[k][r]*(B[pi_k][pi_s]-B[pi_k][pi_r]);
        value += A[k][s]*(B[pi_k][pi_r]-B[pi_k][pi_s]);
        value += A[r][k]*(B[pi_s][pi_k]-B[pi_r][pi_k]);
        value += A[s][k]*(B[pi_r][pi_k]-B[pi_s][pi_k]);
    }

    return value;
}

QAP_Problem::QAP_Problem(std::string filepath, std::string filename){
    std::string full_path = filepath+filename+".dat";
    std::ifstream data_file(full_path);
    if(data_file.fail()){
        throw std::invalid_argument("File not found "+full_path);
    }
    this->name = filename;
    // load size
    data_file >> this->n;

    // load A matrix
    this->A = new int*[this->n];
    for(int i = 0; i<this->n;i++){
        this->A[i] = new int[this->n];
        for(int j = 0; j<this->n; j++){
            data_file >> this->A[i][j];
        }
    }

    // load B matrix
    this->B = new int*[this->n];
    for(int i = 0; i<this->n;i++){
        this->B[i] = new int[this->n];
        for(int j = 0; j<this->n; j++){
            data_file >> this->B[i][j];
        }
    }
    data_file.close();
    std::string solution_file_path = filepath+filename+".sln";
    std::ifstream solution_file(solution_file_path);
    if(solution_file.fail()){
        throw std::invalid_argument("File not found " + solution_file_path);
    }
    int sln_n;
    solution_file >> sln_n;
    if (sln_n != this->n){
        std::cerr << "Warning! Solution size: " << sln_n << " is not equal to instance size: " << this->n << "!" << std::endl; 
    }
    
    solution_file >> this->known_optimum_value;

    this->known_optimum = new int[sln_n];
    for(int i = 0; i<sln_n;i++){
        solution_file >> known_optimum[i];
    }
    solution_file.close();
}

QAP_Problem::~QAP_Problem(){
    for(int i = 0; i<this->n;i++){
        delete this->A[i];
        delete this->B[i];
    }
}

std::string& QAP_Problem::get_name(){
    return name;
}

void print_solution(QAP_Solution solution, unsigned int length){
    if (length == 0){
        std::cout << std::endl;
    }

    std::cout << solution[0];
    for(int i = 1; i<length; i++){
        std::cout << ' ';
        std::cout << solution[i];
    }
    std::cout << std::endl;
}