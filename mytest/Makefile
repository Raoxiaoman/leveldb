source_name=$(sc)
all:
	${CXX} ${CFLAG} ${SRC} ${LIB} -o ${TARGET}

CXX    = g++
LIB    = -L ../out-shared -lleveldb
CFLAG  += -g -std=c++11
TARGET = test
SRC    = $(source_name)
OJBS   = ${SRC:.cpp=.o}

clean:
	rm -rf ${TARGET} ${OJBS}
