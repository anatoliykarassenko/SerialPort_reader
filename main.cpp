#include "windows.h"
#include <iostream>
#include "parser.hpp"
#include "events.h"
#include "eventName.hpp"
#include "stdio.h"
#include <fstream>
#include "io.h"


HANDLE      hSerial = NULL;
uint8_t     Buffer[256]; //Buffer for storing Rxed Data


void CleanBuffer(uint8_t []) // clearing the buffer
{
      for(int i = 0; i < 256; i++)
      {
                Buffer [i] = 0;
      }
      std::cout << "Buffer is cleared" << "\n" << std::endl;
}

 void handleEvent(const EventUnion *ev)
{
	switch (ev->event.num)
	{
		case EV_RAW_ACCEL:
			printf("%s %d %-5d %-5d %-5d\n", eventName(ev), ev->event.addr, ev->ans.i16[0], ev->ans.i16[1], ev->ans.i16[2]);
			break;
		default:
			printf("%s %d\n", eventName(ev), ev->event.addr);
			break;
	}
}

int main()
{
    LPCTSTR portName="COM5";
    BOOL    Status;
    Parser parser;
    std::ofstream writeto("logs.txt"); // saving logs

    //open port
    hSerial=::CreateFile( portName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error in opening serial port" << std::endl;
        writeto << "Error in opening serial port" << std::endl;
    }
    else
    {
        std::cout << "opening serial port successful" << std::endl;
        writeto << "opening serial port successful" << std::endl;
    }

    /*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/
    DCB dcbSerialParams;
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

    Status = GetCommState(hSerial, &dcbSerialParams);
    if (Status == false)
    {
        writeto << "error in GetCommState\n";
    }
    else
    {
        dcbSerialParams.BaudRate = CBR_57600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity   = NOPARITY;
        Status = SetCommState(hSerial, &dcbSerialParams);

            if(Status == false)
            {
                writeto << "error in setting DCB Structure\n";
            }

            else //If Successfull display the contents of the DCB Structure
            {
              std::cout <<"\n\n    Setting DCB Structure Successfull\n";
              std::cout <<"\n      BaudRate = " << dcbSerialParams.BaudRate;
              std::cout <<"\n      ByteSize = " << int(dcbSerialParams.ByteSize);
              std::cout <<"\n      StopBits = " << int(dcbSerialParams.StopBits);
              std::cout <<"\n      Parity   = " << int(dcbSerialParams.Parity);
              std::cout << "\n\n";

              writeto <<"\n\n    Setting DCB Structure Successfull\n";
              writeto <<"\n      BaudRate = " << dcbSerialParams.BaudRate;
              writeto <<"\n      ByteSize = " << int(dcbSerialParams.ByteSize);
              writeto <<"\n      StopBits = " << int(dcbSerialParams.StopBits);
              writeto <<"\n      Parity   = " << int(dcbSerialParams.Parity);
              writeto << "\n\n";
            }
    }

      unsigned char TempChar; //Temporary character used for reading
//      DWORD     BytesReaded;
      int       i = 0;

       CleanBuffer(Buffer);
       std::ofstream supp_info_output("file_log_bytes",  std::ios::out | std::ios::binary); // saving binary file

    int fh1;
    for (int len = _read(fh1, Buffer, sizeof(Buffer)); len > 0; len--)
    {
                Buffer[i] = TempChar;

                writeto <<"\n\n";

                std::cout << std::hex << int(Buffer[i]) << " ";
                writeto << std::hex << int(Buffer[i]) << " ";
                supp_info_output.write((char *)(&Buffer[i]), sizeof( TempChar ));
    }

        uint8_t *p       = Buffer;
		int    received = 0;

        int len = sizeof(Buffer);
        for (int d = parser.parse(p, len, received); d < len; d++)
        {
            len -= d;
            p += d;
        }
            if (received)
            {
                handleEvent(parser.event());
            }
            printf("received = %d", received);
                writeto.close();
                supp_info_output.close();

CloseHandle(hSerial);
}
