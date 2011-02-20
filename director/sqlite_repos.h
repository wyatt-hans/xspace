/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef DIRECTOR_SQLITE_REPOS_H_
#define DIRECTOR_SQLITE_REPOS_H_

#include "repos.h"

#include <sqlite3.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>

namespace xspace { namespace director {

using boost::shared_ptr;
using std::string;
using std::list;

class SqliteRepos;

class SqliteUserRepos : public UserRepos {
    public:
    explicit SqliteUserRepos(sqlite3 *db):db_(db) {}
    
    int64_t AuthenUser(const string& name, const string& passwd);
    bool IsHaveUser(const string& user);

    bool AddUser(const string& name, const UserInfo &ui);
    bool DelUser(const string& name);
    bool GetUser(const string& name, UserInfo *ui);
    bool SetUser(const string &name, const UserInfo &ui);
    
    private:
    sqlite3 *db_;
};

class SqliteSpaceRepos : public SpaceRepos {
    public:
    explicit SqliteSpaceRepos(sqlite3 *db, SqliteRepos *repos):
            db_(db), repos_(repos) {}

    bool CreateSpace(const string& name, int64_t uid,
            const std::string& space,
            const int type) ;
    bool DeleteSpace(int64_t uid, const string& space);
    bool RenameSpace(int64_t uid, const string& oldspace,
            const string& newspace);
    bool IsHaveSpace(int64_t uid, const string& space);

    bool GetSpaceInfo(int64_t uid, const string& space,
            SpaceInfo *si);
    int GetSpaceList(int64_t uid, list<SpaceInfoPtr> *lsi);

    string GetCred(const string& name, const string& space);

    private:
    sqlite3 *db_;
    SqliteRepos *repos_;
};

class SqliteMetaRepos : public MetaRepos {
    public:
    explicit SqliteMetaRepos(sqlite3 *db):db_(db) {}

    bool AddMeta(const string& name, const string& addr,
            int port, int type);
    bool RmvMeta(int64_t mid);
    bool IsHaveMeta(const string& server);

    bool GetMeta(int64_t mid, MetaInfo *msi);
    bool GetMeta(const string& name, MetaInfo *msi);
    bool GetMeta(list<shared_ptr<MetaInfo> > *lmsi);

    bool AssignMeta(int64_t uid, const string& space, int64_t *msaid);

    private:
    sqlite3 *db_;
};


class SqliteRepos {
    public:

     SqliteRepos() : db_(NULL), isopen_(false) {}
     ~SqliteRepos() { Fini(); }

    bool Init(const string &file);
    void Fini();

    bool IsOpen() { return isopen_; }

    shared_ptr<UserRepos> GetUserRepos() { return user_; }
    shared_ptr<SpaceRepos> GetSpaceRepos() { return space_; }
    shared_ptr<MetaRepos> GetMetaRepos() { return meta_; }

    private:
    shared_ptr<UserRepos>  user_;
    shared_ptr<SpaceRepos> space_;
    shared_ptr<MetaRepos>  meta_;

    string  file_;
    sqlite3 *db_;
    bool    isopen_;
};
}}
#endif  // DIRECTOR_SQLITE_REPOS_H_

