#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "qap.hpp"
#include "util.hpp"
#include "experiments/experiment.hpp"


void function(){}

int main(int argc, char** argv){
    Experiment* exp = new Experiment();
    int ret = exp->parse_arguments(argc, argv);
    if(ret == 1){
        return ret;
    }

    srand(time(0));

    return exp->run();

    // std::cout<<"Hello world!\n";

    // std::string filepath = ".\\qap\\qapdatsol\\";
    // std::string instance = "lipa60b";

    // QAP_Problem* problem = new QAP_Problem(filepath, instance);

    // const unsigned int length = 100;
    // int* array = new int[length];

    // generate_random_permutation(array, length);

    // for(int i = 0;i<length;i++){
    //     std::cout<<array[i]<<" ";
    // }
    // std::cout<<'\n';

    // unsigned int max = 10;
    // for(int i = 0;i<100;i++){
    //     unsigned int x = generate_random_number(max);
    //     unsigned int y = generate_different_random_number(x,max);
    //     std::cout<<x<<' '<<y<<'\n';
    // }

    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point end;
    // int iterations = 0;
    // int time = 10;
    // do{
    //     function();
        
    //     iterations++;
    //     end = std::chrono::steady_clock::now();
    // }while(false);

    // delete array;
}