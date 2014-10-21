all: rshell

FLG=-Wall -Werror -ansi -pedantic
CMP=g++


rshell: src/main.cpp
	@if [ ! -d bin/ ];\
	then \
	mkdir bin; \
	fi
	$(CMP) $(FLG) src/main.cpp -o bin/rshell


clean:
	rm bin/*.o bin/rshell.out


