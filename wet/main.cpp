#include <iostream>
#include "PCQueue.hpp"
#include "Semaphore.hpp"


int main() {
    std::cout << "Hello, World!" << std::endl;

    Semaphore *s = new Semaphore();
    PCQueue<int> *p = new PCQueue<int>();
    pid_t pid1 = fork();
    if (pid1 !=0){
        p->pop();
        s->down();
    }else{
        int x = 1;
        int& y = x;
        p->push(y);
        s->up();
    }


    return 0;

}