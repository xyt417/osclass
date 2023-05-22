DIR = Source
SOURCE = $(DIR)/test_device.cpp
Test:
	g++  -I Source $(SOURCE) -o test.exe
	./test.exe
