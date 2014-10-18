all: rshell

FLG=-Wall -Werror -ansi -pedantic
CMP=g++


rshell: src/main.cpp
	$(CMP) $(FLG) src/main.cpp -o src/rshell.out


clean:
	rm *.o rshell.out
#example:
#a.out : BST.h main.cc
#	g++ $(FLAGS) main.cc
#
