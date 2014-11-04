all: rshell

FLG=-Wall -Werror -ansi -pedantic
CMP=g++

rshell: makebin src/main.cpp ls
	$(CMP) $(FLG) src/main.cpp -o bin/rshell

ls: src/ls.cpp
	$(CMP) $(FLG) src/ls.cpp -o bin/ls

makebin:
	@if [ ! -d bin/ ];\
	then \
	mkdir bin; \
	fi

clean:
	rm -rf bin/

#bin/*.o bin/rshell bin/ls


