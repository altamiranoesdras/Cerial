 /*
  *  LnxCOMM is a Serial Port Library
  *  Copyright (C) 2008 2010 Fernando Pujaico Rivera <fernando.pujaico.rivera@gmail.com>
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2, or (at your option)
  *  any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
  *
  */

/** \file 
 *  \brief Archivo usado para un S.O. Windows.
 *
 *  Este archivo contiene la definición de las funciones de 
 *  de acceso al puerto serie, destinadas a S.O. tipo Windows.
 *
 */

/** \defgroup HeaderWindows Funciones para Windows
 * @{
 */

#ifndef __SERIE_WINDOWS__
#define __SERIE_WINDOWS__

    #include <stdio.h>   
    #include <string.h>  
    #include <windows.h>
    #include "baudios.h"
    
    #define MAX_SIZE_BUFFER 8192

    #define NONE      0    
    #define RTSCTS    1
    #define HARD      1
    #define XONXOFF   2
    #define SOFT      2
    
    BOOL ERROR_CONFIGURE_PORT=FALSE;

    HANDLE Open_Port(char COMx[])
    {
         HANDLE fd;
         fd = CreateFile( COMx,	          	   // pointer to name of the file 
	             GENERIC_READ | GENERIC_WRITE, // access (read-write) mode 
	             0,				   // share mode 
	             NULL,			   // pointer to security attributes 
	             OPEN_EXISTING,		   // how to create
	             0,				   // file attributes
	             NULL);			   // handle to file with attributes
                                                   // to copy 

         if (fd == INVALID_HANDLE_VALUE) 
         {
              printf("Error:No se puede abrir puerto: %s \n",COMx); 
              return INVALID_HANDLE_VALUE;
         } 
         return (fd);
    }

    DCB Get_Configure_Port(HANDLE fd)
    {
         DCB PortDCB;
         PortDCB.DCBlength = sizeof (DCB);     
         
         if(!GetCommState (fd, &PortDCB))
         {
               printf("Error pidiendo la configuración de puerto serie.\n");
               ERROR_CONFIGURE_PORT=TRUE; 
               return PortDCB;        
         }
         ERROR_CONFIGURE_PORT=FALSE;
         return PortDCB;
    }
    
    
    DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])
    {
         DCB PortDCB;
         PortDCB.DCBlength = sizeof (DCB); 
         
         // Configuramos el tamaño del buffer de escritura/lectura
         if(!SetupComm(fd, MAX_SIZE_BUFFER, MAX_SIZE_BUFFER)) 
         {
               printf("Error configurando buffer\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return PortDCB;
         }

         if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuración de Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return PortDCB;
         }

         // Change the DCB structure settings.
         PortDCB.BaudRate = BaudRate;             // Current baud 
         PortDCB.fBinary = TRUE;                  // Binary mode; no EOF check 
         //PortDCB.EofChar = 0;
         PortDCB.fErrorChar = FALSE;              // Disable error replacement. 
         PortDCB.fNull = FALSE;                   // Disable null stripping. 
         PortDCB.fAbortOnError = FALSE;           // Do not abort reads/writes on error.
         PortDCB.fParity = FALSE;                 // Disable parity checking.
         
         PortDCB.fRtsControl = RTS_CONTROL_DISABLE; // RTS flow control   
         
         PortDCB.fDtrControl = DTR_CONTROL_DISABLE; // DTR flow control type        

         PortDCB.fOutxCtsFlow = FALSE;            // No CTS output flow control 
         PortDCB.fOutxDsrFlow = FALSE;            // No DSR output flow control

         PortDCB.fDsrSensitivity = FALSE;         // DSR sensitivity 

         PortDCB.fOutX = FALSE;                   // No XON/XOFF out flow control 
         PortDCB.fInX = FALSE;                    // No XON/XOFF in flow control 
         PortDCB.fTXContinueOnXoff = TRUE;        // XOFF continues Tx 
         
         if(strncmp(CharParity,"8N1",3)==0)
         {
         PortDCB.ByteSize = 8;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = NOPARITY;               // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7E1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = EVENPARITY;             // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7O1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = ODDPARITY;              // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7S1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = SPACEPARITY;            // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }   
         
         if (!SetCommState (fd, &PortDCB))
         {  // Could not configure the serial port.
            printf("Error: configurando puerto\n");
            ERROR_CONFIGURE_PORT=TRUE;
            return PortDCB;
         }

         // Configure timeouts 
         COMMTIMEOUTS timeouts;
         // No timeouts 
         timeouts.ReadIntervalTimeout = 0;
         timeouts.ReadTotalTimeoutMultiplier = 0;
         timeouts.ReadTotalTimeoutConstant = 0;
         timeouts.WriteTotalTimeoutMultiplier = 0;
         timeouts.WriteTotalTimeoutConstant = 0;

         if (!SetCommTimeouts(fd, &timeouts)) 
         {
	        printf("ERROR: No se pudo poner SetCommTimeouts: %s\n", 
                                                               GetLastError());
            ERROR_CONFIGURE_PORT=TRUE;
            return PortDCB;
         }


         ERROR_CONFIGURE_PORT=FALSE;
	     return PortDCB;
    }


    int Set_Configure_Port(HANDLE fd,DCB PortDCB)
    {
	 // Ahora limpiamos el buffer de entrada y salida del puerto 
         // y activamos la configuración del puerto.
         if (!SetCommState (fd, &PortDCB))
         {
            printf("ERROR: No se pudo poner configuración del puerto serie\n" );
            ERROR_CONFIGURE_PORT=TRUE;
            return FALSE;
         }
         ERROR_CONFIGURE_PORT=FALSE;

	     return TRUE;
    }    

    
    long Write_Port(HANDLE fd,char Data[],int SizeData)
    {
         long n;

         WriteFile(fd,                // Port handle
                   Data,              // Pointer to the data to write 
                   (DWORD)SizeData,   // Number of bytes to write
                   (DWORD*)&n,        // Pointer to the number of bytes written
                   NULL);             // Must be NULL for Windows CE
         return n;
    }
    

    long Read_Port(HANDLE fd,char *Data,int SizeData)
    {
         long n;

         ReadFile (fd,                // Port handle
                   Data,              // Pointer to the data to write 
                   (DWORD)SizeData,   // Number of bytes to write
                   (DWORD*)(&n),      // Pointer to the number of bytes read
                   NULL);             // Must be NULL for Windows CE
         return n;
    }


    long Gets_Port(HANDLE fd,char *Data,int SizeData)
    {
         long n=0,i=0;

         for(i=0;i<SizeData;i++)
         {
               ReadFile(fd,&Data[i],(DWORD)1,(DWORD*)(&n),NULL);
               if(((Data[i]==13)||(Data[i]==10)||(Data[i]==0))&&(i!=0))
               {
                n=i+1;
                if(n<SizeData) Data[n]=0;
                i=SizeData;
               }
         }
         
         return n;
    }


    long Getc_Port(HANDLE fd,char *Data)
    {
         long n;

         ReadFile(fd,Data,(DWORD)1,(DWORD*)(&n),NULL);
        
         return n;
    }

  
    int Kbhit_Port(HANDLE fd)
    {
     	DWORD x;
     	COMSTAT cs;
     	// Actualizar COMSTAT, sirve para averiguar el número de bytes en el 
     	// buffer de entrada:
     	ClearCommError(fd, &x, &cs);
     	return cs.cbInQue;
    }



    int Close_Port(HANDLE fd)
    {
         if (fd != INVALID_HANDLE_VALUE)
         {  // Close the communication port.
            
            // Liberar máscara de eventos del puerto serie:
            SetCommMask(fd, 0);
            
            if (!CloseHandle (fd))
            {printf("Error cerrando el puerto serie\n");return FALSE;}
            else
            {fd = INVALID_HANDLE_VALUE;return TRUE;}
         }
	     return FALSE;
    }


 
    int Set_Hands_Haking(HANDLE fd,int FlowControl)
    {
        DCB PortDCB;
        if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuración de puerto serie\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return FALSE;
         }
        
        switch(FlowControl)
        {
         case 0: ///NONE
              {
              PortDCB.fOutX = FALSE;                      // No XON/XOFF out flow control. 
              PortDCB.fInX = FALSE;                       // No XON/XOFF in flow control.
              
              PortDCB.fRtsControl = RTS_CONTROL_ENABLE;   // RTS flow control.  
               
              PortDCB.fDtrControl = DTR_CONTROL_ENABLE;   // DTR flow control type.
              break;
              }
         case 1: ///RTS/CTS
              {
              PortDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;// RTS flow control.  
              break;
              }
         case 2: ///XON/OFF
              {
              PortDCB.fOutX = TRUE;                       // XON/XOFF out flow control. 
              PortDCB.fInX = TRUE;                        // XON/XOFF in flow control.  
              PortDCB.XonChar=0x11;                       // ASCII_XON.
              PortDCB.XoffChar=0x13;                      // ASCII_XOFF.
              PortDCB.XonLim=100;
              PortDCB.XoffLim=100;
              break;
              }
         case 3: ///DTR/DSR
              {
              PortDCB.fDtrControl = DTR_CONTROL_HANDSHAKE;// DTR flow control type.
              break;
              }
        }
        
        if (!SetCommState (fd, &PortDCB))
        {
              printf("ERROR: Configurando el puerto serie\n");
              ERROR_CONFIGURE_PORT=TRUE;
              return FALSE;
        }
        ERROR_CONFIGURE_PORT=FALSE;

	    return TRUE;
    }
    

      
    int Set_RThreshold(HANDLE fd,int n)
    {
	    return TRUE;
    }
    
  
    int Set_BaudRate(HANDLE fd,unsigned int BaudRate)
    {
        DCB PortDCB;

        if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuración del Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return FALSE;
         }
         
        PortDCB.BaudRate = BaudRate;                // Binary mode; no EOF check 

        if (!SetCommState (fd, &PortDCB))
        {
              printf("Error configurando el BaudRate\n");
              ERROR_CONFIGURE_PORT=TRUE;
              return FALSE;
        }
        ERROR_CONFIGURE_PORT=FALSE;

	    return TRUE;
    }

  
    int Set_Time(HANDLE fd,unsigned int Time) 
    {
        COMMTIMEOUTS CommTimeouts;

        if(!GetCommTimeouts (fd, &CommTimeouts))
        {
              printf("Error obteniendo configuración time-out actual: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        // Tiempo maximo en mseg. entre caracteres consecutivos
        CommTimeouts.ReadIntervalTimeout = Time*200;

        // Time-Out=TotalTimeoutMultiplier*number_of_bytes+TotalTimeoutConstant

        // Especifique el multiplicador de tiempo fuera de lectura con el miembro 
        // ReadTotalTimeoutMultiplier. En cada operación de lectura , este número 
        // se multiplica por el número de bytes que la lectura espera recibir .
        CommTimeouts.ReadTotalTimeoutMultiplier = Time*100;
        // Constante a sumar al time-out total de recepción.
        CommTimeouts.ReadTotalTimeoutConstant = 0;

        // Igual que lectura.
        CommTimeouts.WriteTotalTimeoutMultiplier = Time*100;
        // Igual que lectura
        CommTimeouts.WriteTotalTimeoutConstant = 0;

        // Establecemos nuevos valores de time-out.
        if(!SetCommTimeouts (fd, &CommTimeouts)) 
        {
              printf("Error estableciendo nueva configuración time-out: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        return TRUE;
    }

 
    int IO_Blocking(HANDLE fd,int Modo) 
    {
        COMMTIMEOUTS CommTimeouts;

        if(!GetCommTimeouts (fd, &CommTimeouts))
        {
              printf("Error obteniendo configuracion time-out actual: %s\n", 
                                                              GetLastError());
              return FALSE;
        }
        
        // Especifica que la operación de lectura debe regresar inmediatamente 
        // con los caracteres que ya se hayan recibido, incluso aunque no se 
        // haya recibido ninguno.
        if(Modo==FALSE)// No bloqueante
        {
         CommTimeouts.ReadIntervalTimeout = MAXDWORD;
         CommTimeouts.ReadTotalTimeoutMultiplier = 0;
         CommTimeouts.ReadTotalTimeoutConstant = 0;
         CommTimeouts.WriteTotalTimeoutMultiplier = 0;
         CommTimeouts.WriteTotalTimeoutConstant = 0;
        }
        // indica que el tiempo total de time-out no se usa para operaciones de
        // lectura/escritura.
        if(Modo==TRUE)// Bloqueante.
        {
         CommTimeouts.ReadIntervalTimeout = 0;
         CommTimeouts.ReadTotalTimeoutMultiplier = 0;
         CommTimeouts.ReadTotalTimeoutConstant = 0;
         CommTimeouts.WriteTotalTimeoutMultiplier = 0;
         CommTimeouts.WriteTotalTimeoutConstant = 0;
        }

        if(!SetCommTimeouts (fd, &CommTimeouts)) 
        {
              printf("Error estableciendo nueva configuración bloqueante/no-bloqueante: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        return TRUE;
    }

  
    int Clean_Buffer(HANDLE fd) 
    {
       return PurgeComm( fd , PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
    }
    
 
    int Setup_Buffer(HANDLE fd,unsigned long InQueue,unsigned long OutQueue) 
    {
       return SetupComm(fd,InQueue,OutQueue);
    }

    /**************************************************************************/
    #ifdef ENABLE_SERIAL_PORT_EVENT  
    typedef  HANDLE pthread_t;
    
    void SERIAL_PORT_EVENT(HANDLE * hPort);
    
    void Thread_Port(HANDLE *hPort)
    {
        int n=0;

        printf("SERIAL_PORT_EVENT [OK]\n");

        do {
            if(Kbhit_Port(*hPort)!=0)
               SERIAL_PORT_EVENT(hPort);
        } while(TRUE);
    }
      
    pthread_t Create_Thread_Port(HANDLE *fd) 
    {
         pthread_t hilo;
         hilo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Port, fd, 0, NULL);
         return hilo;
    }
    #endif

#endif

/**
 * @}
 */
