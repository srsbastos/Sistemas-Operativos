#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "process_generator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double gerar_exponencial(double lambda) {
    double u = (double)rand() / RAND_MAX;
    return -log(1 - u) / lambda;
}

double gerar_normal(double media, double desvio) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return media + desvio * z;
}

int gerar_processos(Processo *lista, int max) {
    int total = 10 + rand() % (max - 10);
    for (int i = 0; i < total; i++) {
        lista[i].pid = i + 1;
        lista[i].chegada = (int)gerar_exponencial(0.1);   // tempo de chegada com dist. exponencial
        lista[i].burst = (int)gerar_normal(10, 3);        // burst com dist. normal
        if (lista[i].burst <= 0) lista[i].burst = 1;       // garantir burst positivo
        lista[i].restante = lista[i].burst;
        lista[i].prioridade = 1 + rand() % 5;
        lista[i].ultima_exec = lista[i].chegada;
        lista[i].deadline = lista[i].chegada + (10 + rand() % 20); 
lista[i].period = 10 + rand() % 30; 
    }
    return total;
}
int ler_processos_de_ficheiro(Processo *lista, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro");
        return 0;
    }

    int i = 0;
    while (fscanf(f, "%d %d %d %d", &lista[i].pid, &lista[i].chegada,
      &lista[i].burst, &lista[i].prioridade) == 4) {
        lista[i].restante = lista[i].burst;
        lista[i].ultima_exec = lista[i].chegada;
        i++;
    }

    fclose(f);
    return i;
}
