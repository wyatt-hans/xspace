
space_objs = space_service.oo \
			 space_session.oo

mgmt_objs =
meta_objs =

common = thrift_session.oo	\
		 sqlite_repos.oo	\
		 server.oo		\
		 ../misc/crypto_md5.oo

objs = $(space_objs) $(mgmt_objs) $(meta_objs) $(common)

thriftsrc = director.thrift
thriftcpp = gen-cpp/director_constants.cpp	\
			gen-cpp/director_types.cpp		\
			gen-cpp/MetaService.cpp		\
			gen-cpp/MgmtService.cpp		\
			gen-cpp/SpaceService.cpp

thriftobj = $(patsubst %.cpp, %.o, $(thriftcpp))

target = portalsd

CXX = g++
INCLUDE = -I/usr/local/include/thrift -Igen-cpp -I../include
CXXFLAGS = -O2 $(INCLUDE) -Wall
LDFLAGS = -lthriftnb -lthrift -levent -lsqlite3 -lcryptopp

objects = $(thriftobj) $(objs)

all : $(objects)
	$(CXX) -o $(target) $(CXXFLAGS) $(LDFLAGS) $(objects)

$(thriftcpp) : $(thriftsrc)
	thrift --gen cpp $(thriftsrc)


.PHONY: gen

%.oo : %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

gen : $(thriftsrc)
	thrift --gen cpp $(thriftsrc)

clean:
	$(RM) -f $(objects)
	$(RM) -f $(target)
