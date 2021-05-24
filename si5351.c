#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "SI5351A-Revb-Registers.h"
#define	I2C_FNAME	"/dev/i2c-2"
#define	SI5351_ADDR	0x60

int	i2c_file;

void i2c_init()
{
	i2c_file = open(I2C_FNAME, O_RDWR);
	if (i2c_file < 0)
		exit(1);
}

int i2c_read(unsigned char reg)
{
	if (ioctl(i2c_file, I2C_SLAVE, SI5351_ADDR) < 0) 
		exit(1);

	int res;

	/* Using SMBus commands */
	res = i2c_smbus_read_byte_data(i2c_file, reg);
	if (res < 0) 
		exit(1);
	else 
		printf("r dev(0x%x) reg(0x%x)=0x%x\n", SI5351_ADDR, reg, res);
	return res;
}

int i2c_write(unsigned char reg,__u8 value)
{
	if (ioctl(i2c_file, I2C_SLAVE, SI5351_ADDR) < 0) 
		exit(1);
	i2c_smbus_read_byte_data(i2c_file, reg,value);
}

int
main()
{
	i2c_init();
	i2c_read(0);
	i2c_write(3,0xFF);
	for(int i=0;i<SI5351A_REVB_REG_CONFIG_REGS;i++){
		i2c_write(si5351_revb_registers[i].address,si5351a_revb_registers[i].value);
		i2c_write(177,0xAC);
		i2c_write(3,0x00);
	}
}
