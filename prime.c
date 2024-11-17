#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>iya

typedef struct {
    int number;
    bool is_prime;
} primeArgs;

void* checkPrime(void* args) {
    primeArgs* data = (primeArgs*)args;
    int num = data->number;

    if (num <= 1) {
        data->is_prime = false;
        return NULL;
    }

    for (int i = 2; i < num; i++){
        if (num % i == 0) {
            data->is_prime = false;
            return NULL;
        }    
    }
    data->is_prime = true;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Penggunaan: %s <angka1> <angka2> ... <angkaN>\n", argv[0]);
        return 1; // Keluar jika tidak ada argumen
    }

    int n = argc - 1;
    primeArgs* args = (primeArgs*)malloc(n * sizeof(primeArgs));
    pthread_t* thread1 = (pthread_t*)malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++){
        args[i].number = atoi(argv[i + 1]);
        args[i].is_prime = false;
    }

    for (int i = 0; i < n; i++) {
        pthread_create(&thread1[i], NULL, checkPrime, &args[i]);
    }

    for (int i = 0; i < n; i++){
        pthread_join(thread1[i], NULL);
    }
    
    printf("\nHasil pengecekan bilangan prima:\n");
    for (int i = 0; i < n; i++) {
        printf("Angka %d: %s\n", args[i].number, args[i].is_prime ? "Prima" : "Bukan Prima");
    }

    free(args);
    free(thread1);

    return 0;
}