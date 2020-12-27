//
// Created by john on 19/12/20.
//

#include "Tools.h"

unsigned long getSeed(){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return (spec.tv_sec*1000000000 + spec.tv_nsec);
}

int randTool(int min, int max){
    srand(getSeed());
    return rand() % (max-min + 1) + min ;
}

void waiting(int sec, int milli){
    struct timespec waiting, _waiting;
    waiting.tv_sec = sec;
    waiting.tv_nsec = milli * 100000L;
    nanosleep(&waiting, &_waiting);

}