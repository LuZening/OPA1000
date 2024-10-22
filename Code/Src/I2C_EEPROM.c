/*
 * I2C_EEPROM.c
 *
 *  Created on: 2022年4月6日
 *      Author: cpholzn
 */


#include "I2C_EEPROM.h"

#define AT24C_HOLDTIME 6
#define PAGE2ADDR(pEEPROM, p) ( (p) * page_sizes[(pEEPROM)->capacity] )


const int byte_capacity[] = {128, 256, 512, 1024, 2048, 4096, 8192};
const int page_sizes[] = {8, 8, 16, 16, 16, 32, 32};
const int n_pages[] = {16, 32, 32, 64, 128, 128, 256};


EEPROM_AT24_t EEPROM;

static void delayMicroseconds(uint16_t us)
{
	volatile uint32_t i = us * 50;
	while(--i);
}

void SET_PIN(GPIO_TypeDef* port, uint16_t pin)
{
	port->BSRR = pin;
}

void CLEAR_PIN(GPIO_TypeDef* port, uint16_t pin)
{
	port->BSRR = ((uint32_t)pin << 16U);
}

uint8_t GET_PIN(GPIO_TypeDef* port, uint16_t pin)
{
	return (port->IDR & pin) > 0;
}


uint8_t get_op_AT24C(EEPROM_AT24_t *pEEPROM, uint16_t addr)
{
	uint8_t op;
    if (pEEPROM->capacity <= AT24C02)
    {
        op = 0b10100000 | ((pEEPROM->addr_chip << 1) & 0b1110);
    }
    else if (pEEPROM->capacity == AT24C04)
    {
        op = 0b10100000 | ((pEEPROM->addr_chip << 2) & 0b1100) | ((addr >> 7) & 0b10);
    }
    else if (pEEPROM->capacity == AT24C08)
    {
        op = 0b10100000 | ((pEEPROM->addr_chip << 3) & 0b1000) | ((addr >> 7) & 0b110);
    }
    else if (pEEPROM->capacity == AT24C16)
    {
        op = 0b10100000 | ((addr >> 7) & 0b1110);
    }
    else
    {
        op = 0b10100000 | ((pEEPROM->addr_chip << 1) & 0b1110);
    }
    return op;
}

void WP_AT24C(EEPROM_AT24_t *pEEPROM)
{
    SET_PIN(pEEPROM->port_WP, pEEPROM->pin_WP);
}

void release_WP_AT24C(EEPROM_AT24_t *pEEPROM)
{
    CLEAR_PIN(pEEPROM->port_WP, pEEPROM->pin_WP);
}

static void start_AT24C(EEPROM_AT24_t *pEEPROM)
{
    // start
    SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME / 2);
    CLEAR_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME / 2);
}

static void stop_AT24C(EEPROM_AT24_t *pEEPROM)
{
    int SCL = pEEPROM->pin_SCL;
    int SDA = pEEPROM->pin_SDA;
    CLEAR_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME);
}

static void send_ACK_AT24C(EEPROM_AT24_t *pEEPROM)
{
	CLEAR_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);

}


static bool send_byte_AT24C(EEPROM_AT24_t *pEEPROM, uint8_t data)
{
    int SCL = pEEPROM->pin_SCL;
    int SDA = pEEPROM->pin_SDA;
    bool is_ACK = false;
    int i = 0;
    // send the byte from MSB
    for (i = 7; i >= 0; --i)
    {
        CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
        delayMicroseconds(AT24C_HOLDTIME);
        if((data >> i) & 0x01 > 0)
        	SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
        else
        	CLEAR_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
        delayMicroseconds(AT24C_HOLDTIME);
        SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
        delayMicroseconds(AT24C_HOLDTIME);
    }
    // ACK
    CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME);
    while ((GET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA) == 1) && (i++ < 1000))
    {
        delayMicroseconds(10);
    }
    if(i < 1000) is_ACK = true;
    SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    //Serial.printf("content: %d, %d\r\n", data, is_ACK);
    return is_ACK;
}

static uint8_t recv_byte_AT24C(EEPROM_AT24_t *pEEPROM)
{
    int i = 8;
    uint8_t data = 0;
    CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    while (i--)
    {
    	SET_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
        delayMicroseconds(AT24C_HOLDTIME);
        data <<= 1;
        data |= GET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
        CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
        delayMicroseconds(AT24C_HOLDTIME);
    }
    return data;
}

static bool ACK_polling_AT24C(EEPROM_AT24_t *pEEPROM)
{
    // send a dummy operation code
	uint8_t op = get_op_AT24C(pEEPROM, 0);
    bool is_ACK = false;
    int i = 0;
    do
    {
        start_AT24C(pEEPROM);
        is_ACK = send_byte_AT24C(pEEPROM, op);
        i++;
    }while((!is_ACK) && (i <= 100));
    stop_AT24C(pEEPROM);
    return is_ACK;
}

static void write_AT24C(EEPROM_AT24_t *pEEPROM, uint16_t addr, uint8_t data)
{
    start_AT24C(pEEPROM);
    send_byte_AT24C(pEEPROM, get_op_AT24C(pEEPROM, addr)); // write signal
    if (pEEPROM->capacity >= AT24C32)
    {
        // addr H
        send_byte_AT24C(pEEPROM, (addr >> 8) & 0xff);
    }
    // addr L
    send_byte_AT24C(pEEPROM, (addr & 0xff));
    // data
    send_byte_AT24C(pEEPROM, data);
    // stop
    stop_AT24C(pEEPROM);
}

