/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_DIRECTOR_MGMT_SERVICE_H_
#define XSPACE_DIRECTOR_MGMT_SERVICE_H_

#include "service.h"
#include "sqlite_repos.h"
#include "mgmt_session.h"

#include "gen-cpp/MgmtService.h"

#include <thrift/server/TServer.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

namespace xspace { namespace director {

using apache::thrift::server::TServerEventHandler;
using boost::shared_ptr;
using std::string;
using std::map;

class MgmtService : virtual public MgmtServiceIf, virtual public Service {
    typedef shared_ptr<MgmtSession> MgmtSessionPtr;
    typedef shared_ptr<SqliteRepos> SqliteReposPtr;
    public:
    MgmtService():inited_(false) { }

    // interface from MgmtServiceIf
    bool Setup(const std::string& name, const std::string& passwd);
    void Close();

    bool CreateUser(const std::string& name, const std::string& passwd);
    bool DeleteUser(const std::string& name);

    void GetUserInfo(MgmtUserInfo& _return, const std::string& name);
    bool SetUserInfo(const std::string& name, const MgmtUserInfo& ui);

    bool CreateSpace(const int64_t uid, const std::string& space,
            const SpaceType::type type);
    bool DeleteSpace(const int64_t uid, const std::string& space);

    void GetSpaceList(SpaceListRsp& _return, const int64_t uid);
    void GetSpaceInfo(SpaceInfoRsp& _return, const int64_t uid,
          const std::string& space);

    bool AddMetaServer(const std::string& addr, const int32_t port);
    bool RmvMetaServer(const int64_t msaid);

    void GetAllMetaServer(std::vector<int64_t> & _return);
    
    // for initalization management service
    bool Init();
    void Fini();
    void SetRepos(shared_ptr<SqliteRepos> repos) { repos_ = repos; }
    shared_ptr<SqliteRepos> GetRepos() { return repos_; }
    
    shared_ptr<MgmtSession> GetSession();
    shared_ptr<Session> GetSession(int socket);
    bool SetupSession(int socket);
    void CloseSession(int socket);

    private:
    bool                        inited_;
    shared_ptr<SqliteRepos>     repos_;
    map<int, MgmtSessionPtr>    sessions_;
};

}}

#endif  // XSPACE_DIRECTOR_MGMT_SERVICE_H_

