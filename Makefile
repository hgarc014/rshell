all: rshell ls cp

FLG=-Wall -Werror -ansi -pedantic
CMP=g++
FLD=bin

rshell: makebin src/rshell.cpp
	$(CMP) $(FLG) src/rshell.cpp -o bin/rshell

ls: makebin src/ls.cpp
	$(CMP) $(FLG) src/ls.cpp -o bin/ls

cp: makebin src/cp.cpp
	$(CMP) $(FLG) src/cp.cpp -o bin/cp

makebin:
	@if [ ! -d $(FLD) ];\
	then \
	mkdir $(FLD); \
	fi

clean:
	rm -rf $(FLD)

