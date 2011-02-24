/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "space_service.h"
#include "thrift_session.h"
#include "gen-cpp/SpaceService.h"

#include <xtracor.h>

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>
#include <server/TNonblockingServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace xspace::director;
using xspace::xtracor_init;

int main(int argc, char **argv) 
{
    int port = 9090;
    int workerCount =2;

    xtracor_init("director");

     // initialize thread pool
    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
    shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    // initialize space service
    shared_ptr<SpaceService> spaceHandler(new SpaceService());
    shared_ptr<TProcessor>   spaceProcessor(new SpaceServiceProcessor(spaceHandler));
    shared_ptr<TProtocolFactory> spaceProtocolFactory(new TBinaryProtocolFactory());
    
    shared_ptr<MessageEventHandler>       msgEventHandler(new MessageEventHandler());
    msgEventHandler->setup();
    spaceProcessor->SetEventHandler(msgEventHandler);

    TNonblockingServer server(spaceProcessor, spaceProtocolFactory, port, threadManager); 
    shared_ptr<ClientEventHandler>  ceh(new ClientEventHandler());
    ceh->SetService(spaceHandler);
    server.setServerEventHandler(ceh);

    server.serve();
    
    return 0;
}


