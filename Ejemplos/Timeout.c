#define __WINDOWS_COM__                 /* #define __LINUX_COM__ */

#include "com/serial.h"

int main()
{
    HANDLE fd;
    DCB OldConf;
    char cad[16]="Enviando Texto";
    int n,TIME=2;

    fd=Open_Port("COM1");               /* Abre el puerto serie
                                           fd=Open_Port("/dev/ttyS0"); */

    OldConf=Get_Configure_Port(fd);     /* guardo la configuracion del puerto */

    Configure_Port(fd,B115200,"8N1");   /* Configuro el puerto serie */

    Set_Time(fd,TIME);                  /* time-out entre caracteres es TIME*0.1*/

    n=Write_Port(fd,cad,16);            /* Escribo en el puerto serie*/

    n=Read_Port(fd,cad,16);             /* Leo el puerto serie */

    Set_Configure_Port(fd,OldConf);     /* Restituyo la antigua configuracion
                                           del puerto */
    Close_Port(fd);                     /* Cierro el puerto serie */

    printf("Presione ENTER para terminarn");
    getchar();

    return 0;
}