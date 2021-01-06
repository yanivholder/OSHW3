#ifndef _QUEUEL_H
#define _QUEUEL_H
#include "Headers.hpp"

// Single Producer - Multiple Consumer queue
template <typename T>
class PCQueue
{
public:
    PCQueue(){
        this->q = std::queue<T>();
        pthread_mutex_init(&this->m, NULL);
        pthread_cond_init(&this->c, NULL);
    }

    ~PCQueue(){
        pthread_mutex_destroy(&this->m);
        pthread_cond_destroy(&this->c);
    }

    T pop(){
        pthread_mutex_lock(&this->m);
        while(this->q.empty()){
            pthread_cond_wait(&this->c, &this->m);
        }
        T to_return = this->q.front();
        this->q.pop();
        pthread_mutex_unlock(&this->m);
        return to_return;
    }

    void push(const T item){
        pthread_mutex_lock(&this->m);
        this->q.push(item);
        pthread_cond_signal(&this->c);
        pthread_mutex_unlock(&this->m);
    }


private:
    // Add your class memebers here
    std::queue<T> q;
    pthread_mutex_t m;
    pthread_cond_t c;
};
// Recommendation: Use the implementation of the std::queue for this exercise
#endif