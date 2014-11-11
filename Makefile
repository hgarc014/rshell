all: rshell ls

FLG=-Wall -Werror -ansi -pedantic
CMP=g++
FLD=bin

rshell: makebin src/main.cpp
	$(CMP) $(FLG) src/main.cpp -o bin/rshell

ls: makebin src/ls.cpp
	$(CMP) $(FLG) src/ls.cpp -o bin/ls

makebin:
	@if [ ! -d $(FLD) ];\
	then \
	mkdir $(FLD); \
	fi

clean:
	rm -rf $(FLD)

#bin/*.o bin/rshell bin/ls


