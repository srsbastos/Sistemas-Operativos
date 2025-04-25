#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process_generator.h"
#include "scheduler.h"
#include "metrics.h"

#define MAX_PROCESSES 100

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <algoritmo> [quantum] [seed]\n", argv[0]);
        printf("Ou:  %s FICHEIRO <ficheiro.txt> <algoritmo> [quantum] [seed]\n", argv[0]);
        printf("Algoritmos: FCFS, PRIORITY, PRIORITYP, RR, SJF, EDF, RM\n");
        return 1;
    }

    char *algoritmo;
    int quantum = 4;
    int seed = time(NULL);

    Processo lista[MAX_PROCESSES];
    int total;

    // Escolha: leitura de ficheiro ou geração aleatória
    if (strcmp(argv[1], "FICHEIRO") == 0 && argc >= 4) {
        total = ler_processos_de_ficheiro(lista, argv[2]);
        algoritmo = argv[3];
        if (argc >= 5) quantum = atoi(argv[4]);
        if (argc >= 6) seed = atoi(argv[5]);
    } else {
        algoritmo = argv[1];
        if (argc >= 3) quantum = atoi(argv[2]);
        if (argc >= 4) seed = atoi(argv[3]);
        total = gerar_processos(lista, MAX_PROCESSES);
    }

    srand(seed);

    Estatisticas stats;

    if (strcmp(algoritmo, "FCFS") == 0) {
        stats = escalonar_fcfs(lista, total);
    } else if (strcmp(algoritmo, "PRIORITY") == 0) {
        stats = escalonar_prioridade(lista, total);
    } else if (strcmp(algoritmo, "PRIORITYP") == 0) {
        stats = escalonar_prioridade_preemptivo(lista, total);
    } else if (strcmp(algoritmo, "RR") == 0) {
        stats = escalonar_rr(lista, total, quantum);
    } else if (strcmp(algoritmo, "SJF") == 0) {
        stats = escalonar_sjf(lista, total);
    } else if (strcmp(algoritmo, "EDF") == 0) {
        stats = escalonar_edf(lista, total);
    } else if (strcmp(algoritmo, "RM") == 0) {
        stats = escalonar_rm(lista, total);
    } else {
        fprintf(stderr, "Algoritmo não reconhecido: %s\n", algoritmo);
        return 1;
    }

    imprimir_estatisticas(stats, total);
    return 0;
}
