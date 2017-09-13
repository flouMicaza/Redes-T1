
#include <stdio.h>
#include <stdlib.h>
#include "jsocket6.h"

#define PORTUDP "2000"
#define PORTTCP "2001"
#define BUFFER_LENGTH 1400 //hay que definir que largo vamos a usarrrr

int sudp; //socket udp
int stcp; //socket tcp

//funcion que lee del tpc y lo escribe en udp
void* funcionTCP(void *puerto){
	stcp = j_socket_tcp_connect(server,PORTTCP);
	if(sudp < 0) {
	printf("connect TCP failed\n");
       	exit(1);
    }

    printf("conectado\n");
    
	//////todo esto lo saque de bws.c porque se conecta a traves de tcp
	nt n, nl;
    int bytes, cnt, packs;
    struct timeval t0, t1, t2;
    double t;
    char tmpfilename[15];
    int fd;
    int cl;

    cl = *((int *)puerto);
    free(puerto);

    //esto de aqui nose pq lo hace, osea nose pq esto es el fd de destino :/
    strcpy(tmpfilename, "tmpbwXXXXXX");

    fd = mkstemp(tmpfilename);
    if(fd < 0) {
	fprintf(stderr, "Can't create temp %s\n", tmpfilename);
	exit(1);
    }

    fprintf(stderr, "cliente conectado\n");

    gettimeofday(&t0, NULL);
    //aqui esta leyendo del socket 
    for(bytes=0,packs=0;; bytes+=cnt,packs++) {
		//lee del puerto cl
		cnt = Dread(cl, buffer, BUFFER_LENGTH);
		if(cnt <= 0) break;
		//hay que ver como hacer que el fd sea el fd del socket q va al udp
		write(fd, buffer, cnt);
    }

    gettimeofday(&t1, NULL);
    t = (t1.tv_sec+t1.tv_usec*1e-6)-(t0.tv_sec+t0.tv_usec*1e-6); 
    fprintf("El  tiempo que demoro es: %d\n",t);
    ///////////////aqui se acaba lo que copie de bws///////////

}

//funcion que recibe la respuesta del udp y se lo manda al connectTCP
void* funcionUDP(int server, int port){
	sudp = j_socket_udp_connect(server,PORTUDP);
    if(sudp < 0) {
	printf("connect UDP failed\n");
       	exit(1);
    }

    printf("conectado\n");

	while(true){
		read(s1,buf); //lee del udp
		Dwrite(s2,buf); //escribe en el tcp
	}
}

int main(){
	//nose que se le pone a la parte de server
	//talvez estas variables tengan que ser globales
	

	

	p1=pthreadcreate(funcionTCP,server, PORTTCP);
	p2=pthreadcreate(funcionUDP,server, PORTUDP);

	//falta enterrar a los thread y cachar cuando hay q enterrarlos

	return 0;
}

