//
// Created by marcos on 04.05.2023.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include "a2_helper.h"

sem_t *startsem4;
sem_t *finalsem5;

sem_t *sem1;
sem_t *sem2;


void *threadFn1(void *vid) {
    int id = *(int *) vid;
    if (id == 1) {
        info(BEGIN, 8, 1);
        info(END, 8, 1);

    } else if (id == 2) {
        info(BEGIN, 8, 2);
        info(END, 8, 2);
    } else if (id == 3) {
        sem_wait(sem1);
        info(BEGIN, 8, 3);
        info(END, 8, 3);
        sem_post(sem2);


    } else if (id == 4) {
        info(BEGIN, 8, 4);
        sem_post(finalsem5);
        sem_wait(startsem4);
        info(END, 8, 4);

    } else if (id == 5) {
        sem_wait(finalsem5);
        info(BEGIN, 8, 5);
        info(END, 8, 5);
        sem_post(startsem4);
    }

    return NULL;
}

void *threadFn2(void *vid) {
    int id = *(int *) vid;
    if (id == 1) {
        info(BEGIN, 4, 1);
        info(END, 4, 1);

    } else if (id == 2) {
        info(BEGIN, 4, 2);
        info(END, 4, 2);
        sem_post(sem1);
    } else if (id == 3) {
        sem_wait(sem2);
        info(BEGIN, 4, 3);
        info(END, 4, 3);

    } else if (id == 4) {
        info(BEGIN, 4, 4);

        info(END, 4, 4);
    }

    return NULL;
}

int main() {
    init();
    pid_t pid2 = -1;
    pid_t pid3 = -1;
    pid_t pid4 = -1;
    pid_t pid5 = -1;
    pid_t pid6 = -1;
    pid_t pid7 = -1;
    pid_t pid8 = -1;

    info(BEGIN, 1, 0);

    finalsem5 = sem_open("finalsem5", O_CREAT, 0644, 0);
    startsem4 = sem_open("startsem4", O_CREAT, 0644, 0);

    sem1 = sem_open("sem1", O_CREAT, 0644, 0);
    sem2 = sem_open("sem2", O_CREAT, 0644, 0);


    pid2 = fork();
    if (pid2 == -1) {
        printf("eroare la procesul 2");
        return -2;
    } else if (pid2 == 0) {
        info(BEGIN, 2, 0);
        info(END, 2, 0);

        return 0;
    } else {
        pid3 = fork();
        if (pid3 == -1) {
            printf("eroare la procesul 3");
            return -3;
        } else if (pid3 == 0) {
            info(BEGIN, 3, 0);

            pid4 = fork();
            if (pid4 == -1) {
                printf("eroare la procesul 4");
                return -4;
            } else if (pid4 == 0) {
                info(BEGIN, 4, 0);

                pid5 = fork();
                if (pid5 == -1) {
                    printf("eroare la procesul 5");
                    return -5;
                } else if (pid5 == 0) {
                    info(BEGIN, 5, 0);
                    info(END, 5, 0);
                    return 0;
                } else {
                    pid6 = fork();
                    if (pid6 == -1) {
                        printf("eroare la procesul 6");
                        return -6;
                    } else if (pid6 == 0) {
                        info(BEGIN, 6, 0);
                        info(END, 6, 0);
                        return 0;
                    } else {
                        wait(NULL); //astept dupa 5
                        wait(NULL); // astept dupa 6

                        pthread_t tid4[4];
                        int id_tid4[4];



                        for (int i = 0; i < 4; i++) {
                            id_tid4[i] = i+1;
                            pthread_create(&tid4[i], NULL, threadFn2, &(id_tid4[i]));
                        }
                        for (int i = 0; i < 4; i++) {
                            pthread_join(tid4[i], 0);
                        }

                        info(END, 4, 0);
                        return 0;
                    }
                }

            } else {
                //p3
                wait(NULL);//asteapta 4
                info(END, 3, 0);
                return 0;
            }

        } else {
            pid7 = fork();
            if (pid7 == -1) {
                printf("eroare la procesul 7");
                return -7;
            } else if (pid7 == 0) {
                info(BEGIN, 7, 0);
                info(END, 7, 0);
                return 0;
            } else {
                //p1
                pid8 = fork();
                if (pid8 == -1) {
                    printf("eroare la procesul 8");
                    return -8;
                } else if (pid8 == 0) {
                    info(BEGIN, 8, 0);

                    pthread_t tid[5];
                    int id_tid[5];


                    for (int i = 0; i < 5; i++) {
                        id_tid[i] = i+1;
                        pthread_create(&tid[i], NULL, threadFn1, &(id_tid[i]));
                    }
                    for (int i = 0; i < 5; i++) {
                        pthread_join(tid[i], 0);
                    }


                    info(END, 8, 0);
                    return 0;
                }
            }
        }
    }
    wait(NULL); //ramane
    wait(NULL); //ramane
    wait(NULL);  //ramane
    wait(NULL);  //ramane

    sem_close(sem1);
    sem_unlink("sem1");

    sem_close(sem2);
    sem_unlink("sem2");


    sem_close(startsem4);
    sem_unlink("startsem4");

    sem_close(finalsem5);
    sem_unlink("finalsem5");

    info(END, 1, 0);


    return 0;
}