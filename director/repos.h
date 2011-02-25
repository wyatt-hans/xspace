/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef DIRECTOR_REPOS_H_
#define DIRECTOR_REPOS_H_

#include <sqlite3.h>

#include <boost/shared_ptr.hpp>
#include <list>
#include <string>

namespace xspace { namespace director {

using boost::shared_ptr;
using std::string;
using std::list;

enum MetaServerType {
    kMetaServerIPv4 = 0,
    kMetaServerIPv6,
};

struct MetaInfo {
    int64_t     id;
    string      name;
    int         type;
    string      addrv4;
    string      addrv6;
    int         port;
    int64_t     hosted;
};
typedef shared_ptr<MetaInfo> MetaInfoPtr;

struct SpaceInfo {
    int64_t     id;         // space id, globally
    string      name;       // space name
    int64_t     owner;      // space owner's id
    int         type;       // space type
    string      cred;       // cred used by metaservice and client
    int64_t     msaid0;     // metaserver address id
    int64_t     msaid1;
    int64_t     msaid2;
};
typedef shared_ptr<SpaceInfo> SpaceInfoPtr;

struct UserInfo {
    int64_t uid;
    string  name;
    string  passwd;
};
typedef shared_ptr<UserInfo>  UserInfoPtr;

class UserRepos {
    public:
    virtual ~UserRepos() {}
    
    virtual int64_t AuthenUser(const string& name, const string& passwd) = 0;

    virtual bool IsHaveUser(const string& user) = 0;   
    virtual bool AddUser(const string& name, const UserInfo &ui) = 0;
    virtual bool DelUser(const string& name) = 0;
    virtual bool GetUser(const string& name, UserInfo *ui) = 0;
    virtual bool SetUser(const string &name, const UserInfo &ui) = 0;
};

class SpaceRepos {
    public:
    virtual ~SpaceRepos() {}

    virtual bool CreateSpace(const string& name, int64_t uid,
            const std::string& space,
            const int type) = 0;
    virtual bool DeleteSpace(int64_t uid, const string& space) = 0;
    virtual bool RenameSpace(int64_t uid, const string& oldspace,
            const string& newspace) = 0;

    virtual bool IsHaveSpace(int64_t uid, const string& space) = 0;

    virtual bool GetSpaceInfo(int64_t uid, const string& space,
            SpaceInfo *si) = 0;
    virtual int GetSpaceList(int64_t uid, list<SpaceInfoPtr> *lsi) = 0;
    
    virtual string GetCred(const string& name, const string& space) = 0;
};

class MetaRepos {
    public:
    virtual ~MetaRepos() {}

    virtual bool AddMeta(const string& name, const string& addr,
            int port, int type) = 0;
    virtual bool RmvMeta(int64_t mid) = 0;

    virtual bool IsHaveMeta(const string& server) = 0;

    virtual bool GetMeta(int64_t mid, MetaInfo *msi) = 0;
    virtual bool GetMeta(const string& name, MetaInfo *msi) = 0;
    virtual bool GetMeta(list<shared_ptr<MetaInfo> > *lmsi) = 0;
    virtual bool AssignMeta(int64_t uid,
                const string& space, int64_t *msaid) = 0;

};

}}
#endif  // DIRECTOR_REPOS_H_

