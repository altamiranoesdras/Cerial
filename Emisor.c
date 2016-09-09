#define __LINUX_COM__
#include "com/serial.h"


int main(int argc, char const *argv[]){

	HANDLE fd;
    DCB OldConf;
    char cad[16]="Enviando Texto";
    int n;

	if (argc<2){
		printf("Faltan parametros de conexión\n");
		return 0;
	}

	printf("Abriendo puerto serie...\n");
	
	fd=Open_Port((char*) argv[1]);         
  
	printf("Configurando puerto serie...\n");
	Configure_Port(fd,B9600,(char*) "8N1");   
    
	/* code */
	return 0;
}