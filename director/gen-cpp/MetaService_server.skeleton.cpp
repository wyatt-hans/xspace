// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "MetaService.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace xspace::director;

class MetaServiceHandler : virtual public MetaServiceIf {
 public:
  MetaServiceHandler() {
    // Your initialization goes here
  }

  bool Setup(const int64_t id, const std::string& cred) {
    // Your implementation goes here
    printf("Setup\n");
  }

  void Close() {
    // Your implementation goes here
    printf("Close\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<MetaServiceHandler> handler(new MetaServiceHandler());
  shared_ptr<TProcessor> processor(new MetaServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
