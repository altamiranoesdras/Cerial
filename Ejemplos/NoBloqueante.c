#define __WINDOWS_COM__                 /* #define __LINUX_COM__ */

#include "com/serial.h"

int main()
{
    HANDLE fd;
    DCB OldConf;
    char cad[16]="Enviando Texto";
    int n;

    fd=Open_Port("COM1");               /* Abre el puerto serie
                                           fd=Open_Port("/dev/ttyS0"); */

    OldConf=Get_Configure_Port(fd);     /* guardo la configuración del puerto */

    Configure_Port(fd,B115200,"8N1");   /* Configuro el puerto serie */

    IO_Blocking(fd,FALSE);              /* Seleccionamos lectura no bloquenate */

    n=Write_Port(fd,cad,16);            /* Escribo en el puerto serie*/

    while(Kbhit_Port(fd)<16);           /* Espero a leer hasta que se tengan
                                           16 bytes en el buffer de entrada */
    n=Read_Port(fd,cad,16);             /* Leo el puerto serie */

    Set_Configure_Port(fd,OldConf);     /* Restituyo la antigua configuración
                                           del puerto */
    Close_Port(fd);                     /* Cierro el puerto serie */

    printf("Presione ENTER para terminarn");
    getchar();

    return 0;
}