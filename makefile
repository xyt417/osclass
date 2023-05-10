DIR = Source
SOURCE = $(DIR)/test.cpp
Test: $(DIR)/test.cpp $(DIR)/device_queue.h
	g++ $(SOURCE) -o test.exe
	./test.exe
