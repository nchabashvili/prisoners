#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>


int games = 100;
int game = 0;
static int box[100];
int did_pass[100];
pthread_mutex_t global_lock;


void randomize();
void random_game();
void strat_game();
static double timeit(int n, void* (*proc)(void *));
void* run_threads(int n, void* (*proc)(void *));
void* prisoner_thread(void *num);
void* thread_global_random(void *num);
void* thread_global_strat(void *num);
int did_win();


int main(int argc, char *argv[]) {
    char ch;
    if (pthread_mutex_init(&global_lock, NULL) == -1) {
        perror("pthread_mutex_init()");
        return EXIT_FAILURE;
    }
  
    while ((ch = getopt(argc, argv, "n:s")) != -1) {
        if (ch == 'n') {
            if (optarg != NULL) {
                int total_games;
                total_games = atoi(optarg);
                games = total_games;
            }
        }
        if (ch == 's') {
            srand(time(NULL));
        }
    }

    double time;
    int won;

    time = 0;
    won = 0;
    for (int i = 0; i < games; i++) {
        time += timeit(100, thread_global_random);
        won += did_win();
    }

    printf("method random_global\t %d/%d wins = %.2f%% %.3f ms\n", won, games, (float) won / (float) games * 100.0f, time);

    time = 0;
    won = 0;
    for (int i = 0; i < games; i++) {
        time += timeit(100, thread_global_strat);
        won += did_win();
    }

    printf("method strategy_global\t %d/%d wins = %.2f%% %.3f ms\n", won, games, (float) won / (float) games * 100.0f, time);
}

void* thread_global_strat(void *num) {
    pthread_mutex_lock(&global_lock);

    int prisoner_index = *((int *) num);
    int next_attempt = prisoner_index;

    for (int i = 0; i < 50; i++) {
        if (box[next_attempt] == prisoner_index) {
            did_pass[prisoner_index] = 1;
            break;
        }

        next_attempt = box[next_attempt];
    }

    pthread_mutex_unlock(&global_lock);
}

void* thread_global_random(void *num) {
    pthread_mutex_lock(&global_lock);

    // Cast to int* and dereference
    int prisoner_index = *((int *) num);
    int checked[100];
    memset(checked, 0, sizeof(checked));
    did_pass[prisoner_index] = 0;
    
    for (int i = 0; i < 50; i++) {
        int random = (rand() % 100);
        while (checked[random]) {
            random = (rand() % 100);
        }

        if (box[random] == prisoner_index) {
            did_pass[prisoner_index] = 1;
            break;
        }
    }

    pthread_mutex_unlock(&global_lock);
}

void* run_threads(int n, void* (*proc)(void *)) {
    pthread_t thread_ids[100];   
    pthread_t newthread;
    for (int i = 0; i < 100; i++) {
        did_pass[i] = 0;
    }

    randomize();

    for (int i = 0; i < 100; i++) {
        int *num = (int*) malloc(sizeof(int));
        *num = i;

        pthread_create(&newthread, NULL, proc, num);

        thread_ids[i] = newthread;
    }
    void* val;
    for (int i = 0; i < 100; i++) {
        pthread_join(thread_ids[i], &val);
    }
}

void randomize() {
    int checked[100];
    memset(checked, 0,100 * sizeof(int));
    for (int i = 0; i < 100; i++) {
        
        int random = rand() % 100;
        while (checked[random]){
            random = rand() % 100;
        }
        box[i] = random;
        checked[random] = 1;
    }
}

static double timeit(int n, void* (*proc)(void *)) {
    clock_t t1, t2;
    t1 = clock();
    run_threads(n, proc);
    t2 = clock();
    return ((double) t2 - (double) t1) / CLOCKS_PER_SEC * 1000;
}

int did_win() {
    int total = 0;
    for (int i = 0; i < 100; i++) {
        total += did_pass[i];
    }

    return total == 100;
}

