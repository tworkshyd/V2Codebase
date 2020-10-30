#include <extEEPROM.h>
#include <EEPROM.h>



/*!EEPROM I2C Address  */
#define EEPROM_I2C_ADDR 0x50 //80

#define EEPROM_DEFAULT_DATA_WRITE_ADDR 0xA //10


#define EEPROM_ADDR 0xC //12


#define EEPROM_MAX_SIZE 4096  //max size of the EEPROM i.e. 4K bytes


typedef enum
{
    EEPROM_READ,
    EEPROM_WRITE
} eeprom_ops;


//const uint32_t totalKBytes = 64;

/*!device size, number of devices, page size< */
extEEPROM hbad_mem(kbits_256, 1, 64, EEPROM_I2C_ADDR);


void storeParam(int storeAddress, int data);

int retrieveParam(int address);


int eeprom_ext_rw(unsigned int address, int *data, unsigned int size, eeprom_ops ops);


/**@}*/
