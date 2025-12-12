#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    char name[50];
    int w, d, l, gs, gc;
    int puan;
    int fark;
    pthread_t tid;
} Takim;

int global_max = -1;
int global_min = 10000;
pthread_mutex_t mutex;

void* worker(void* arg) {
    Takim* t = (Takim*)arg;
    t->puan = (t->w * 3) + t->d;
    t->fark = t->gs - t->gc;

    pthread_mutex_lock(&mutex);
    if (t->puan > global_max) {
        global_max = t->puan;
    }
    if (t->puan < global_min) {
        global_min = t->puan;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int compare(const void* a, const void* b) {
    Takim* t1 = (Takim*)a;
    Takim* t2 = (Takim*)b;
    if (t1->puan != t2->puan) {
        return t2->puan - t1->puan;
    }
    if (t1->fark != t2->fark) {
        return t2->fark - t1->fark;
    }
    return t2->gs - t1->gs;
}

int main() {
    int n, c, r;
    
    if (scanf("%d", &n) != 1) return 0;
    scanf("%d", &c);
    scanf("%d", &r);

    Takim* takimlar = (Takim*)malloc(n * sizeof(Takim));
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < n; i++) {
        scanf("%s %d %d %d %d %d", takimlar[i].name, &takimlar[i].w, &takimlar[i].d, &takimlar[i].l, &takimlar[i].gs, &takimlar[i].gc);
        pthread_create(&takimlar[i].tid, NULL, worker, &takimlar[i]);
    }

    for(int i = 0; i < n; i++) {
        pthread_join(takimlar[i].tid, NULL);
    }

    qsort(takimlar, n, sizeof(Takim), compare);

    for(int i = 0; i < n; i++) {
        printf("%s %d", takimlar[i].name, takimlar[i].puan);
        if (i < c) {
            printf(" - Qualified");
        } else if (i >= n - r) {
            printf(" - Relegated");
        }
        printf("\n");
    }

    printf("\nHighest point: %d\n", global_max);
    printf("Lowest point: %d", global_min);

    pthread_mutex_destroy(&mutex);
    free(takimlar);
    return 0;
}