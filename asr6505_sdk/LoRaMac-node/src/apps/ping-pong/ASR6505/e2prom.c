#include "e2prom.h"

uint8_t e2prom_read_byte;

void e2prom_byte_rxtx(uint32_t addr, uint8_t c){//e2prom start at 0x1000
	FLASH_DeInit();
	FLASH_Unlock(FLASH_MemType_Data);
	FLASH_EraseByte(addr);         
	FLASH_ProgramByte(addr, c);  
	e2prom_read_byte = FLASH_ReadByte(addr);
}
