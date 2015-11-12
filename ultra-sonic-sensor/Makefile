DEVICE = atmega168
AVRDUDE_DEVICE = m168
DEVICE ?= atmega168
AVRDUDE_DEVICE ?= m168

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(DEVICE) -O2 -I../libpololu-avr
CPP=avr-g++
CC=avr-gcc
OBJ2HEX=avr-objcopy 
LDFLAGS=-Wl,-gc-sections -L../libpololu-avr -lpololu_$(DEVICE) -Wl,-relax

PORT ?= /dev/cuaU0
AVRDUDE=avrdude
TARGET=ultra_sonic_sensor

all: $(TARGET).hex

%.hex: %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: %.o
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex
