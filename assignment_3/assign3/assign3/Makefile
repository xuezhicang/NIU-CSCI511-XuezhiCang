CC = g++
CCFLAGS = -Wall -g



all: load_file_to_DB query_place
	
load_file_to_DB: load_file_to_DB.o
	$(CC) $(CCFLAGS) -o load_file_to_DB load_file_to_DB.o -lpq
	
	
load_file_to_DB.o:load_file_to_DB.cc
	$(CC) $(CCFLAGS) -c load_file_to_DB.cc
	
query_place: query_place.o
	$(CC) $(CCFLAGS) -o query_place query_place.o -lpq
	
	
query_place.o: query_place.cc
	$(CC) $(CCFLAGS) -c query_place.cc
		
clean: 
	-rm *.o query_place load_file_to_DB


