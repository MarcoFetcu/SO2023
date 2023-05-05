//
// Created by marcos on 04.05.2023.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"

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
                        wait(NULL);
                        wait(NULL);

                        info(END, 4, 0);
                        return 0;
                    }


                }


            } else {
                wait(NULL);
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

                pid8 = fork();
                if (pid8 == -1) {
                    printf("eroare la procesul 8");
                    return -8;
                } else if (pid8 == 0) {
                    info(BEGIN, 8, 0);


                    info(END, 8, 0);
                    return 0;
                }


            }


        }


    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    info(END, 1, 0);


    return 0;
}