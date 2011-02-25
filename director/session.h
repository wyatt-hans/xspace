/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef DIRECTOR_SESSION_H_
#define DIRECTOR_SESSION_H_

namespace xspace { namespace director {

enum SessionState {
    kSessionStateIdle = 0,
    kSessionStateConnecting,
    kSessionStateSetuped,
    kSessionStateDisconnecting,
};

class Session {
    public:
    explicit Session(int socket) : socket_(socket),
            state_(kSessionStateIdle) {}

    void SetState(SessionState ss) { state_ = ss; }
    SessionState GetState() { return state_; }

    virtual bool Init() = 0;
    virtual void Fini() = 0;
    
    private:
    int socket_;
    SessionState state_;
};

}}

#endif  // DIRECTOR_SESSION_H_

