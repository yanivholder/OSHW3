//
// Created by eilon on 27/12/2020.
//
#include "Semaphore.hpp"


Semaphore::Semaphore(){
    this->counter = 0;
    pthread_mutex_init(&this->m, nullptr);
    pthread_cond_init(&this->c, nullptr);
}

Semaphore::Semaphore(int val){
    this->counter = val;
    pthread_mutex_init(&this->m, nullptr);
    pthread_cond_init(&this->c, nullptr);
}

Semaphore::~Semaphore(){
    pthread_mutex_destroy(&this->m);
    pthread_cond_destroy(&this->c);
}

void Semaphore::up(){

    pthread_mutex_lock(&this->m);
//    assert(counter >= 0);
    this->counter++;
    pthread_cond_signal(&this->c);
    pthread_mutex_unlock(&this->m);
}

void Semaphore::down(){

    pthread_mutex_lock(&this->m);
    while(this->counter<=0){
        pthread_cond_wait(&this->c, &this->m);
    }
//    assert(counter > 0);
    this->counter--;
//    assert(counter >= 0);
    pthread_mutex_unlock(&this->m);
}

int Semaphore::get_val(){
    return this->counter;
}

