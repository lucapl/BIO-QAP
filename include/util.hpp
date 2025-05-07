#ifndef UTIL_HPP
#define UTIL_HPP
#include <cstdlib>

unsigned int generate_random_number(const unsigned int max);

double generate_random_double();

void generate_random_permutation(int* array, const unsigned int length);

void shuffle(int* array, const unsigned int length);

unsigned int generate_different_random_number(unsigned int first_number, unsigned int max);

void copy_array(int* array_to, int* copied_array, unsigned int length);

void swap(int* array, unsigned int i, unsigned int j);

class EarlyStopper{
    private:
        unsigned int no_improv_iters;
        unsigned int iter;
        unsigned int best_value;
    public:
        EarlyStopper(unsigned int no_improv_iters);
        bool should_stop(unsigned int value);
};

#endif