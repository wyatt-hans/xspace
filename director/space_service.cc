/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "space_service.h"
#include "space_session.h"
#include "thrift_session.h"

#include "gen-cpp/SpaceService.h"

#include <xspace.h>
#include <assert.h>

#include <thrift/TProcessor.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/singleton.hpp>

namespace xspace { namespace director {

using boost::shared_ptr;
using std::string;

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
    if (ss != NULL) {
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
    if (ss != NULL) {
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
    if (!inited_)
        return false;
    
    shared_ptr<SpaceSession> ss = GetSession();
    if (ss != NULL)
        return false;

    if(ss->GetState() != kSessionStateSetuped)
        return false;
 
    shared_ptr<SqliteRepos> sr = GetRepos();
    assert(sr != NULL);

    return sr->GetSpaceRepos()->CreateSpace(ss->GetUser(),
            ss->GetUserID(), space, type);
}

bool SpaceService::DeleteSpace(const std::string& space) {
    if (!inited_)
        return false;
    
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
        ::XSPACELOG(("Load space database fail!\n"));
        repos_ = shared_ptr<SqliteRepos>();
        return false;
    }
    ::XSPACELOG(("SpaceService : load db ok\n"));

    return true;
}

void SpaceService::Fini() {
    repos_->Fini();
}

shared_ptr<SpaceSession> SpaceService::GetSession() {
    int socket = MessageEventHandler::GetSocket();
    return this->GetSession(socket);
}

shared_ptr<SpaceSession> SpaceService::GetSession(int socket) {
    map<int, SpaceSessionPtr>::iterator it;

    it = sessions_.find(socket);
    if (it != sessions_.end()) {
        return it->second;
    }

    return shared_ptr<SpaceSession>();
}
     
bool SpaceService::SetupSession(int socket) {
    shared_ptr<SpaceSession> ssp = this->GetSession(socket);
    if (ssp == NULL) {
        ::XSPACELOG(("Dumplicated socket\n"));
        return false;
    }

    ssp = shared_ptr<SpaceSession> (new SpaceSession(socket));
    sessions_[socket] = ssp;
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

