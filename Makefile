TARGET ?= pulse_rgb
PORT   ?= /dev/ttyACM0
BOARD  ?= arduino:avr:uno

check: $(TARGET)/$(TARGET).ino
	arduino --verbose --verify $?

install: $(TARGET)/$(TARGET).ino
	arduino --upload $? --port $(PORT) --board $(BOARD)
