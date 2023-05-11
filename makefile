DIR = Source
SOURCE = $(DIR)/test.cpp
Test: $(DIR)/test.cpp $(DIR)/device.h
	g++  -I Source $(SOURCE) -o test.exe
	./test.exe
