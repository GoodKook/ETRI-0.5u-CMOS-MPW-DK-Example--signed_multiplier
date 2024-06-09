//
// Poorman's Standard-Emulator by GoodKook, goodkook@gmail.com
// Emulation Interface to "signed_mult8"/Cycle Accurate
//

#ifndef _SIGNED_MULT8_H_
#define _SIGNED_MULT8_H_

#include "systemc"

// Includes for accessing Arduino via serial port
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

SC_MODULE(signed_mult8)
{
    // PORTS
    sc_in<bool>             clk;
    sc_in<sc_uint<8> >      ABin;
    sc_in<bool>             ABSel;
    sc_out<sc_uint<16> >    RES;

    // Arduino Serial IF
    int fd;                 // Serial port file descriptor
    struct termios options; // Serial port setting

#define N_TX    2   // Number of Bytes to DUT's inputs
#define N_RX    2   // Number of Bytes from DUT's outputs

    // Thread: UART Driver for emulating DUT ----------------------------------
    void signed_mult8_thread(void)
    {
        uint8_t     x, y, txPacket[N_TX], rxPacket[N_RX];

        while(true)
        {
            // ----------------------------------------------------------------
            wait(clk.posedge_event());   //Posedge of Clock
            // DUT input: Byte order and Bitmap must match with Verilog wrapper
            txPacket[0] = (uint8_t)(ABin.read());
            txPacket[1] = (uint8_t)(ABSel.read()? 0x01:0x00);

            // Send to Emulator
            for (int i=0; i<N_TX; i++)
            {
                x = txPacket[i];
                while(write(fd, &x, 1)<=0)  usleep(1);
            }
            // Receive from Emulator
            for (int i=0; i<N_RX; i++)
            {
                while(read(fd, &y, 1)<=0)   usleep(1);
                rxPacket[i] = y;
            }
            RES.write(((uint16_t)rxPacket[1]<<8 | (uint16_t)rxPacket[0]));
        }
    }

    SC_CTOR(signed_mult8) :   // Constructor
        clk("clk")
    {
        SC_THREAD(signed_mult8_thread);  // UART Driver for emulating DUT
        sensitive << clk;
        
        // Arduino DUE: initialize USB-UART
        //fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
        if (fd < 0)
        {
            perror("Error opening serial port");
            return;
        }
        // Set up serial port
        options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        // Apply the settings
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &options);

        // Establish Contact
        int len = 0;
        char rx;
        while(!len)
            len = read(fd, &rx, 1);
        if (rx=='A')
            write(fd, &rx, 1);
        printf("Connection established...\n");
    }

};

#endif

