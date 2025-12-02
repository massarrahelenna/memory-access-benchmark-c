#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define DATA_TYPE int

// Função para medir tempo
double time_diff(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}

// Gerador rápido de números aleatórios (Xorshift64)
uint64_t rng_state = 88172645463393265ULL;

uint64_t xorshift64() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return rng_state;
}

// Preencher índices em ordem
void fill_indices(size_t *indices, size_t size) {
    for (size_t i = 0; i < size; i++) {
        indices[i] = i;
    }
}

// Shuffle (Fisher–Yates + xorshift)
void shuffle(size_t *indices, size_t size) {
    for (size_t i = size - 1; i > 0; i--) {
        size_t j = xorshift64() % (i + 1);
        size_t temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

// Warmup da memória: tocar cada página
// para evitar pagefault durante o benchmark
void warmup(DATA_TYPE *array, size_t size) {
    size_t step = 4096 / sizeof(DATA_TYPE);
    for (size_t i = 0; i < size; i += step) {
        array[i] = (DATA_TYPE)i;
    }
}

// Executa um benchmark e retorna o tempo
double run_single(DATA_TYPE *array, size_t *indices, size_t size) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < size; i++) {
        volatile DATA_TYPE tmp = array[indices[i]];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    return time_diff(start, end);
}
// Mediana de 3 valores
double median3(double a, double b, double c) {
    if (a > b) { double t = a; a = b; b = t; }
    if (b > c) { double t = b; b = c; c = t; }
    if (a > b) { double t = a; a = b; b = t; }
    return b;
}

// Executa benchmark completo para um tamanho N
void execute_tests_for_size(size_t N, const char *output) {

    DATA_TYPE *data_array = malloc(N * sizeof(DATA_TYPE));
    if (!data_array) {
        perror("Falha ao alocar data_array");
        return;
    }

    size_t *indices = malloc(N * sizeof(size_t));
    if (!indices) {
        perror("Falha ao alocar indices");
        free(data_array);
        return;
    }

    // Popular dados
    for (size_t i = 0; i < N; i++)
        data_array[i] = (DATA_TYPE)i;

    printf("\n--- Testando com N = %zu elementos (%.2f MB) ---\n",
           N, (double)(N * sizeof(DATA_TYPE)) / (1024.0 * 1024.0));

    // Warmup de memória
    warmup(data_array, N);

    FILE *fp = fopen(output, "a");
    if (!fp) {
        perror("Falha ao abrir arquivo de saída");
        free(data_array);
        free(indices);
        return;
    }

    // Teste SEQUENCIAL
    fill_indices(indices, N);

    double s1 = run_single(data_array, indices, N);
    double s2 = run_single(data_array, indices, N);
    double s3 = run_single(data_array, indices, N);

    double seq_result = median3(s1, s2, s3);

    fprintf(fp, "N=%zu | Sequential: %.6f s\n", N, seq_result);

    // Teste RANDÔMICO
    shuffle(indices, N);

    double r1 = run_single(data_array, indices, N);
    double r2 = run_single(data_array, indices, N);
    double r3 = run_single(data_array, indices, N);

    double rand_result = median3(r1, r2, r3);

    fprintf(fp, "N=%zu | Random:     %.6f s\n", N, rand_result);

    fclose(fp);

    free(data_array);
    free(indices);
}


int main() {
    const char *output = "benchmark_results.txt";
    FILE *clean = fopen(output, "w");
    if (clean) fclose(clean);

    size_t sizes[] = {
        100000ULL,        // 10^5
        1000000ULL,       // 10^6
        10000000ULL,      // 10^7
        100000000ULL,      // 10^8
        1000000000ULL     // 10^9 (ativar se tiver 8GB+ RAM)
    };

    int num = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num; i++)
        execute_tests_for_size(sizes[i], output);

    printf("\nBenchmark finalizado. Resultados em %s\n", output);

    return 0;
}

