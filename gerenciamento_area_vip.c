#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define TOTAL_PESSOAS 100
#define VIP_CAPACIDADE 20

sem_t sem_vip;                 // controla capacidade da área VIP
pthread_mutex_t mutex_vip;     // mutex para proteger contador VIP

int pessoas_vip = 0;           // contador de pessoas na área VIP

void *pessoa(void *id) {
    long pid = (long)id;

    int escolhe_vip = (rand() % 100) < 30;  // 30% chance VIP

    if (escolhe_vip) {
        printf("Pessoa %ld tentando entrar na AREA VIP e aguardando...\n", pid);
        sem_wait(&sem_vip);  // espera vaga na VIP

        pthread_mutex_lock(&mutex_vip);
        pessoas_vip++;
        printf("Pessoa %ld entrou na AREA VIP. Pessoas VIP agora: %d\n", pid, pessoas_vip);
        pthread_mutex_unlock(&mutex_vip);

        // Permanência entre 10 e 20 segundos
        int tempo_vip = (rand() % 11) + 10;
        sleep(tempo_vip);

        pthread_mutex_lock(&mutex_vip);
        pessoas_vip--;
        printf("Pessoa %ld saiu da AREA VIP após %d segundos. Pessoas VIP agora: %d\n", pid, tempo_vip, pessoas_vip);
        pthread_mutex_unlock(&mutex_vip);

        sem_post(&sem_vip);  // libera vaga na VIP
    } else {
        // Área comum: sem controle nem contagem
        printf("Pessoa %ld está na AREA COMUM.\n", pid);
        // Permanência 5 a 10 segundos
        int tempo_comum = (rand() % 6) + 5;
        sleep(tempo_comum);
        printf("Pessoa %ld saiu da AREA COMUM após %d segundos.\n", pid, tempo_comum);
    }

    return NULL;
}

int main() {
    pthread_t pessoas[TOTAL_PESSOAS];
    srand(time(NULL));

    sem_init(&sem_vip, 0, VIP_CAPACIDADE);
    pthread_mutex_init(&mutex_vip, NULL);

    for (long i = 0; i < TOTAL_PESSOAS; i++) {
        pthread_create(&pessoas[i], NULL, pessoa, (void*)i);
        usleep(50000);  // 50ms para espaçar as criações
    }

    for (int i = 0; i < TOTAL_PESSOAS; i++) {
        pthread_join(pessoas[i], NULL);
    }

    sem_destroy(&sem_vip);
    pthread_mutex_destroy(&mutex_vip);

    printf("Todas as pessoas já passaram pelas áreas.\n");
    return 0;
}
