all: rshell

FLG=-Wall -Werror -ansi -pedantic
CMP=g++


rshell: src/main.cpp cp
	@if [ ! -d bin/ ];\
	then \
	mkdir bin; \
	fi
	$(CMP) $(FLG) src/main.cpp -o bin/rshell

cp: src/cp.cpp
	@if [ ! -d bin/ ];\
	then \
	mkdir bin;\
	fi
	$(CMP) $(FLG) src/cp.cpp -o bin/cp

clean:
	rm bin/*.o bin/rshell.out


