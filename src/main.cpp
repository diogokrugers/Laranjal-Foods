#include <iostream>
#include <pthread.h>    // variáveis e funções daqui possuem prefixo 'pthread_'
#include <unistd.h>     // getpid(), gettid(), sleep()
#include <vector>
#include "headers/init_and_destroy.h"

#define NUM_REST 6
std::vector<pthread_mutex_t> pedidos(NUM_REST);
std::vector<pthread_mutex_t> motos(NUM_REST);

#define NUM_ENTREG 4

void* fazerEntrega(void* arg){
    std::cout << "oi eu sou a thread " << gettid() << "!" << std::endl;
    sleep(3);
    pthread_exit(NULL);
}

int main(void){

    std::vector<pthread_t> entregNov(NUM_ENTREG);
    std::vector<pthread_t> entregVet(NUM_ENTREG);

    initMutexes(pedidos);
    initMutexes(motos);

    spawnThreads(entregNov, &fazerEntrega);
    spawnThreads(entregVet, &fazerEntrega);

    joinThreads(entregNov);
    joinThreads(entregVet);

    destroyMutexes(pedidos);
    destroyMutexes(motos);
    
    std::cout << "cheguei no final" << std::endl;

    return 0;
}