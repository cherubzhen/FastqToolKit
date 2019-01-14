CC=g++
FLAG=-w -std=c++0x
LIB=-lstdc++ -lgzstream -lz
all: RandomSelectFastq
	
RandomSelectFastq: randomSelectFastq.cpp
	${CC} ${FLAG} randomSelectFastq.cpp -o randomSelectFastq ${LIB}

