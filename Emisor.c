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
    printf("Modo de uso: ./Emisor -b <bps> -p <puerto> [Opciones]\n"
    "\n"
    "Opciones:\n"
    "  -h, --help                 Imprime modo de uso\n"
    "  -b, --baud=baudrate        Baudrate (bps) (velocidad) (9600 por defecto)\n"
    "  -p, --port=serialport      Puerto o adaptador conectado (\"/dev/ttyUSB0\")\n"
    "  -k, --keep                 Mantien enviando el texto en intervalos de un segundo\n"
    "  -s, --send=string          Envia una cadena de texto\n"
    "  -f, --send-file=ruta       Envia un archivo de texto\n"
 
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
   	char rutaArchivo[bufmax]="";
   	int keep=0;
   	char sn[2]="S";
   	int i=0;
    
    

	if (argc==1){
		modoUso();
		printf("Faltan parametros de conexión\n");
	}

	/* Parseo de opciones */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"keep",       required_argument, 0, 'k'},
        {"send",       required_argument, 0, 's'},
        {"sendFile",   required_argument, 0, 'f'},
        {NULL,         0,                 0, 0}
    };

    while(1) {

    	//le los argumetos y/o opciones
        opt = getopt_long (argc, argv, "hp:b:ks:f:",loptions, &option_index);

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

	            Configure_Port(fd,velocidad,(char*) "8N1");   

	        break;

	        case 'k':
	        	keep=1;
	        	break;
	        
	        case 's':
	        	if( fd == -1 ) error((char*) "El puerto serial no esta abierto");

	        	strcpy(texto,optarg);
	        		
	        	
	        	do
	        	{
		        	
	        		printf("Enviando texto: %s ...\n",texto);
	        		Write_Port(fd,texto,bufmax);
	        		usleep(1000 * 1000 ); // sleep milliseconds            

	        		if(i==15){

	        			printf("Desea continuar enviando \n");
	        			scanf("%s",sn);

	        			if(strcmp(sn,"n")==0 || strcmp(sn,"N")==0) 
	        				keep=0;
	        			else
	        				i=0;
	        		}

	        		i++;
		        		        		
	        	} while (keep);



	            Close_Port(fd);
	            
            break;

            case 'f':
				if( fd == -1 ) error((char*) "El puerto serial no esta abierto");

	        	strcpy(rutaArchivo,optarg);
	        	printf("Enviando archivo: %s ...\n",rutaArchivo);
	            
            break;
        
        }
    }

    exit(EXIT_SUCCESS);
}