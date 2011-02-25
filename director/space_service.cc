/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "space_service.h"
#include "space_session.h"
#include "thrift_session.h"

#include "gen-cpp/SpaceService.h"

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

bool SpaceService::Setup(const std::string& name,
        const std::string& passwd) {

    if (!inited_)
        return false;
    
    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL)
        return false;

    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    int64_t uid = sr->GetUserRepos()->AuthenUser(name, passwd);
    if (uid != -1) {
        
        UserInfo ui;
        ui.uid = uid;
        ui.name = name;
        ui.passwd = passwd;
        sr->GetUserRepos()->SetUser(name, ui);

        ss->SetState(kSessionStateSetuped);
        return true;
    } else {
        ss->SetState(kSessionStateConnecting);
        return false;
    }
}

void SpaceService::Close() {
    if (!inited_)
        return;

    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL)
        return ;

    ss->SetState(kSessionStateDisconnecting);
    ss->Fini();
    ss->SetState(kSessionStateIdle);
}

void SpaceService::GetSpaceInfo(SpaceInfoRsp& _return,
        const std::string& space) {
    if (!inited_) {
        _return.ret = -1;
        return ;
    }

    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL) {
        _return.ret = -1;
        return ;
    }

    if(ss->GetState() != kSessionStateSetuped) {
        _return.ret = -1;
        return ;
    }
    
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);
    
    SpaceInfo si;
    if (!sr->GetSpaceRepos()->GetSpaceInfo(ss->GetUserID(), space, &si)) {
        _return.ret = -1;
        return ;
    }
    _return.ret = 0;
    _return.id = si.id;
    _return.cred = si.cred;
#if 0
    _return.address.push_back(si.msaid0);
    _return.metaAddr.push_back(si.msaid1);
    _return.metaAddr.push_back(si.msaid2);
#endif
}

void SpaceService::GetSpaceList(SpaceListRsp& _return) {
    if (!inited_) {
        _return.ret = -1;
        return ;
    }
    
    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL) {
        _return.ret = -1;
        return ;
    }

    if(ss->GetState() != kSessionStateSetuped) {
        _return.ret = -1;
        return ;
    }
 
    list<SpaceInfoPtr> lsi;
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    sr->GetSpaceRepos()->GetSpaceList(ss->GetUserID(), &lsi);
    list<SpaceInfoPtr>::iterator it;
    for (it = lsi.begin(); it != lsi.end(); it++) {
        _return.spaces.push_back((*it)->name);
    }

    _return.ret = 0;
}

bool SpaceService::CreateSpace(const std::string& space,
        const SpaceType::type type) {
    xtracor(LOG_INFO, "space service : createspace\n");
    if (!inited_) {
        xtracor(LOG_DEBUG, "space service not initialization!\n");
        return false;
    }

    shared_ptr<SpaceSession> ss = GetSession();
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
            ss->GetUserID(), space, type);
}

bool SpaceService::DeleteSpace(const std::string& space) {
    if (!inited_) {
        xtracor(LOG_DEBUG, "space service not initialization!\n");
        return false;
    }
    
    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped)
        return false;
 
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    return sr->GetSpaceRepos()->DeleteSpace(ss->GetUserID(), space);
}

bool SpaceService::RenameSpace(const std::string& oldspace,
        const std::string& newspace) {
    if (!inited_)
        return false;
    
    shared_ptr<SpaceSession> ss = GetSession();
    if (ss == NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped)
        return false;
 
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    return sr->GetSpaceRepos()->RenameSpace(ss->GetUserID(),
            oldspace, newspace);
}

bool SpaceService::Init() {
    repos_ = shared_ptr<SqliteRepos>(new SqliteRepos());
    
    if (!repos_->Init("director.db")) {
        xtracor(LOG_ERR, "Load space database fail!\n");
        repos_ = shared_ptr<SqliteRepos>();
        return false;
    }
    xtracor(LOG_INFO, "SpaceService : load db ok\n");

    inited_ = true;
    return true;
}

void SpaceService::Fini() {
    repos_->Fini();
}

shared_ptr<SpaceSession> SpaceService::GetSession() {
    int socket = MessageEventHandler::GetSocket();
    xtracor(LOG_INFO, "current session from:%d\n", socket);
    return dynamic_pointer_cast<SpaceSession>(this->GetSession(socket));
}

shared_ptr<Session> SpaceService::GetSession(int socket) {
    map<int, SpaceSessionPtr>::iterator it;

    it = sessions_.find(socket);
    if (it != sessions_.end()) {
        return it->second;
    }

    xtracor(LOG_INFO, "no session with socket:%d\n", socket);
    return shared_ptr<SpaceSession>();
}
     
bool SpaceService::SetupSession(int socket) {
    shared_ptr<Session> ssp = this->GetSession(socket);
    if (ssp != NULL) {
        xtracor(LOG_ERR, "Dumplicated socket:%d\n", socket);
        return false;
    }

    sessions_[socket] = shared_ptr<SpaceSession> (new SpaceSession(socket));
    xtracor(LOG_INFO, "setup session : %d\n", socket);
    return true;
}

void SpaceService::CloseSession(int socket) {
    map<int, SpaceSessionPtr>::iterator it;

    it = sessions_.find(socket);
    if (it != sessions_.end()) {
        sessions_.erase(it);
    }
}

}}

