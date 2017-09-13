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

#define PORTUDP "2000"
#define PORTtcp "2001"
#define BUFFER_LENGTH 1400 //hay que definir que largo vamos a usarrrr

int sudp; //socket udp
int stcp; //socket stcp
struct timeval t0, t1, t2;
int t;



//funcion que lee del tpc y lo escribe en udp
void *funciontcp(){
	int bytes, cnt;
	char buffer[BUFFER_LENGTH];

    gettimeofday(&t0, NULL);
    for(bytes=0;; bytes+=cnt) {
        cnt = Dread(stcp, buffer, BUFFER_LENGTH);
		if(bytes == 0)
			gettimeofday(&t1, NULL); /* Mejor esperar al primer read para empezar a contar */
			write(sudp,NULL,0); //primer mensaje a udp
		if(cnt <= 0)
		    break;

		//escribir en el servidor udp
		write(sudp, buffer, cnt); //enviar a udp el mensaje que recibimos recien

	}
	return NULL;
}

//funcion que recibe la respuesta del udp y se lo manda al connectstcp
void *funcionUDP(){
	int bytes, cnt;
	char buffer[BUFFER_LENGTH];

	for(bytes=0;;bytes+=cnt) {
		if((cnt=read(sudp, buffer, BUFFER_LENGTH)) <= 0){ //ya no queda nada mas para leer
		    break;
	        
	    }

	    Dwrite(stcp, buffer, cnt); //devolvemos cosas a stcp
	    Dwrite(stcp, buffer, 0); //avisa que termino
	}

	gettimeofday(&t2, NULL); //tiempo final

    t = (t2.tv_sec*1.0+t2.tv_usec*1e-6) - (t1.tv_sec*1.0+t1.tv_usec*1e-6);
    printf("El tiempo tardado es: %d\n", t);  
    Dclose(stcp);
    close(sudp);
    return NULL;
}

void Dbind_aux(){
	Dbind(funciontcp, PORTtcp);
}

int main(){
	//nose que se le pone a la parte de server
	//talvez estas variables tengan que ser globales
	
	//conexion con el servidor stcp
	/*stcp = Dconnect("localhost",PORTtcp);
	if(sudp < 0) {
	printf("connect stcp failed\n");
       	exit(1);
    }

    printf("conectado\n");
	*/
	sudp = j_socket_udp_connect("localhost",PORTUDP);
    if(sudp < 0) {
	printf("connect UDP failed\n");
       	exit(1);
    }

    printf("conectado\n");

    pthread_t pid1;

    pthread_t pid2;
	pthread_create(&pid1,NULL,Dbind,funciontcp,2000);
	pthread_create(&pid2,NULL,funcionUDP,NULL);

	//falta enterrar a los thread y cachar cuando hay q enterrarlos

	return 0;
}

