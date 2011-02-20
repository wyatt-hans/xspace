
objs-t = space_repos_test.o
objs-s = ../space_repos.o ../crypto_md5.o

thriftobj = ../gen-cpp/portals_constants.o	\
			../gen-cpp/SpaceService.o		\
			../gen-cpp/portals_types.o

gtestobj = ../../thirdparty/gtest/src/gtest.o gtest.o

target = test

CXX = g++
INCLUDE = -I/usr/local/include/thrift -I../gen-cpp -I../../thirdparty/gtest/include
CXXFLAGS = -O2 $(INCLUDE)
LDFLAGS = -lthrift -lsqlite3 -lcryptopp

objects = $(thriftobj) $(objs-s) $(objs-t) $(gtestobj)

all : $(objects)
	$(CXX) -o $(target) $(CXXFLAGS) $(LDFLAGS) $(objects)

.PHONY: thrift-gen

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

thrift-gen : $(thriftsrc)
	thrift --gen cpp $(thriftsrc)

clean:
	$(RM) -f $(thriftobj)
	$(RM) -f $(objs-s) $(objs-t)
	$(RM) -f $(target)
