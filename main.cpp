#include "mbed.h"

#include "mbed_rpc.h"

//----------ACC------------------------------------------------------
#include "fsl_port.h"

#include "fsl_gpio.h"

#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7


I2C i2c( PTD9,PTD8);

int m_addr = FXOS8700CQ_SLAVE_ADDR1;

void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);

void FXOS8700CQ_writeRegs(uint8_t * data, int len);

//----------ACC------------------------------------------------------


RawSerial pc(USBTX, USBRX);

RawSerial xbee(D12, D11);

void ACC(void);

float vel_x = 0.0;

float vel_y = 0.0;

int main(){

  EventQueue queue;

  pc.baud(115200);

  // XBee setting

  xbee.baud(9600);

  xbee.printf("+++");

  xbee.printf("ATMY 0x40\r\n");

  xbee.printf("ATDL 0x39\r\n");

  xbee.printf("ATID 0x1\r\n");

  xbee.printf("ATWR\r\n");

  xbee.printf("ATCN\r\n");

  // start

  queue.call_every(100, ACC);   // call in every 100 millisec = 0.1 sec

  queue.dispatch();
}


void ACC (void)
{

   uint8_t who_am_i, value[2], res[6];
   int16_t acc16;
   float t[3];


   // Enable the FXOS8700Q
   FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &value[1], 1);
   value[1] |= 0x01;
   value[0] = FXOS8700Q_CTRL_REG1;
   FXOS8700CQ_writeRegs(value, 2);

   // Get the slave address
   FXOS8700CQ_readRegs(FXOS8700Q_WHOAMI, &who_am_i, 1);

   //---------------FXOS8700CQ--------------------------------------------
    FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);

    acc16 = (res[0] << 6) | (res[1] >> 2);

    if (acc16 > UINT14_MAX/2)

        acc16 -= UINT14_MAX;

    t[0] = ((float)acc16) / 4096.0f;


    acc16 = (res[2] << 6) | (res[3] >> 2);

    if (acc16 > UINT14_MAX/2)

        acc16 -= UINT14_MAX;

    t[1] = ((float)acc16) / 4096.0f;


    acc16 = (res[4] << 6) | (res[5] >> 2);

    if (acc16 > UINT14_MAX/2)

        acc16 -= UINT14_MAX;

    t[2] = ((float)acc16) / 4096.0f;

    //------------------FXOS8700CQ-----------------------------------------

    vel_x = float((t[0] * 9.8) * 0.1);   

    vel_y = float((t[1] * 9.8) * 0.1);

    pc.printf("%1.4f\r\n%1.4f\r\n", vel_x, vel_y);

    if (vel_x >= 0)
      xbee.printf(" %1.4f", vel_x);
    else
      xbee.printf("%1.4f", vel_x);
    
    if (vel_y >= 0)
      xbee.printf(" %1.4f", vel_y);
    else
      xbee.printf("%1.4f", vel_y);

}

void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char t = addr;

   i2c.write(m_addr, &t, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}