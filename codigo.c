#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h> // importação das bibliotecas necessárias

#define DATA_TYPE int // definição do tipo de dado a ser usado

double time_diff(struct timespec start, struct timespec end) { // função para calcular a diferença de tempo
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}

void fill_indices(int*indices, int size) { // função para preencher o array de índices
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }
}

void shuffle(int* indices, int size) { // função para embaralhar o array de índices
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}   

void run_benchmark(DATA_TYPE* array, int* indices, int size, const char* access_pattern, const char* output_file) { // função para executar o benchmark
    struct timespec start, end;
    FILE* fp = fopen(output_file, "a");
    if (fp == NULL) {
        perror("Failed to open output file");
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < size; i++) {
        volatile DATA_TYPE temp = array[indices[i]];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = time_diff(start, end);
    fprintf(fp, "%s access time: %f seconds\n", access_pattern, elapsed);
    fclose(fp);
}

void execute_tests_for_size(long N, const char* output_filename) {
    
    // 1. Alocação Dinâmica do Vetor Principal
    DATA_TYPE *data_array = (DATA_TYPE *)malloc(N * sizeof(DATA_TYPE));
    if (data_array == NULL) {
        perror("Falha ao alocar data_array");
        return;
    }
    
    // Inicialização do vetor 
    for (long i = 0; i < N; i++) {
        data_array[i] = (DATA_TYPE)i;
    }

    // Alocação do Vetor de Índices 
    int *indices = (int *)malloc(N * sizeof(int));
    if (indices == NULL) {
        perror("Falha ao alocar indices");
        free(data_array);
        return;
    }
    
    // Limpa o arquivo de saída para cada execução, ou remove esta linha e apenas use 'a' no fopen
    // FILE* fp = fopen(output_filename, "w"); fclose(fp); 

    printf("\n--- Testando com N = %ld elementos (%ld MB) ---\n", N, (N * sizeof(DATA_TYPE)) / (1024 * 1024));

    // Indices preenchidos em ordem: 0, 1, 2, ... para o acesso sequencial
    fill_indices(indices, N); 
    run_benchmark(data_array, indices, N, "Sequential", output_filename);


    // Embaralha o vetor de índices para o acesso randômico
    shuffle(indices, N); 
    run_benchmark(data_array, indices, N, "Random", output_filename);
    
    // Limpeza
    free(data_array);
    free(indices);
}

int main() {
    srand(time(NULL)); 
    const char* output_file = "benchmark_results.txt";
    
    FILE* fp_clean = fopen(output_file, "w"); 
    if (fp_clean != NULL) {
        fclose(fp_clean);
    }

    // 3. Definição dos tamanhos de vetor para teste (10^5, 10^6, 10^7)
    long sizes[] = {
        100000,     // 10^5
        1000000,    // 10^6 
        10000000,   // 10^7
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Resultados serão salvos em: %s\n", output_file);
    
    for (int i = 0; i < num_sizes; i++) {
        execute_tests_for_size(sizes[i], output_file);
    }

    return 0;
}