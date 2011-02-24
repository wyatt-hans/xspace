
objs = client.o

thriftsrc = portal.thrift
thriftobj = ../gen-cpp/director_constants.o	\
			../gen-cpp/SpaceService.o		\
			../gen-cpp/director_types.o

target = client

CXX = g++
INCLUDE = -I/usr/local/include/thrift -I../gen-cpp
CXXFLAGS = -O2 $(INCLUDE)
LDFLAGS = -lthrift

objects = $(thriftobj) $(objs)

all : $(objects)
	$(CXX) -o $(target) $(CXXFLAGS) $(LDFLAGS) $(objects)

.PHONY: thrift-gen

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

thrift-gen : $(thriftsrc)
	thrift --gen cpp $(thriftsrc)

clean:
	$(RM) -rf gen-cpp
	$(RM) -f $(objects)
	$(RM) -f $(target)
