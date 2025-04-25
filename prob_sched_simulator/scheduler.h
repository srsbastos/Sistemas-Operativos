#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process_generator.h"

typedef struct {
    double tempo_medio_espera;
    double tempo_medio_resposta;
    double utilizacao_cpu;
    int throughput;
} Estatisticas;

Estatisticas escalonar_fcfs(Processo *lista, int n);
Estatisticas escalonar_prioridade(Processo *lista, int n);
Estatisticas escalonar_rr(Processo *lista, int n, int quantum);
Estatisticas escalonar_sjf(Processo *lista, int n);
Estatisticas escalonar_edf(Processo *lista, int n);
Estatisticas escalonar_rm(Processo *lista, int n);
Estatisticas escalonar_prioridade_preemptivo(Processo *lista, int n);

#endif
