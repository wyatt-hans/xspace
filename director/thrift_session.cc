/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "thrift_session.h"

#include <xspace.h>

#include <boost/shared_ptr.hpp>
#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>
#include <pthread.h>

#include <iostream>

namespace xspace { namespace director {
using ::apache::thrift::server::TServerEventHandler;
using ::apache::thrift::TMessageEventHandler;
using ::boost::shared_ptr;

void ClientEventHandler::preServe() {
    ::XSPACELOG(("Director : service starting ...\n"));
    
    if (service_ != NULL) {
        service_->Init();
        ::XSPACELOG(("Director : service started.\n"));
    } else {
        ::XSPACELOG(("Director : service not setting.\n"));
    }
}

void ClientEventHandler::clientBegin(int socket) {
    if (service_ != NULL) {
        if (!service_->SetupSession(socket)) {
            ::XSPACELOG(("Setup Session with socket:%d fail\n", socket));
            return ;
        }
        shared_ptr<SpaceSession> ss = service_->GetSession(socket);
        assert(ss != NULL);

        ss->Init();
        ss->SetState(kSessionStateConnecting);
    }
}

void ClientEventHandler::clientEnd(int socket) {
    if (service_ != NULL) {
        shared_ptr<SpaceSession> ss = service_-> GetSession(socket);
        if (ss != NULL) {
            ::XSPACELOG(("Close Session with socket:%d fail\n", socket));
            return ;
        }

        ss->Fini();
        service_->CloseSession(socket);
    }
}


void MessageEventHandler::create_key()
{
    pthread_key_create(&m_key, NULL);
}

void MessageEventHandler::setup()
{
    pthread_once(&m_once, create_key);
}

void MessageEventHandler::messageBegin(int &socket)
{
    pthread_setspecific(m_key, &socket);
}

void MessageEventHandler::messageEnd(int &socket)
{
    pthread_setspecific(m_key, NULL);
}

int MessageEventHandler::GetSocket()
{
    int *socket = (int *)pthread_getspecific(m_key);
    if(socket != NULL)
        return *socket;

    return -1;
}

pthread_key_t MessageEventHandler::m_key = 0;
pthread_once_t MessageEventHandler::m_once = PTHREAD_ONCE_INIT;

} }

