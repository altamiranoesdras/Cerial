#include "com/serial.h"

int numero=0;
SERIAL_PORT_EVENT(HANDLE * hPort)
{
    char c;
    Getc_Port(*hPort,& c);
    printf("[%d]=%cn",numero,c);
    numero++;
}

int main()
{
    HANDLE fd;
    DCB OldConf;
    char cad[16]="Enviando Texton";
    int n;

    fd=Open_Port("COM1");               /* Abre el puerto serie
                                           fd=Open_Port("/dev/ttyS0"); */

    OldConf=Get_Configure_Port(fd);     /* guardo la configuracion del puerto */

    Configure_Port(fd,B115200,"8N1");   /* Configuro el puerto serie */

    IO_Blocking(fd,TRUE);               /* Bloqueante por defecto, pero tambien
                                           se puede usar
                                           IO_Blocking(fd,TRUE); */

    n=Write_Port(fd,cad,16);            /* Escribo en el puerto serie*/

    Create_Thread_Port(& fd);        /* Creo un hilo y le paso el manejador */

    while(TRUE);

    Set_Configure_Port(fd,OldConf);     /* Restituyo la antigua configuracion
                                           del puerto */
    Close_Port(fd);                     /* Cierro el puerto serie */

    printf("Presione ENTER para terminarn");
    getchar();

    return 0;
}