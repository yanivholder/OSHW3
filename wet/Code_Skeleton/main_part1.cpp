#include <iostream>
#include "PCQueue.hpp"
#include "Semaphore.hpp"
using std::cout;
using std::endl;


struct arg_struct {
    PCQueue<std::string> q;
    std::string s;
    std::string s2 = "before";
};


void* Fpop(void* args){
    struct arg_struct* arguments = (struct arg_struct*)args;
    arguments->s = arguments->q.pop();
    arguments->s2 = "after";
    return NULL;
}

void* Fpush(void* args){
    struct arg_struct* arguments = (struct arg_struct*)args;
    arguments->q.push(arguments->s);
    arguments->s2 = "mid";
//    PCQueue<std::string>* q = &((struct arg_struct*)args)->q;
//    q->push(((struct arg_struct*)args)->s);

    return NULL;
}



int main2() {
    std::cout << "starting program" << std::endl;

    //PCQueue<std::string>* q = new PCQueue<std::string>();

    Semaphore semph = Semaphore(-5);
    semph.up();

    struct arg_struct args1;
    args1.s = std::string("first");
    args1.q = PCQueue<std::string>();

    struct arg_struct args2;
    args2.s = std::string("second");
    args2.q = PCQueue<std::string>();

    struct arg_struct args3;
    args3.s = std::string("third");
    args3.q = PCQueue<std::string>();

    pthread_t t_up1;
    pthread_t t_up2;
    pthread_t t_up3;

    pthread_t t_down1;
    pthread_t t_down2;
    pthread_t t_down3;

    pthread_create(&t_down1, NULL, Fpop, (void *) &args1);
    pthread_create(&t_down2, NULL, Fpop, (void *) &args2);
    pthread_create(&t_down3, NULL, Fpop, (void *) &args3);

    sleep(1);
    std::cout << "all should be before: (" << args1.s << ", " << args1.s2 << ")" << ", " << "(" << args2.s << ", " << args2.s2 << "), (" << args3.s << ", " << args3.s2 << ")" << std::endl;
    sleep(1);



    pthread_create(&t_up1, NULL, Fpush, (void *) &args1);
    sleep(0);
    sleep(0);
    std::cout << "1 should be mid/after: (" << args1.s << ", " << args1.s2 << ")" << ", " << "(" << args2.s << ", " << args2.s2 << "), (" << args3.s << ", " << args3.s2 << ")" << std::endl;
    sleep(1);
    pthread_create(&t_up2, NULL, Fpush, (void *) &args2);
    pthread_create(&t_up3, NULL, Fpush, (void *) &args3);

    pthread_join(t_down1, NULL);
    pthread_join(t_down2, NULL);
    pthread_join(t_down3, NULL);

    pthread_join(t_up1, NULL);
    pthread_join(t_up2, NULL);
    pthread_join(t_up3, NULL);

    std::cout << "final strings are: (" << args1.s << ", " << args1.s2 << ")" << ", " << "(" << args2.s << ", " << args2.s2 << "), (" << args3.s << ", " << args3.s2 << ")" << std::endl;



//    std::cout << "final val is: " << s->get_val() << std::endl;


    //PCQueue<int> *p = new PCQueue<int>();

    return 0;

}