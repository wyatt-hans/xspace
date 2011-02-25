/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef DIRECTOR_THRIFT_SESSION_H_
#define DIRECTOR_THRIFT_SESSION_H_

#include "service.h"

#include <boost/shared_ptr.hpp>

#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>
#include <pthread.h>

namespace xspace { namespace director {

using apache::thrift::server::TServerEventHandler;
using apache::thrift::TMessageEventHandler;
using boost::shared_ptr;

class ClientEventHandler: public TServerEventHandler {
    public:
    void preServe();
    void clientBegin(int socket);
    void clientEnd(int socket);

    void SetService(shared_ptr<Service> ssp) {
        service_ = ssp; }

    private:
    shared_ptr<Service>  service_;
};

class MessageEventHandler : public TMessageEventHandler {
    public:
    void setup();
    void messageBegin(int &socket);
    void messageEnd(int &socket);

    static int GetSocket();
    private:
    static void create_key();
    static pthread_key_t key_;
    static pthread_once_t once_;
};
}}
#endif  // DIRECTOR_THRIFT_SESSION_H_

