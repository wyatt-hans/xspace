/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_DIRECTOR_SPACE_SERVICE_H_
#define XSPACE_DIRECTOR_SPACE_SERVICE_H_

#include "sqlite_repos.h"
#include "space_session.h"

#include "gen-cpp/SpaceService.h"

#include <thrift/server/TServer.h>

#include <boost/shared_ptr.hpp>
#include <string>

namespace xspace { namespace director {

using apache::thrift::server::TServerEventHandler;
using boost::shared_ptr;
using std::string;

class SpaceService : virtual public SpaceServiceIf {
    public:

    typedef shared_ptr<SpaceSession> SpaceSessionPtr;
    typedef shared_ptr<SqliteRepos>  SqliteReposPtr;

    SpaceService() : inited_(false) { }

    // space service interface
    bool Setup(const std::string& name, const std::string& passwd);
    void Close();

    void GetSpaceInfo(SpaceInfoRsp& _return, const std::string& space);
    void GetSpaceList(SpaceListRsp& _return);

    bool CreateSpace(const std::string& space,
            const SpaceType::type type);
    bool DeleteSpace(const std::string& space);
    bool RenameSpace(const std::string& oldspace,
            const std::string& newspace);
     
    // space service self function
    bool Init();
    void Fini();

    SqliteReposPtr GetRepos() { return repos_; }
    shared_ptr<SpaceSession> GetSession();
    shared_ptr<SpaceSession> GetSession(int socket);
     
    bool SetupSession(int socket);
    void CloseSession(int socket);

 private:
    bool                        inited_;
    shared_ptr<SqliteRepos>     repos_;
    map<int, SpaceSessionPtr>   sessions_;
};

}}
#endif  // XSPACE_DIRECTOR_SPACE_SERVICE_H_
