#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <stdio.h>

#define MAX_ITEMS 2

struct shmSeg {
    int cnt;
    int table[MAX_ITEMS];
};

int main() {
    int shmID = shmget(0x1234, sizeof(struct shmSeg), 0666 | IPC_CREAT);
    struct shmSeg* sharedMem = (struct shmSeg*)shmat(shmID, NULL, 0);
    usleep(1000000);
    sem_t* sem = sem_open("/sem", O_CREAT);

    while (1) {
        while (sharedMem->cnt == 0) {}
        sem_wait(sem);
        sharedMem->table[sharedMem->cnt] = 0;
        sem_post(sem);
        sharedMem->cnt--;
        usleep(1000000);
    }
    return 0;
}