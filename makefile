all: Parser

Parser: VKParser.o additional.o Task.o
	g++ -o "Parser" VKParser.o additional.o Task.o -lpthread -lcurl 

	
Parser.o: VKParser.cpp
	g++ -c VKParser.cpp -std=c++11

Task.o: Task.cpp
	g++ -c Task.cpp -std=c++11

additional.o: additional.cpp
	g++ -c additional.cpp -std=c++11
	
clean:
	rm -rf *.o Parser
 
