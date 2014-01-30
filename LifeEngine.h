#ifndef LIFEENGINE_H
#define LIFEENGINE_H
#include <list>
#include <vector>
#include <thread>
typedef unsigned int uint;

#define ASYNC_CALC_NEXT

class LifeEngine{
private:
    std::vector<std::vector<bool>>* field;
    std::list<std::vector<std::vector<bool>>*> log;
    unsigned int size_x;
    unsigned int size_y;
    unsigned int max_log_length;

public:
    LifeEngine(unsigned int _size_x, unsigned int _size_y){
        size_x = _size_x;
        size_y = _size_y;
        field = createField();
        max_log_length = 50;
    }

public:
#ifdef ASYNC_CALC_NEXT
    void threadFoo(std::vector<std::vector<bool>>* last_field, uint start, uint len){
        if(start + len >= size_x - 1)
            len = size_x - start - 1;
        for(uint x = start; x < start + len; ++x){
            for(uint y = 1; y < size_y - 1; ++y){
                (*field)[x][y] = false;
                int nc = calculateNeighbor(x, y);
                if((!(*last_field)[x][y]) && nc == 3)
                    (*field)[x][y] = true;
                if(((*last_field)[x][y])){
                    if(nc == 3 || nc == 2)
                        (*field)[x][y] = true;
                    else
                        (*field)[x][y] = false;
                }
            }
        }
    }
    void calculateNext(){
        std::vector<std::vector<bool>>* last_field = *log.rbegin();
        std::list<std::thread> threads;
        uint block_len = 5;
        for(uint x = 1; x < size_x - 1; x += block_len){
            std::thread t(&LifeEngine::threadFoo, this, last_field, x, block_len);
            threads.push_back(std::move(t));
        }
        for(auto iter = threads.begin(); iter != threads.end(); ++iter){
            iter->join();
        }
    }
#else
    void calculateNext(){
        std::vector<std::vector<bool>>* last_field = *log.rbegin();
        uint block_len = 5;
        for(uint x = 1; x < size_x - 1; ++x){
            for(uint y = 1; y < size_y - 1; ++y){
                (*field)[x][y] = false;
                int nc = calculateNeighbor(x, y);
                if((!(*last_field)[x][y]) && nc == 3)
                    (*field)[x][y] = true;
                if(((*last_field)[x][y])){
                    if(nc == 3 || nc == 2)
                        (*field)[x][y] = true;
                    else
                        (*field)[x][y] = false;
                }
            }
        }
    }
#endif

    int calculateNeighbor(int x, int y){
        std::vector<std::vector<bool>>* last_field = *log.rbegin();
        int res = 0;
        for(int i = x - 1; i <= x + 1; ++i)
            for(int j = y - 1; j <= y + 1; ++j)
                if((*last_field)[i][j])
                    ++res;
        if((*last_field)[x][y])
            --res;
        return res;
    }

    std::vector<std::vector<bool>>* createField(){
        std::vector<std::vector<bool>>* m = new std::vector<std::vector<bool>>(size_x);
        for(uint i = 0; i < m->size(); ++i){
            (*m)[i] = std::move(std::vector<bool>(size_y));
            for(int j = 0; j < (*m)[i].size(); ++j){
                (*m)[i][j] = false;
            }
        }
        return m;
    }

    std::vector<std::vector<bool>>* next(){
        if(log.size() < max_log_length){
            log.push_back(field);
        } else {
            delete (*log.begin());
            log.pop_front();
            log.push_back(field);
        }
        field = createField();
        calculateNext();
        return field;
    }

    std::vector<std::vector<bool>>* get_field(){
        return field;
    }

    void print(){
        for(uint x = 1; x < size_x - 1; ++x){
            for(uint y = 1; y < size_y - 1; ++y){
                std::cout << ((*field)[x][y] == true ? 1 : 0);
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

};

#endif
