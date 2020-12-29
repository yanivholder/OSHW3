#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

//######################################################################################
//                       Part ONE
//######################################################################################


struct pipe_mutex {
    int fd[2];
    char char_storage;

    pipe_mutex() {
        int r = pipe(this->fd);
        write(fd[1], "#", sizeof(char));
    }

    void lock() {
        while(read(fd[0], &char_storage, sizeof(char)) != sizeof(char)) {}
    }

    void unlock() {
        write(fd[1], "#", sizeof(char));
    }
};


struct mutex_pipe {
    char bbuf_[BUFSIZ];
    size_t bpos_;
    size_t blen_;
    pthread_mutex_t m;

    mutex_pipe() {
        this->bpos_ = 0;
        this->blen_ = 0;
        memset(this->bbuf_, 0, BUFSIZ);
    }

    ssize_t read(char* buf, size_t sz) {
        size_t pos = 0;
        pthread_mutex_lock(&this->m);
        while (pos < sz && (pos == 0 || this->blen_ != 0)) {
            if(this->blen_ != 0) {
                buf[pos] = this->bbuf_[this->bpos_];
                ++this->bpos_;
                this->bpos_ = this->bpos_ % BUFSIZ;
                --this->blen_;
                ++pos;
            }
        }
        pthread_mutex_unlock(&this->m);
        return pos;
    }

    ssize_t write(const char* buf, size_t sz) {
        size_t pos = 0;
        pthread_mutex_lock(&this->m);
        while(pos < sz && (pos == 0 || this->blen_ < BUFSIZ)) {
            if(this->blen_ != BUFSIZ) {
                size_t bindex = this->bpos_ + this->blen_;
                bindex = bindex % BUFSIZ;
                this->bbuf_[bindex] = buf[pos];
                ++this->blen_;
                ++pos;
            }
        }
        pthread_mutex_unlock(&this->m);
        return pos;
    }
};

//######################################################################################
//                       Part TWO
//######################################################################################

typedef struct singlephore {
    int value;
} singlephore;

void singlephore_init(singlephore* h) {
    h->value = 0;
}

void H(singlephore* h, int bound, int delta) {
    while(h->value < bound) {
        sched_yield();
    }
    h->value += delta;
}

typedef struct mutex {
    singlephore h;
} mutex;

void mutex_init(mutex* m) {
    singlephore_init(&m->h); // h->value == 0 means open
}

void mutex_lock(mutex* m) {
    H(&m->h, -1, -2); // if h->value < -1 (locked), wait
                                  // else, h->value == 0 so set h->value = -2 (locked)
}

void mutex_unlock(mutex* m) {
    H(&m->h, -100, 2); // if h->value < -100 (never), wait.
                                   // else (always), h->value == -2 so set h->value = 0 (open)
}

typedef struct condvar {
    mutex m;
    singlephore h;
//    bool no_signals_pending;
//    bool is_someone_waiting;
} condvar;

void cond_init(condvar* c) {
//    c->no_signals_pending = true;
//    c->is_someone_waiting = false;
    mutex_init(&c->m);
    singlephore_init(&c->h);
}

void cond_signal(condvar* c) {
    mutex_lock(&c->m);
    H(&c->h, 0, 1);
//    if(c->no_signals_pending && c->is_someone_waiting) {
//        c->no_signals_pending = false;
//    }
    mutex_unlock(&c->m);
}

void cond_wait(condvar* c, mutex* m) {
    mutex_unlock(m);
    // We assume that in a real H implementation the test and increment are atomic
    H(&c->h, INT32_MIN, -1);
//    mutex_lock(&c->m);
//    c->is_someone_waiting = true;
//    while (c->no_signals_pending) {
//        mutex_unlock(&c->m);
//        sched_yield();
//        mutex_lock(&c->m);
//        c->is_someone_waiting = true;
//    }
//    c->no_signals_pending = true;
//    mutex_lock(m);
//    c->is_someone_waiting = false;
//    mutex_unlock(&c->m);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
