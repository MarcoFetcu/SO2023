//
// Created by marcos on 22.05.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>


#define PIPERESP "RESP_PIPE_84020"
#define PIPEREQ "REQ_PIPE_84020"

int fdReq = -1;
int fdResp = -1;
int fdMem = -1;

int main() {
    if (mkfifo(PIPERESP, 0600) != 0) {
        perror("ERROR\ncannot create the response pipe\n");
        exit(1);
    }

    fdReq = open(PIPEREQ, O_RDONLY);
    if (fdReq == -1) {
        perror("ERROR\ncannot open the request pipe\n");
        exit(1);
    }

    fdResp = open(PIPERESP, O_WRONLY);
    if (fdResp == -1) {

        perror("ERROR\ncannot open the response pipe\n");
        //close(fd2);
        exit(1);
    }
    char size = 5;
    write(fdResp, &size, 1);
    write(fdResp, "START", (int) size);
    printf("SUCCESS\n");


    char request[250];

    int cnt = 0;
//    //unsigned int size;
    unsigned int auxNr = 84020;
//    char size3;
//    //int i=0;
//    //int n;
    char *memory = (char *) malloc(sizeof(char));
    char *memory2 = (char *) malloc(sizeof(char));
    int sh;
    int aux;
//
    while (1) {
        char size2;
        unsigned int size3;
        read(fdReq, &size2, 1);
        cnt = read(fdReq, request, (int) size2);
        request[cnt] = 0;

        if (strcmp(request, "ECHO") == 0) {
            size2 = 4;
            write(fdResp, &size2, 1);
            write(fdResp, "ECHO", (int) size2);
            size2 = 7;
            write(fdResp, &size2, 1);
            write(fdResp, "VARIANT", (int) size2);
            write(fdResp, &auxNr, sizeof(unsigned int));
            break;
        } else if (strcmp(request, "CREATE_SHM") == 0) {

            read(fdReq, &aux, sizeof(int));
            sh = shm_open("/tuDpCD", O_CREAT | O_RDWR, 0664);
            if (sh >= 0) {
                ftruncate(sh, aux);
                memory = (char *) mmap(0, aux * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, sh, 0);
                if (memory == MAP_FAILED) {
                    size2 = 10;
                    write(fdResp, &size2, 1);
                    write(fdResp, "CREATE_SHM", (int) size2);
                    size2 = 5;
                    write(fdResp, &size2, 1);
                    write(fdResp, "ERROR", (int) size2);


                } else {
                    size2 = 10;
                    write(fdResp, &size2, 1);
                    write(fdResp, "CREATE_SHM", (int) size2);
                    size2 = 7;
                    write(fdResp, &size2, 1);
                    write(fdResp, "SUCCESS", (int) size2);
                }
            } else {
                size2 = 10;
                write(fdResp, &size2, 1);
                write(fdResp, "CREATE_SHM", (int) size2);
                size2 = 5;
                write(fdResp, &size2, 1);
                write(fdResp, "ERROR", (int) size2);

            }
            break;
        } else if (strcmp(request, "WRITE_TO_SHM") == 0) {

            unsigned int offset;
            unsigned int value;

            read(fdReq, &offset, sizeof(unsigned int));
            read(fdReq, &value, sizeof(unsigned int));

            if (offset < 0 || offset > 1482104 || offset + sizeof(value) < 0 || offset + sizeof(value) > 1482104) {

                size3 = 12;
                write(fdResp, &size3, sizeof(unsigned int));
                write(fdResp, "WRITE_TO_SHM", (int)size3);
                size2 = 5;
                write(fdResp, &size2, 1);
                write(fdResp, "ERROR", (int) size2);
            } else {

                memcpy(memory + offset, (char *) &value, sizeof(value));


                size3 = 12;
                write(fdResp, &size3, sizeof(unsigned int));
                write(fdResp, "WRITE_TO_SHM", size3);
                size2 = 7;
                write(fdResp, &size2, 1);
                write(fdResp, "SUCCESS", (int) size2);
                break;

            }
        } else if (strcmp(request, "MAP_FILE") == 0) {

            char path[250];
            read(fdReq,&size2,1);
            cnt=read(fdReq,path,(int)size2);
            path[cnt]=0;

            fdMem = open(path, O_RDWR);

            if(fdMem == -1) {
                size2 = 8;
                write(fdResp, &size2, 1);
                write(fdResp, "MAP_FILE", (int) size2);
                size2 = 5;
                write(fdResp, &size2, 1);
                write(fdResp, "ERROR", (int) size2);
            }else
            {

                aux = lseek(fdMem, 0, SEEK_END);
                lseek(fdMem, 0, SEEK_SET);

                memory2 = (char*)mmap(NULL, aux, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdMem, 0);
                if(memory2 == (void*)-1)
                {
                    size2 = 8;
                    write(fdResp, &size2, 1);
                    write(fdResp, "MAP_FILE", (int) size2);
                    size2 = 5;
                    write(fdResp, &size2, 1);
                    write(fdResp, "ERROR", (int) size2);
                }else{
                    size2 = 8;
                    write(fdResp, &size2, 1);
                    write(fdResp, "MAP_FILE", (int) size2);
                    size2 = 7;
                    write(fdResp, &size2, 1);
                    write(fdResp, "SUCCESS", (int) size2);
                }
                break;
            }

        } else if (strcmp(request, "READ_FROM_FILE_OFFSET") == 0) {
            break;
        } else if (strcmp(request, "READ_FROM_FILE_SECTION") == 0) {
            break;
        } else if (strcmp(request, "READ_FROM_LOGICAL_SPACE_OFFSET") == 0) {
            break;
        } else if (strcmp(request, "EXIT") == 0) {
            break;
        }
    }
    close(fdReq);
    close(fdResp);
    close(fdMem);
    close(sh);
    unlink(PIPERESP);
    unlink(PIPEREQ);
    munmap(memory, aux);
    munmap(memory2, aux);

    return 0;
}



