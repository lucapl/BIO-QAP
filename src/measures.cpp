#include "measures.hpp"

double ratio_measure(unsigned int value,unsigned int optimal_value){
    return ((double)value - (double)optimal_value)/(double)optimal_value;
}