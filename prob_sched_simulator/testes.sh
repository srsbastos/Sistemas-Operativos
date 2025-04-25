#!/bin/bash
rm -f resultados.txt

for algo in FCFS SJF PRIORITY PRIORITYP RR EDF RM; do
  echo "=== $algo ===" >> resultados.txt
  ./simulator $algo 0 123 >> resultados.txt
  echo "" >> resultados.txt
done
