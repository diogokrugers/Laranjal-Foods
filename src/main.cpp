#include <iostream>
#include <pthread.h>    // variáveis e funções daqui possuem prefixo 'pthread_'
#include <unistd.h>     // getpid(), gettid(), sleep()
#include <vector>
#include <cstring>
#include "headers/init_and_destroy.h"
#include "headers/ThreadArgs.h"
#include "headers/buildTabs.h"

#define NUM_REST 6
std::vector<pthread_mutex_t> pedidos(NUM_REST);
std::vector<pthread_mutex_t> motos(NUM_REST);

#define NUM_ENTREG 4

#define SLEEP_TIME 1

pthread_mutex_t coutMutex = PTHREAD_MUTEX_INITIALIZER;

void* fazerEntrega(void* args){
    ThreadArgs* localArgs = (ThreadArgs*) args;
    int threadID = localArgs->threadID;
    const char* tipoEntregador = localArgs->tipoEntregador;
    std::string spaces = localArgs->spaces;

    float lucroDiario = 0;
    unsigned int seedLocal = time(NULL) ^ gettid();

    while(true){
            int numRestaurante = rand_r(&seedLocal) % NUM_REST;

            bool isVeterano = strcmp(tipoEntregador, "Veterano") == 0;
            const char* primeiroRecurso = (isVeterano) ? "a moto" : "o pedido";
            const char* segundoRecurso  = (isVeterano) ? "o pedido" : "a moto";
            pthread_mutex_t* primeiraAcao = (isVeterano) ? &motos[numRestaurante] : &pedidos[numRestaurante];
            pthread_mutex_t* segundaAcao  = (isVeterano) ? &pedidos[numRestaurante] : &motos[numRestaurante];

            pthread_mutex_lock(primeiraAcao);
            pthread_mutex_lock(&coutMutex);
            std::cout << spaces<< "[" << tipoEntregador << " nº " << threadID << "]:\n" << spaces << "pegou " << primeiroRecurso << " do res. " << numRestaurante << "\n\n";
            pthread_mutex_unlock(&coutMutex);

            sleep(SLEEP_TIME); // caminhando pra pegar o outro recurso

            pthread_mutex_lock(segundaAcao);
            pthread_mutex_lock(&coutMutex);
            std::cout << spaces << "[" << tipoEntregador << " nº " << threadID << "]:\n" << spaces << "pegou " << segundoRecurso << " do res. " << numRestaurante << "\n\n";
            pthread_mutex_unlock(&coutMutex);

                lucroDiario += (rand_r(&seedLocal) % 4001 + 1000) / 100.0f; // conseguiu fazer a entrega e ganhou algo entre 10,00 e 50,00
                pthread_mutex_lock(&coutMutex);
                std::cout << spaces << "[" << tipoEntregador << " nº " << threadID << "]:\n" << spaces << "fez a entrega do res. " << numRestaurante << "\n\n";
                pthread_mutex_unlock(&coutMutex);

            pthread_mutex_unlock(primeiraAcao);
            pthread_mutex_unlock(segundaAcao);
            pthread_mutex_lock(&coutMutex);
            std::cout << spaces << "[" << tipoEntregador << " nº " << threadID << "]:\n" << spaces << "possui R$ " << lucroDiario << "\n\n";
            pthread_mutex_unlock(&coutMutex);
    }

    pthread_exit(NULL);
}

int main(void){

if (NUM_ENTREG < 2){
    std::cerr << "Pra ser divertido tem que ter mais de 2 threads rolando!!!" << std::endl;
    return 1;
}   

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