#include <iostream>
#include <pthread.h>    // variáveis e funções daqui possuem prefixo 'pthread_'
#include <unistd.h>     // getpid(), gettid(), sleep()

void* printOi(void* arg){
    std::cout << "oi! o meu Thread ID no computador inteiro é: " << gettid() << std::endl;
    std::cout << "ah! e meu \"Thread ID\" somente nesse programa é: " << (long) arg << std::endl << std::endl;
    sleep(3);
    pthread_exit(NULL);
}

int main(void){

    pthread_t thread1;
    if (pthread_create(&thread1, NULL, &printOi, (void *) 1) != 0){
        return 1;
    }

    pthread_t thread2;
    if (pthread_create(&thread2, NULL, &printOi, (void *) 2) != 0){
        return 2;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    std::cout << "cheguei aqui" << std::endl;

    return 0;
}