static uint8_t read_AT24C(EEPROM_AT24_t *pEEPROM, uint16_t addr)
{
	uint8_t data;
	uint8_t op = get_op_AT24C(pEEPROM, addr);
    start_AT24C(pEEPROM);
    send_byte_AT24C(pEEPROM, op); // write signal for address setting
    if(pEEPROM->capacity >= AT24C32)
    {
        // addr H
        send_byte_AT24C(pEEPROM, (addr >> 8) & 0xff);
    }
    // addr L
    send_byte_AT24C(pEEPROM, (addr & 0xff));
    start_AT24C(pEEPROM);
    send_byte_AT24C(pEEPROM, op | 0x01); // read signal
    data = recv_byte_AT24C(pEEPROM);
    stop_AT24C(pEEPROM);
    return data;
}


// only compatible with 24C64 and above
static void read_array_AT24C(EEPROM_AT24_t *pEEPROM, uint16_t addr, unsigned char* data, int n)
{
    int i;
    uint8_t op = get_op_AT24C(pEEPROM, addr);
    start_AT24C(pEEPROM);
    send_byte_AT24C(pEEPROM, op); // write signal for address setting
    if(pEEPROM->capacity >= AT24C32)
    {
        // addr H
        send_byte_AT24C(pEEPROM, (addr >> 8) & 0xff);
    }
    // addr L
    send_byte_AT24C(pEEPROM, (addr & 0xff));
    start_AT24C(pEEPROM);
    send_byte_AT24C(pEEPROM, op | 0x01); // read signal
    data[0] = recv_byte_AT24C(pEEPROM);
    for(i=1; i<n; ++i)
    {
        // SEND ACK
        send_ACK_AT24C(pEEPROM);
        data[i] = recv_byte_AT24C(pEEPROM);
    }
    // NO ACK
    /*
    SET_PIN(pEEPROM->pin_SDA, HIGH);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->pin_SCL, HIGH);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->pin_SCL, LOW);
    delayMicroseconds(AT24C_HOLDTIME);
    */
    stop_AT24C(pEEPROM);
}

static void write_array_AT24C(EEPROM_AT24_t *pEEPROM, uint16_t page, unsigned char* data, size_t n)
{
    int i = 0;
    int page_size = page_sizes[(int)(pEEPROM->capacity)];
    // total number of pages the chip has
    int max_page = n_pages[(int)(pEEPROM->capacity)];
    // number of pages we will need to write data
    int n_page = n / page_size + ((n % page_size)?(1):(0));
    int addr = page * page_size;
    if (page + n_page > max_page) return;

    while(i < n)
    {
        // Change page
        if(i % page_size == 0)
        {
            if(i > 0)
            {
                stop_AT24C(pEEPROM);
                ACK_polling_AT24C(pEEPROM);
                //Serial.println(addr);
                addr += page_size;
            }
            start_AT24C(pEEPROM);
            send_byte_AT24C(pEEPROM, get_op_AT24C(pEEPROM, addr)); // write
            if(pEEPROM->capacity >= AT24C32)
            {
                // addr H
                send_byte_AT24C(pEEPROM, (addr >> 8) & 0xff);
            }
            // addr L
            send_byte_AT24C(pEEPROM, (addr & 0xff));
        }
        // write data continously on a single page
        send_byte_AT24C(pEEPROM, data[i]);
        ++i;
    }
    stop_AT24C(pEEPROM);

}



void I2C_EEPROM_init(EEPROM_AT24_t *pEEPROM, AT24C_Cap_t capacity,
		GPIO_TypeDef* port_SCL,	uint16_t pin_SCL,
		GPIO_TypeDef* port_SDA, uint16_t pin_SDA,
		GPIO_TypeDef* port_WP, 	uint16_t pin_WP,
		uint8_t addr_chip)
{
    pEEPROM->capacity = capacity;
    pEEPROM->port_SCL = port_SCL;
    pEEPROM->pin_SCL = pin_SCL;
    pEEPROM->port_SDA = port_SDA;
    pEEPROM->pin_SDA = pin_SDA;
    pEEPROM->port_WP = port_WP;
    pEEPROM->pin_WP = pin_WP;
    pEEPROM->addrCur = 0;
    pEEPROM->addr_chip = addr_chip;
//    pinMode(pEEPROM->pin_SCL, OUTPUT);
//    pinMode(pEEPROM->pin_SDA, OUTPUT_OPEN_DRAIN);
    WP_AT24C(pEEPROM);
    CLEAR_PIN(pEEPROM->port_SCL, pEEPROM->pin_SCL);
    delayMicroseconds(AT24C_HOLDTIME);
    SET_PIN(pEEPROM->port_SDA, pEEPROM->pin_SDA);
    delayMicroseconds(AT24C_HOLDTIME);
}

size_t I2C_EEPROM_WriteBytes(EEPROM_AT24_t* pEEPROM, uint8_t *Buffer, size_t Length)
{
	// write buffer bytewise
	size_t nMaxLen = byte_capacity[pEEPROM->capacity] - pEEPROM->addrCur;
	release_WP_AT24C(pEEPROM);
	if (Length > nMaxLen) Length = nMaxLen;
	for(size_t i = 0; i < Length; ++i)
	{
		write_AT24C(pEEPROM, pEEPROM->addrCur + i, Buffer[i]);
	}
	WP_AT24C(pEEPROM);
	return Length;
}

size_t I2C_EEPROM_ReadBytes(EEPROM_AT24_t* pEEPROM, uint8_t *Buffer, size_t Length)
{
	size_t nMaxLen = byte_capacity[pEEPROM->capacity] - pEEPROM->addrCur;
	if (Length > nMaxLen) Length = nMaxLen;
	for(size_t i = 0; i < Length; ++i)
	{
		Buffer[i] = read_AT24C(pEEPROM, pEEPROM->addrCur + i);
	}
	return Length;
}



