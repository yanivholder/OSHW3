//
// Created by eilon on 27/12/2020.
//
#include "PCQueue.hpp"

template <typename T>
PCQueue<T>::PCQueue(){
    this->q = std::queue<T>();
    pthread_mutex_init(&this->m, NULL);
    pthread_cond_init(&this->c, NULL);
}

template <typename T>
PCQueue<T>::~PCQueue(){
    pthread_mutex_destroy(&this->m);
    pthread_cond_destroy(&this->c);
}

template <typename T>
void PCQueue<T>::push(const T& item){

    pthread_mutex_lock(&this->m);
    this->q.enqueue(item);
    pthread_cond_signal(&this->c);
    pthread_mutex_unlock(&this->m);
}

template <typename T>
T PCQueue<T>::pop(){

    pthread_mutex_lock(&this->m);
    while(this->q.empty()){
        pthread_cond_wait(&this->c, &this->m);
    }
    T to_return = this->q.front();
    this->q.pop();
    pthread_mutex_unlock(&this->m);
    return to_return;
}
