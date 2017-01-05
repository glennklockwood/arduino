TARGET = pulse_rgb/pulse_rgb.ino
PORT   = /dev/ttyACM0
BOARD  = arduino:avr:uno

check: $(TARGET)
	arduino --verbose --verify $?

install: $(TARGET)
	arduino --upload $? --port $(PORT) --board $(BOARD)
