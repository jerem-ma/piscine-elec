NAME=program
F_CPU=16000000

CC=avr-gcc
CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=atmega328p

SRCS=main.c
_OBJS=$(SRCS:.c=.o)
OBJS=$(addprefix build/, $(_OBJS))

DEVICE=/dev/ttyUSB0

all: hex flash

hex:	$(NAME).hex

build/%.o	: srcs/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) $(CFLAGS) -c $< -o $@

build/$(NAME).bin	:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(NAME).hex	:	build/$(NAME).bin
	avr-objcopy -O ihex $< $@

flash:	$(NAME).hex
	avrdude -c arduino -p atmega328p -P $(DEVICE) -U flash:w:$<

clean:
	rm -rf build/
	rm -f $(NAME).hex


.PHONY: all clean hex flash
