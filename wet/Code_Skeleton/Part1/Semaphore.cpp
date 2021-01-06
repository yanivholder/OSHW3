//
// Created by eilon on 27/12/2020.
//
#include "Semaphore.hpp"


Semaphore::Semaphore(){
    this->counter = 0;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);
}

Semaphore::Semaphore(unsigned val){
    this->counter = val;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);
}

Semaphore::~Semaphore(){
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
}

void Semaphore::up(){

    pthread_mutex_lock(&m);
    assert(counter >= 0);
    this->counter++;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void Semaphore::down(){

    pthread_mutex_lock(&m);
    while(counter<=0){
        pthread_cond_wait(&c, &m);
    }
    assert(counter > 0);
    this->counter--;
    assert(counter >= 0);
    pthread_mutex_unlock(&m);
}

int Semaphore::get_val(){
    return this->counter;
}

