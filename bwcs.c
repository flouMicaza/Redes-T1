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


#define PORTUDP "2000"
#define PORTTCP "2001"
#define BUFFER_LENGTH 1400 //hay que definir que largo vamos a usarrrr

char *sudp; //socket udp
char *stcp; //socket tcp
struct timeval t0, t1, t2;

//funcion que lee del tpc y lo escribe en udp
void* funcionTCP(int fd){
	int bytes, cnt;
	char buffer[BUFFER_LENGTH];
    
    gettimeofday(&t0, NULL);
    for(bytes=0;; bytes+=cnt) {
        cnt = Dread(s, buffer, BUFFER_LENGTH);
		if(bytes == 0)
			gettimeofday(&t1, NULL); /* Mejor esperar al primer read para empezar a contar */
			write(supd,NULL,0); //primer mensaje a udp
		if(cnt <= 0)
		    break;

		//escribir en el servidor udp
		write(sudp, buffer, cnt); //enviar a udp el mensaje que recibimos recien

	}
	
}

//funcion que recibe la respuesta del udp y se lo manda al connectTCP
void* funcionUDP(int fd){
	int bytes, cnt;
	char buffer[BUFFER_LENGTH];
	for(bytes=0;;bytes+=cnt) {
		if((cnt=read(fd, buffer, BUFFER_LENGTH)) <= 0){ //ya no queda nada mas para leer
		    break;
	        
	    }

	    Dwrite(s, buffer, cnt); //devolvemos cosas a TCP
	    Dwrite(s, buffer, 0); //avisa que termino
	}

	gettimeofday(&t2, NULL); //tiempo final

    t = (t2.tv_sec*1.0+t2.tv_usec*1e-6) - (t1.tv_sec*1.0+t1.tv_usec*1e-6);
    fprintf("El tiempo tardado es: %d\n", t);  
    Dclose(stcp);
    close(sudp);
}

int main(){
	//nose que se le pone a la parte de server
	//talvez estas variables tengan que ser globales
	
	/conexion con el servidor TCP
	stcp = Dconect(server,PORTTCP);
	if(sudp < 0) {
	printf("connect TCP failed\n");
       	exit(1);
    }

    printf("conectado\n");
	
	sudp = j_socket_udp_connect(server,PORTUDP);
    if(sudp < 0) {
	printf("connect UDP failed\n");
       	exit(1);
    }

    printf("conectado\n");


	p1=pthreadcreate(funcionTCP,server, PORTTCP);
	p2=pthreadcreate(funcionUDP,server, PORTUDP);

	//falta enterrar a los thread y cachar cuando hay q enterrarlos

	return 0;
}

