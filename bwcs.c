
#include <stdio.h>
#include <stdlib.h>
#include "jsocket6.h"

#define PORTUDP "2000"
#define PORTTCP "2001"

int funcionTCP(void *puerto){
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
    ///////////////aqui se acaba lo que copie de bws///////////

}


//esta funcion la podemos copiar de bws porque es una conexion con un tcp
int funcionUDP(){
	s1=connectUDP(2000);
	s2=connectTCP(2001);
	while(true){
		read(s1,buf); //lee del udp
		Dwrite(s2,buf); //escribe en el tcp
	}
}

int main(){
	Dbind(funcionUDP,PORTUDP)

	p1=pthreadcreate(funcionTCP)
	p2=pthreadcreate(funcionUDP)

	//falta enterrar a los thread y cachar cuando hay q enterrarlos

	return 0;
}

