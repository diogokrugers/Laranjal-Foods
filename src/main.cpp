#include <iostream>
#include <pthread.h>    // variáveis e funções daqui possuem prefixo 'pthread_'
#include <unistd.h>     // getpid(), gettid(), sleep()
#include <vector>
#include <cstring>
#include "headers/init_and_destroy.h"

#define NUM_REST 6
std::vector<pthread_mutex_t> pedidos(NUM_REST);
std::vector<pthread_mutex_t> motos(NUM_REST);

#define NUM_ENTREG 4

void* fazerEntrega(void* tipoEntregador){
    int threadID = (gettid() % NUM_ENTREG) + 1;
    float lucroDiario = 0;
    unsigned int seedLocal = time(NULL) ^ gettid();
    while(true){
            int numRestaurante = rand_r(&seedLocal) % NUM_REST;

            bool isVeterano = strcmp((char*) tipoEntregador, "Veterano") == 0;
            const char* primeiroRecurso = (isVeterano) ? "a moto" : "o pedido";
            const char* segundoRecurso  = (isVeterano) ? "o pedido" : "a moto";
            pthread_mutex_t* primeiraAcao = (isVeterano) ? &motos[numRestaurante] : &pedidos[numRestaurante];
            pthread_mutex_t* segundaAcao  = (isVeterano) ? &pedidos[numRestaurante] : &motos[numRestaurante];

            pthread_mutex_lock(primeiraAcao);
            std::cout << "[" << (char*) tipoEntregador << " nº " << threadID << "]: \npegou " << primeiroRecurso << " do restaurante " << numRestaurante << std::endl << std::endl;
            
            sleep(2); // caminhando pra pegar o outro recurso

            pthread_mutex_lock(segundaAcao);
            std::cout << "[" << (char*) tipoEntregador << " nº " << threadID << "]: \npegou " << segundoRecurso << " do restaurante " << numRestaurante << std::endl << std::endl;
                    
                lucroDiario += (rand_r(&seedLocal) % 4001 + 1000) / 100.0f; // conseguiu fazer a entrega e ganhou algo entre 10,00 e 50,00
                std::cout << "[" << (char*) tipoEntregador << " nº " << threadID << "]: \nrealizei a entrega do restaurante " << numRestaurante << std::endl << std::endl;

            pthread_mutex_unlock(primeiraAcao);
            pthread_mutex_unlock(segundaAcao);
    }
    pthread_exit(NULL);
}

int main(void){

    std::vector<pthread_t> entregNov(NUM_ENTREG);
    std::vector<pthread_t> entregVet(NUM_ENTREG);

    initMutexes(pedidos);
    initMutexes(motos);

    spawnThreads(entregNov, "Novato", &fazerEntrega);
    spawnThreads(entregVet, "Veterano", &fazerEntrega);

    // --- --- --- * --- --- --- * --- --- ---

    joinThreads(entregNov);
    joinThreads(entregVet);

    destroyMutexes(pedidos);
    destroyMutexes(motos);
    
    std::cout << "cheguei no final" << std::endl;

    return 0;

}