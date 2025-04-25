#include <stdio.h>
#include "metrics.h"

void imprimir_estatisticas(Estatisticas est, int total) {
    printf("\n--- Estatísticas ---\n");
    printf("Processos simulados: %d\n", total);
    printf("Tempo médio de espera: %.2f\n", est.tempo_medio_espera);
    printf("Tempo médio de resposta: %.2f\n", est.tempo_medio_resposta);
    printf("Utilização da CPU: %.2f%%\n", est.utilizacao_cpu);
    printf("Throughput: %d processos\n", est.throughput);
}