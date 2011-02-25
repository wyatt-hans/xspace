/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "thrift_session.h"

#include <xtracor.h>

#include <boost/shared_ptr.hpp>
#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>
#include <pthread.h>

#include <iostream>

namespace xspace { namespace director {
using ::apache::thrift::server::TServerEventHandler;
using ::apache::thrift::TMessageEventHandler;
using ::boost::shared_ptr;
using xspace::xtracor;

void ClientEventHandler::preServe() {
    xtracor(LOG_INFO, "Director : service starting ...\n");

    if (service_ != NULL) {
        service_->Init();
        xtracor(LOG_INFO, "Director : service started.\n");
    } else {
        xtracor(LOG_INFO, "Director : service not setting.\n");
    }
}

void ClientEventHandler::clientBegin(int socket) {
    xtracor(LOG_INFO, "Login from socket:%d\n", socket);

    if (service_ != NULL) {
        if (!service_->SetupSession(socket)) {
            xtracor(LOG_ERR, "Setup Session with socket:%d fail\n", socket);
            return ;
        }
        shared_ptr<Session> ss = service_->GetSession(socket);
        assert(ss != NULL);

        ss->Init();
        ss->SetState(kSessionStateConnecting);
    }
}

void ClientEventHandler::clientEnd(int socket) {
    xtracor(LOG_INFO, "Logout from socket:%d\n", socket);
    if (service_ != NULL) {
        shared_ptr<Session> ss = service_-> GetSession(socket);
        if (ss == NULL) {
            xtracor(LOG_ERR, "Close Session with socket:%d fail\n", socket);
            return ;
        }

        ss->Fini();
        service_->CloseSession(socket);
    }
}


void MessageEventHandler::create_key()
{
    pthread_key_create(&key_, NULL);
}

void MessageEventHandler::setup()
{
    pthread_once(&once_, create_key);
}

void MessageEventHandler::messageBegin(int &socket)
{
    pthread_setspecific(key_, (void *)socket);
}

void MessageEventHandler::messageEnd(int &socket)
{
    pthread_setspecific(key_, NULL);
}

int MessageEventHandler::GetSocket()
{
    int socket = (int64_t)pthread_getspecific(key_);
    if(socket != 0)
        return socket;

    return -1;
}

pthread_key_t MessageEventHandler::key_ = 0;
pthread_once_t MessageEventHandler::once_ = PTHREAD_ONCE_INIT;

} }

