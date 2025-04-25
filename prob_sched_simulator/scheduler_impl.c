#include <stdio.h>
#include <stdbool.h>
#include "scheduler.h"

Estatisticas escalonar_fcfs(Processo *lista, int n) {
    int tempo = 0;
    int total_espera = 0, total_resposta = 0;

    for (int i = 0; i < n; i++) {
        if (tempo < lista[i].chegada) tempo = lista[i].chegada;
        int espera = tempo - lista[i].chegada;
        total_espera += espera;
        total_resposta += espera;
        tempo += lista[i].burst;
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_prioridade(Processo *lista, int n) {
    bool executado[n];
    for (int i = 0; i < n; i++) executado[i] = false;

    int tempo = 0, completados = 0;
    int total_espera = 0, total_resposta = 0;

    while (completados < n) {
        int escolhido = -1;
        int menor_prioridade = 9999;

        for (int i = 0; i < n; i++) {
            if (!executado[i] && lista[i].chegada <= tempo) {
                if (lista[i].prioridade < menor_prioridade ||
                    (lista[i].prioridade == menor_prioridade && lista[i].chegada < lista[escolhido].chegada)) {
                    escolhido = i;
                    menor_prioridade = lista[i].prioridade;
                }
            }
        }

        if (escolhido == -1) {
            tempo++;
        } else {
            int espera = tempo - lista[escolhido].chegada;
            total_espera += espera;
            total_resposta += espera;
            tempo += lista[escolhido].burst;
            executado[escolhido] = true;
            completados++;
        }
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_rr(Processo *lista, int n, int quantum) {
    int tempo = 0, completados = 0;
    int fila[1000], front = 0, back = 0;
    int restante[n];
    bool na_fila[n], iniciou[n];

    for (int i = 0; i < n; i++) {
        restante[i] = lista[i].burst;
        na_fila[i] = false;
        iniciou[i] = false;
    }

    fila[back++] = 0;
    na_fila[0] = true;

    int total_espera = 0, total_resposta = 0;

    while (completados < n) {
        if (front == back) {
            tempo++;
            for (int i = 0; i < n; i++) {
                if (!na_fila[i] && restante[i] > 0 && lista[i].chegada <= tempo) {
                    fila[back++] = i;
                    na_fila[i] = true;
                }
            }
            continue;
        }

        int atual = fila[front++];
        na_fila[atual] = false;

        if (!iniciou[atual]) {
            total_resposta += tempo - lista[atual].chegada;
            iniciou[atual] = true;
        }

        int exec_time = (restante[atual] > quantum) ? quantum : restante[atual];
        restante[atual] -= exec_time;
        tempo += exec_time;

        for (int i = 0; i < n; i++) {
            if (!na_fila[i] && restante[i] > 0 && lista[i].chegada <= tempo) {
                fila[back++] = i;
                na_fila[i] = true;
            }
        }

        if (restante[atual] > 0) {
            fila[back++] = atual;
            na_fila[atual] = true;
        } else {
            completados++;
            int espera = tempo - lista[atual].chegada - lista[atual].burst;
            total_espera += espera;
        }
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_sjf(Processo *lista, int n) {
    bool executado[n];
    for (int i = 0; i < n; i++) executado[i] = false;

    int tempo = 0, completados = 0;
    int total_espera = 0, total_resposta = 0;

    while (completados < n) {
        int escolhido = -1;
        int menor_burst = 9999;

        for (int i = 0; i < n; i++) {
            if (!executado[i] && lista[i].chegada <= tempo) {
                if (lista[i].burst < menor_burst) {
                    menor_burst = lista[i].burst;
                    escolhido = i;
                }
            }
        }

        if (escolhido == -1) {
            tempo++;
        } else {
            int espera = tempo - lista[escolhido].chegada;
            total_espera += espera;
            total_resposta += espera;
            tempo += lista[escolhido].burst;
            executado[escolhido] = true;
            completados++;
        }
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_prioridade_preemptivo(Processo *lista, int n) {
    int tempo = 0, completados = 0;
    int restante[n], prioridade[n], chegada[n];
    for (int i = 0; i < n; i++) {
        restante[i] = lista[i].burst;
        prioridade[i] = lista[i].prioridade;
        chegada[i] = lista[i].chegada;
    }

    int total_espera = 0, total_resposta = 0;
    bool executado[n];
    for (int i = 0; i < n; i++) executado[i] = false;

    int atual = -1;
    while (completados < n) {
        int menor = 9999;
        atual = -1;

        for (int i = 0; i < n; i++) {
            if (chegada[i] <= tempo && restante[i] > 0) {
                if (prioridade[i] < menor) {
                    menor = prioridade[i];
                    atual = i;
                }
            }
        }

        if (atual == -1) {
            tempo++;
            continue;
        }

        if (!executado[atual]) {
            total_resposta += tempo - chegada[atual];
            executado[atual] = true;
        }

        restante[atual]--;
        tempo++;

        if (restante[atual] == 0) {
            completados++;
            int espera = tempo - chegada[atual] - lista[atual].burst;
            total_espera += espera;
        }
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_edf(Processo *lista, int n) {
    int tempo = 0, completados = 0;
    int restante[n];
    for (int i = 0; i < n; i++) {
        restante[i] = lista[i].burst;
    }

    int total_espera = 0, total_resposta = 0;

    while (completados < n) {
        int escolhido = -1;
        int menor_deadline = 99999;

        for (int i = 0; i < n; i++) {
            if (lista[i].chegada <= tempo && restante[i] > 0) {
                if (lista[i].deadline < menor_deadline) {
                    menor_deadline = lista[i].deadline;
                    escolhido = i;
                }
            }
        }

        if (escolhido == -1) {
            tempo++;
            continue;
        }

        int espera = tempo - lista[escolhido].chegada;
        total_espera += espera;
        total_resposta += espera;
        tempo += lista[escolhido].burst;
        restante[escolhido] = 0;
        completados++;
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}

Estatisticas escalonar_rm(Processo *lista, int n) {
    int tempo = 0, completados = 0;
    int restante[n];
    for (int i = 0; i < n; i++) {
        restante[i] = lista[i].burst;
    }

    int total_espera = 0, total_resposta = 0;

    while (completados < n) {
        int escolhido = -1;
        int menor_periodo = 99999;

        for (int i = 0; i < n; i++) {
            if (lista[i].chegada <= tempo && restante[i] > 0) {
                if (lista[i].period < menor_periodo) {
                    menor_periodo = lista[i].period;
                    escolhido = i;
                }
            }
        }

        if (escolhido == -1) {
            tempo++;
            continue;
        }

        int espera = tempo - lista[escolhido].chegada;
        total_espera += espera;
        total_resposta += espera;
        tempo += lista[escolhido].burst;
        restante[escolhido] = 0;
        completados++;
    }

    Estatisticas est;
    est.tempo_medio_espera = (double)total_espera / n;
    est.tempo_medio_resposta = (double)total_resposta / n;
    est.utilizacao_cpu = 100.0;
    est.throughput = n;
    return est;
}
