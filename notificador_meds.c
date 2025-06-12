#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char nome[50];
    int intervalo_horas;  // em horas reais, mas usaremos segundos para teste
    int total_comprimidos;
} Medicamento;

void* notificar(void* arg) {
    Medicamento* med = (Medicamento*) arg;

    // Exibe os dados do medicamento antes de começar
    printf("=== Medicamento: %s | Intervalo: %d horas | Total: %d comprimidos ===\n",
           med->nome, med->intervalo_horas, med->total_comprimidos);

    for (int i = 1; i <= med->total_comprimidos; i++) {
        printf("[NOTIFICAÇÃO] Tome %s - dose %d de %d\n", med->nome, i, med->total_comprimidos);
        sleep(med->intervalo_horas);  // simulando tempo em segundos
    }
    printf("[FIM] Tratamento com %s concluído!\n", med->nome);
    return NULL;
}

int main() {
    pthread_t threads[3];
    Medicamento meds[3] = {
        {"Dorflex", 12, 10},        // 12 segundos (simulando 12 horas)
        {"Paracetamol", 8, 12},     // 8 segundos (8h)
        {"Cataflan", 24, 6}         // 24 segundos (24h)
    };

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, notificar, (void*)&meds[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Todas as medicações foram concluídas.\n");
    return 0;
}
