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
	while(hay_algo_que_leer_del_archivo){
		char paquete[1024];
		paquete=leer_1024_bytes_del_archivo();
		n=Write_Port(fd,paquete,1024);	 // Escribo en el puerto serie
	}
	cerrar_puerto_serie();
	
}