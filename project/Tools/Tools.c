//
// Created by john on 19/12/20.
//

#include "Tools.h"

unsigned long getSeed(){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return (spec.tv_sec*1000000000 + spec.tv_nsec);
}