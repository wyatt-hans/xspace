/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "thrift_session.h"
#include "mgmt_service.h"
#include "mgmt_session.h"

#include "gen-cpp/MgmtService.h"

#include <xtracor.h>
#include <assert.h>

#include <thrift/TProcessor.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/singleton.hpp>

namespace xspace { namespace director {

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using std::string;
using xspace::xtracor;
using std::map;

// interface from MgmtServiceIf

#define CHECK_INITED_R(inited_var_,return_var_)       \
    do {                                            \
        if (!(inited_var_)) {                       \
            xtracor(LOG_ERR, "management service not initialized!\n");  \
            return return_var_;                     \
        }                                           \
    } while(0)

#define CHECK_INITED(inited_var_)                   \
    do {                                            \
        if (!(inited_var_)) {                       \
            xtracor(LOG_ERR, "management service not initialized!\n");  \
            return ;                                \
        }                                           \
    } while(0)


bool MgmtService::Setup(const std::string& name,
        const std::string& passwd) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return false;

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    int64_t uid = sr->GetUserRepos()->AuthenUser(name, passwd);
    if (uid != -1) {
        ss->SetState(kSessionStateSetuped);
        return true;
    } else {
        ss->SetState(kSessionStateConnecting);
        return false;
    }
}

void MgmtService::Close() {
    CHECK_INITED(inited_);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return ;

    ss->SetState(kSessionStateDisconnecting);
    ss->Fini();
    ss->SetState(kSessionStateIdle);
}

bool MgmtService::CreateUser(const std::string& name,
        const std::string& passwd) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped) {
        xtracor(LOG_WARNING, "user not setuped");
        return false;
    }
    
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    UserInfo ui;
    ui.name = name;
    ui.passwd = passwd;
    if (!sr->GetUserRepos()->AddUser(name, ui)) {
        xtracor(LOG_INFO, "create user:%s fail", name.c_str());
        return false;
    }
    return true;
}

bool MgmtService::DeleteUser(const std::string& name) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped) {
        xtracor(LOG_WARNING, "user not setuped");
        return false;
    }
    
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    if (!sr->GetUserRepos()->DelUser(name)) {
        xtracor(LOG_INFO, "delete user:%s fail", name.c_str());
        return false;
    }
    return true;
}

void MgmtService::GetUserInfo(MgmtUserInfo& _return,
        const std::string& name) {
    _return.ret = -1;
}

bool MgmtService::SetUserInfo(const std::string& name,
        const MgmtUserInfo& ui) {
    return false;
}

bool MgmtService::CreateSpace(const int64_t uid, const std::string& space,
            const SpaceType::type type) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL) {
        xtracor(LOG_DEBUG, "session not found!\n");
        return false;
    }

    if(ss->GetState() != kSessionStateSetuped) {
        xtracor(LOG_DEBUG, "session not setuped!\n");
        return false;
    }

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    return sr->GetSpaceRepos()->CreateSpace(ss->GetUser(),
            uid, space, type);
}

bool MgmtService::DeleteSpace(const int64_t uid, const std::string& space) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped)
        return false;
 
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    return sr->GetSpaceRepos()->DeleteSpace(uid, space);
}

void MgmtService::GetSpaceList(SpaceListRsp& _return, const int64_t uid) {
    _return.ret = -1;
    CHECK_INITED(inited_);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return ;

    if(ss->GetState() != kSessionStateSetuped)
        return ;
 
    list<SpaceInfoPtr> lsi;
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    sr->GetSpaceRepos()->GetSpaceList(uid, &lsi);
    list<SpaceInfoPtr>::iterator it;
    for (it = lsi.begin(); it != lsi.end(); it++) {
        _return.spaces.push_back((*it)->name);
    }

    _return.ret = 0;
}

void MgmtService::GetSpaceInfo(SpaceInfoRsp& _return, const int64_t uid,
          const std::string& space) {
    _return.ret = -1;
    CHECK_INITED(inited_);

    shared_ptr<MgmtSession> ss = GetSession();
    if (ss == NULL)
        return ;

    if(ss->GetState() != kSessionStateSetuped)
        return ;

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    SpaceInfo si;
    if (!sr->GetSpaceRepos()->GetSpaceInfo(uid, space, &si))
        return ;
    
    _return.ret = 0;
    _return.id = si.id;
    _return.cred = si.cred;
#if 0
    _return.address.push_back(si.msaid0);
    _return.metaAddr.push_back(si.msaid1);
    _return.metaAddr.push_back(si.msaid2);
#endif
}

bool MgmtService::AddMetaServer(const std::string& addr, const int32_t port) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    return sr->GetMetaRepos()->AddMeta(addr, addr, port, kMetaServerIPv4);
}

bool MgmtService::RmvMetaServer(const int64_t msaid) {
    CHECK_INITED_R(inited_, false);

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    return sr->GetMetaRepos()->RmvMeta(msaid);
}

void MgmtService::GetAllMetaServer(std::vector<int64_t> & _return) {
    CHECK_INITED(inited_);
    list<shared_ptr<MetaInfo> > msis;

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    if (!sr->GetMetaRepos()->GetMeta(&msis))
        return ;

    list<shared_ptr<MetaInfo> >::iterator it = msis.begin();
    for (; it != msis.end(); it++) {
        _return.push_back((*it)->id);
    }
}

// for initalization management service

bool MgmtService::Init() {
    inited_ = true;
    return true;
}

void MgmtService::Fini() {
}

shared_ptr<MgmtSession> MgmtService::GetSession() {
    int socket = MessageEventHandler::GetSocket();
    xtracor(LOG_INFO, "current session from:%d\n", socket);
    return dynamic_pointer_cast<MgmtSession>(this->GetSession(socket));
}

shared_ptr<Session> MgmtService::GetSession(int socket) {
    map<int, MgmtSessionPtr>::iterator it;

    it = sessions_.find(socket);
    if (it != sessions_.end()) {
        return it->second;
    }

    xtracor(LOG_INFO, "no session with socket:%d\n", socket);
    return shared_ptr<MgmtSession>();
}
     
bool MgmtService::SetupSession(int socket) {
    shared_ptr<Session> ssp = this->GetSession(socket);
    if (ssp != NULL) {
        xtracor(LOG_ERR, "Dumplicated socket:%d\n", socket);
        return false;
    }

    sessions_[socket] = shared_ptr<MgmtSession> (new MgmtSession(socket));
    xtracor(LOG_INFO, "setup session : %d\n", socket);
    return true;
}

void MgmtService::CloseSession(int socket) {
    map<int, MgmtSessionPtr>::iterator it;

    it = sessions_.find(socket);
    if (it != sessions_.end()) {
        sessions_.erase(it);
    }
}

}}
