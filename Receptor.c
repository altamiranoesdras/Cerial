#include "com/serial.h"

 HANDLE fd;  //manejador del puerto
 DCB OldConf; //Estructura de configuracion del puerto
 
 //******************************************* procedimiento abrir_puerto_serie ****************************************

void abrir_puerto_serie(){
	fd=Open_Port("/dev/ttyS0");         // Abre el puerto serie        
    	OldConf=Get_Configure_Port(fd);     // guardo la configuracion del puerto 
    	Configure_Port(fd,B115200,"8N1");   // Configuro el puerto serie 
}


//******************************************* procedimiento cerrar_puerto_serie ****************************************

void cerrar_puerto_serie(){
    	Set_Configure_Port(fd,OldConf);     // Restituyo la antigua configuracion del puerto 
    	Close_Port(fd); 		    // Cierro el puerto serie     
}

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
	
}