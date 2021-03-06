/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef DIRECTOR_SPACE_SESSION_H_
#define DIRECTOR_SPACE_SESSION_H_

#include "session.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace xspace { namespace director {

using boost::shared_ptr;
using std::string;

class SpaceSession : public Session {
    public:
    explicit SpaceSession(int socket) : Session(socket), uid_(-1) { }
    ~SpaceSession() {}

    void SetUser(const string& name) { uname_ = name; }
    string& GetUser() { return uname_; }
    
    void SetUserID(int64_t uid) { uid_ = uid; }
    int64_t GetUserID() { return uid_; }

    bool Init() { return true; }
    void Fini() { }
    private:
    int64_t  uid_;
    string   uname_;
};

}}

#endif  // DIRECTOR_SPACE_SESSION_H_

