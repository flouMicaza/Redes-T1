#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include "jsocket6.4.h"
#include "Data.h"
#include <pthread.h>


#define BUFFER_LENGTH 1400 //hay que definir que largo vamos a usarrrr

struct timeval t0, t1, t2;
int portTCP;
char *portUDP="2000";
int sudp;
int stcp;
int rdy=0;
void* funcionTCP(void *puerto);



//funcion auxiliar que hace Dbind
//creada para no enrtegar inputs en el thread
void *DbindAux(){
	Dbind(funcionTCP,"2001");
	return NULL;
}

void *funcionTCP(void *puerto){
	int bytes,cnt,packs;
	char buffer[BUFFER_LENGTH];
	gettimeofday(&t0,NULL);


	portTCP=*((int*)puerto);
	free(puerto);
	fprintf(stderr, "cliente tcp conectado\n");

	for(bytes=0,packs=0;; bytes+=cnt,packs++) {
		cnt = Dread(portTCP, buffer, BUFFER_LENGTH);
		printf("cnt: %d\n", cnt);
		if(bytes == 0){
			gettimeofday(&t1, NULL); /* Mejor esperar al primer read para empezar a contar */
			write(sudp,NULL,0); //primer mensaje a udp
		}

		if(cnt <= 0){
			write(sudp, NULL, 0);
			break;
		}

		write(sudp, buffer, cnt);

		
    }	

	return NULL;
}


void *funcionUDP(){
	int bytes, cnt;
	char buffer[BUFFER_LENGTH];
	
	for(bytes=0;;bytes+=cnt) {
		if((cnt=read(sudp, buffer, BUFFER_LENGTH)) <= 0){
			Dwrite(portTCP, buffer, 0); //ya no queda nada mas para leer
		    break;
		}
		Dwrite(portTCP, buffer, cnt); //devolvemos cosas a stcp
	}

	
	
	//Dwrite(stcp, buffer, 0); //avisa que termino
	rdy=1;
	Dclose(stcp);
    close(sudp);
	return NULL;
}


int main(){
	sudp = j_socket_udp_connect("localhost","2000");
    if(sudp < 0) {
	printf("connect UDP failed\n");
       	exit(1);
    }

    printf(" UDP conectado\n");
    pthread_t pid1;
    pthread_t pid2;

	pthread_create(&pid1,NULL,DbindAux,NULL);
	pthread_create(&pid2,NULL,funcionUDP,NULL);

	while(!rdy){
		;
	}

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	
	return 0;

}