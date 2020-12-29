#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

struct pipe_mutex {
    int fd[2];
    bool buffer = false;


    pipe_mutex() {
        int r = pipe(this->fd);
        write(fd[1], (void*)(&buffer), sizeof(bool));
    }

    void lock() {
        while(read(fd[0], (void*)(&buffer), sizeof(bool) == 0)) {}
    }

    void unlock() {
        write(fd[1], (void*)(&buffer), sizeof(bool));
    }
};


struct mutex_pipe {
    char bbuf_[BUFSIZ];
    size_t bpos_;
    size_t blen_;

    mutex_pipe() {
        this->bpos_ = 0;
        this->blen_ = 0;
        memset(this->bbuf_, 0, BUFSIZ);
    }

    ssize_t read(char* buf, size_t sz) {
        size_t pos = 0;
        while (pos < sz && (pos == 0 || this->blen_ != 0)) {
            if(this->blen_ != 0) {
                buf[pos] = this->bbuf_[this->bpos_];
                ++this->bpos_;
                this->bpos_ = this->bpos_ % BUFSIZ;
                --this->blen_;
                ++pos;
            }
        }
        return pos;
    }

    ssize_t write(const char* buf, size_t sz) {
        size_t pos = 0;
        while(pos < sz && (pos == 0 || this->blen_ < BUFSIZ)) {
            if(this->blen_ != BUFSIZ) {
                size_t bindex = this->bpos_ + this->blen_;
                bindex = bindex % BUFSIZ;
                this->bbuf_[bindex] = buf[pos];
                ++this->blen_;
                ++pos;
            }
        }
        return pos;
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
