#define __LINUX_COM__
#include "com/serial.h"
#include <stdio.h>    // Libreria estanda entrada y salida
#include <unistd.h>   // para utilizar usleep()
#include <getopt.h>   // para leer la opciones que se pasan como argumentos
#include <string.h>   // para manejar cadenas de texto
#include <stdlib.h>


void error(char* msg){
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

void modoUso(void){
    printf("Modo de uso: ./Receptor -b <bps> -p <puerto> [Opciones]\n"
    "\n"
    "Opciones:\n"
    "  -h, --help                 Imprime modo de uso\n"
    "  -b, --baud=baudrate        Baudrate (bps) (velocidad) (9600 por defecto)\n"
    "  -p, --port=serialport      Puerto o adaptador conectado (\"/dev/ttyUSB0\")\n"
    "  -r, --receive              Recibe texto\n"
    "  -R, --receive-file=string  Recibe un archivo\n"
    "\n"
    "Nota: El orden es importante. Configure '-b' baudrate antes de arbrir el puerto '-p'. \n"
    "\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){

	const int bufmax = 256;//maximo tamaño de las cadenas

	HANDLE fd = -1; //manejador
    DCB OldConf; //configuracion antigua
   	
    char strport[bufmax]="/dev/ttyUSB0";
    int  velocidad = 9600;  // default
   	char texto[bufmax]="Hola Mundo";
   	char rutaGuarda[bufmax]="";
   	int n;
    
    

	if (argc==1){
		modoUso();
	}

	/* Parseo de opciones */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",          no_argument,       0, 'h'},
        {"port",          required_argument, 0, 'p'},
        {"baud",          required_argument, 0, 'b'},
        {"receive",       no_argument,       0, 'r'},
        {"receive-file",  required_argument, 0, 'R'},
        {NULL,                            0, 0,  0 }
    };

    while(1) {

    	//le los argumetos y/o opciones
        opt = getopt_long (argc, argv, "hp:b:s:rR:",loptions, &option_index);

        //cuando ya no hay mas opciones o argumento que ler
        if (opt==-1) break;

        switch (opt) {
	        case '0': break;
	 
	        case 'h':
	            modoUso();
	        break;

	        case 'b':
	            velocidad = strtol(optarg,NULL,10);
	        break;

	        case 'p':

	        	//si el puerto esta abierto
		        if( fd!=-1 ) {
	                //Set_Configure_Port(fd,OldConf);//restaura configuracion        
    				Close_Port(fd);
	                printf("Cerando puerto... %s\n",strport);
	            }    

	            strcpy(strport,optarg);
	            fd=Open_Port(strport); 
	            //OldConf=Get_Configure_Port(fd); // guardo la configuracion del puerto 

	            //si no se pudo abrir el puerto
	            if (fd == -1) exit(EXIT_SUCCESS);

	            Configure_Port(fd,B9600,"8N1");   

	        break;
	        
	        case 'r':
	        	if( fd == -1 ) error((char*) "El puerto serial no esta abierto");

	        	printf("Esperando texto del emisor..\n");

	        	//Esperar hasta que se llene el bufer
	        	while(Kbhit_Port(fd)<bufmax);

    			n=Read_Port(fd,texto,bufmax);

	        	//strcpy(texto,optarg);
	        	printf("El texto recibido es: %s ...\n",texto);
          
	            Close_Port(fd);	            
            break;

            case 'R':
				if( fd == -1 ) error((char*) "El puerto serial no esta abierto");

	        	strcpy(rutaGuarda,optarg);
	        	printf("Recibiendo archivo...\n");
	            
            break;
        
        }
    }

    exit(EXIT_SUCCESS);
}

/*
main()
{
	abrir_puerto_serie();	
	while (no_quiera_salir){
		while(Kbhit_Port(fd)<1024); //Espero a leer 1024 bytes
		char paquete[1024];
		n=Read_Port(fd,paquete,1024);    // Leo el puerto serie 
	}
	Cuando_tenga_el_contenido_de_todo_el_archivo = Escribir en la pc2 el archivo;
	cerrar_puerto_serie();
	
}*/