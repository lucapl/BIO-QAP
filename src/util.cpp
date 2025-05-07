#include "util.hpp"

unsigned int generate_random_number(const unsigned int max){
    return rand() % (max);
}

void generate_random_permutation(int* array, const unsigned int length){
    for(int i=0;i<length;i++){
        array[i] = i;
    }

    shuffle(array, length);
}

void shuffle(int* array, const unsigned int length){
    unsigned int j;
    for(unsigned int i=length-1;i>1;i--){
        j = generate_random_number(i);
        swap(array, i, j);
    }
}

unsigned int generate_different_random_number(unsigned int first_number, unsigned int max){
    return (first_number + generate_random_number(max - 1) + 1) % max;
}

void copy_array(int* array_to, int* copied_array, unsigned int length){
    for(int i = 0; i < length; i++){
        array_to[i] = copied_array[i];
    }
}

void swap(int* array, unsigned int i, unsigned int j){
    // xor swap! (fancy...)
    array[i] = array[i] ^ array[j];
    array[j] = array[i] ^ array[j];
    array[i] = array[i] ^ array[j];
}


bool EarlyStopper::should_stop(unsigned int value){
    if (value < best_value){
        best_value = value;
        iter = 0;
    }
    iter++;
    return iter >= no_improv_iters;
}

EarlyStopper::EarlyStopper(unsigned int no_improv_iters){
    iter = 0;
    this->no_improv_iters = no_improv_iters;
    best_value = UINT_MAX;
}

double generate_random_double(){
    return (double)(rand()) / RAND_MAX;
}