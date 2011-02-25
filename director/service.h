/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_DIRECTOR_SERVICE_H_
#define XSPACE_DIRECTOR_SERVICE_H_

#include "session.h"

#include <boost/shared_ptr.hpp>

namespace xspace { namespace director {

using boost::shared_ptr;

class Service {
    public:
    virtual ~Service() {}

    virtual bool Init() = 0;
    virtual void Fini() = 0;
    virtual shared_ptr<Session> GetSession(int socket) = 0;
    virtual bool SetupSession(int socket) = 0;
    virtual void CloseSession(int socket) = 0;
};

}}

#endif  // XSPACE_DIRECTOR_SERVICE_H_

