#ifndef PROCESS_GENERATOR_H
#define PROCESS_GENERATOR_H

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int chegada;
    int burst;
    int prioridade;
    int restante;
    int deadline;  
    int period;
    int ultima_exec;
} Processo;

int gerar_processos(Processo *lista, int max);
int ler_processos_de_ficheiro(Processo *lista, const char *filename);

#endif