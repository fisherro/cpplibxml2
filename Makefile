CXXFLAGS+=-std=c++17
CXXFLAGS+=-fsanitize=address
CXXFLAGS+=-I/opt/local/include/libxml2

LDFLAGS+=-lxml2

all: tree1

