//
// Created by eilon on 06/01/2021.
//

#ifndef WET_ARGS_H
#define WET_ARGS_H

#include "../Part1/Semaphore.hpp"
#include "../Part1/PCQueue.hpp"
#include "Job.hpp"


class GOL_Args{
public:
    GOL_Args(void (*the_func)(bool, int, int), PCQueue<job>& pcq, int num_of_gens, vector<double>& m_tile_hist, Semaphore& sem):
            func(the_func), assignments(pcq), num_of_gens(num_of_gens), m_tile_hist(m_tile_hist), s(sem){}
    ~GOL_Args(){};
    void (*func)(bool, int, int);
    PCQueue<job>& assignments;
    int num_of_gens;
    vector<double>& m_tile_hist;
    Semaphore& s;
};

#endif //WET_ARGS_H
