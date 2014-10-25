all: rshell

FLG=-Wall -Werror -ansi -pedantic
CMP=g++


rshell: src/main.cpp
	@if [ ! -d bin/ ];\
	then \
	mkdir bin; \
	fi
	$(CMP) $(FLG) src/main.cpp -o bin/rshell
	$(COMP) $(FLG) src/ls.cpp -o bin/ls


clean:
	rm bin/*.o bin/rshell.out


