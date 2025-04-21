CC = g++
CFLAGS = -Wall -std=c++11

SOURCES = main.cpp system_reader.cpp real_fme.cpp integer_fme.cpp
HEADERS = system_types.h system_reader.h real_fme.h integer_fme.h
EXECUTABLE = fme

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
