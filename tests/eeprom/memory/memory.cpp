#include "memory.h"
  
void storeParam(int storeAddress, int data)
{
  
  byte dataToStore[2] = {byte(data >> 8), byte(data)};
  hbad_mem.write(storeAddress, dataToStore, 2);
  
}

int retrieveParam(int address)
{
  
  byte retrievedData[2];
  hbad_mem.read(address, retrievedData, 2);  
  return ((int)(retrievedData[0] << 8) + (int) retrievedData[1]);
}

 //Below function performs read/write functionality in the EEPROM
 //It take the following arguments 
 // Address = The address where read/write functionality has to be performed 
 // *data = the pointer to the data, will be filled in in case of read and will be read from in case of write
 // size = size of data pointed by *data
 // ops = operation select (defined by enum EEPROM_READ, EEPROM_WRITE)

int eeprom_ext_rw(unsigned int address, int *data, unsigned int size, eeprom_ops ops)
{
    int err = 0;
    unsigned int i=0;
    
	
    if ((data == NULL) || (size == 0) || (size > EEPROM_MAX_SIZE)) 
    {
        err = -1;
        goto err_ret;
    }
    
    if (ops == EEPROM_READ)
    {
        for (i=0; i<size; i++)
        {
            *data = EEPROM.read(address);
            return *data;
            data++;
			address++;
        }
    }
    else if (ops == EEPROM_WRITE)
    {
        for (i=0; i<size; i++)
        {
            EEPROM.write(address, *data);
            data++;
			address++;
        }
    }
    else
    {
        err = -1;
    }
	
err_ret:
    return err;
}


