/*
 * copyright (c) 2011 konghan, All rights reserved.
 */
#include "sqlite_repos.h"

#include <crypto_md5.h>

#include <sqlite3.h>
#include <assert.h>
#include <time.h>

#include <boost/shared_ptr.hpp>
#include <sstream>
#include <string>
#include <list>

namespace xspace { namespace director {

using xspace::misc::crypto_md5;
using std::string;
using std::list;
using std::stringstream;

bool SqliteRepos::Init(const string &file) {
    int ret;

    ret = sqlite3_open_v2(file.c_str(), &db_,
                SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);

    if (ret != SQLITE_OK) {
        return false;
    } else {
        string sql;

        sql  = "CREATE TABLE IF NOT EXISTS tbl_user (";
        sql += "id INTEGER PRIMARY KEY AUTOINCREMENT,";
        sql += "name TEXT NOT NULL,";
        sql += "passwd TEXT NOT NULL";
        sql += ");";
        ret = sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL);
        if (ret != SQLITE_OK) {
            sqlite3_close(db_);
            db_ = NULL;
            return false;
        }
        user_ = shared_ptr<SqliteUserRepos>(new SqliteUserRepos(db_));

        sql  = "CREATE TABLE IF NOT EXISTS tbl_space (";
        sql += "id INTEGER PRIMARY KEY AUTOINCREMENT,";
        sql += "name TEXT NOT NULL,";
        sql += "owner INTEGER NOT NULL,";
        sql += "type INTEGER NOT NULL,";
        sql += "cred INTEGER NOT NULL,";
        sql += "msaid0 INTEGER NOT NULL,";
        sql += "msaid1 INTEGER,";
        sql += "msaid2 INTEGER";
        sql += ");";
        ret = sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL);
        if (ret != SQLITE_OK) {
            sqlite3_close(db_);
            db_ = NULL;
            return false;
        }
        space_ = shared_ptr<SqliteSpaceRepos>(new SqliteSpaceRepos(db_, this));

        sql  = "CREATE TABLE IF NOT EXISTS tbl_msa (";
        sql += "id INTEGER PRIMARY KEY AUTOINCREMENT,";
        sql += "name TEXT NOT NULL,";
        sql += "type INTEGER NOT NULL,";
        sql += "addrv4 TEXT NOT NULL,";
        sql += "addrv6 TEXT NOT NULL,";
        sql += "port INTEGER NOT NULL,";
        sql += "hosted INTEGER NOT NULL";
        sql += ");";
        ret = sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL);
        if (ret != SQLITE_OK) {
            sqlite3_close(db_);
            db_ = NULL;
            return false;
        }
        meta_ = shared_ptr<SqliteMetaRepos>(new SqliteMetaRepos(db_));

        isopen_ = true;
        return true;
    }
}

void SqliteRepos::Fini() {
    if (IsOpen()) {
        user_  = shared_ptr<SqliteUserRepos>();
        space_ = shared_ptr<SqliteSpaceRepos>();
        meta_  = shared_ptr<SqliteMetaRepos>();

        sqlite3_close(db_);
        db_ = NULL;
        isopen_ = false;
    }
}

bool SqliteUserRepos::IsHaveUser(const string& user) {
    int ret;
    sqlite3_stmt *stmt;

    string sql = "SELECT * FROM tbl_user WHERE NAME = \"" + user + "\";";

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(), &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (ret == SQLITE_ROW) ? true : false;
}

bool SqliteSpaceRepos::IsHaveSpace(int64_t uid, const string& space) {
    int ret;
    sqlite3_stmt *stmt;

    stringstream ss;

    ss << "SELECT * FROM tbl_space WHERE owner = \"" << uid;
    ss << "\" and name = \"" + space + "\";";

    ret = sqlite3_prepare_v2(db_, ss.str().c_str(), ss.str().size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (ret == SQLITE_ROW) ? true : false;
}

bool SqliteMetaRepos::IsHaveMeta(const string& server) {
    int ret;
    sqlite3_stmt *stmt;

    string sql = "SELECT * FROM tbl_msa WHERE name = \"";
    sql += server;
    sql += "\";";

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (ret == SQLITE_ROW) ? true : false;
}

int64_t SqliteUserRepos::AuthenUser(const string& name, const string& passwd) {
    int ret;
    int64_t uid = -1;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM tbl_user WHERE name = \"" + name;
    sql += "\" and passwd = \"" + passwd + "\";";

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(), &stmt, NULL);
    if (ret != SQLITE_OK) {
        return -1;
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        uid = sqlite3_column_int64(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return uid;
}

bool SqliteUserRepos::AddUser(const string& name, const UserInfo &ui) {
    if (IsHaveUser(name))
        return false;

    string sql = "INSERT INTO tbl_user VALUES(NULL,\"" + name +  "\",\"";
    sql += ui.passwd + "\");";

    if (sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteUserRepos::DelUser(const string& name) {
    string sql = "DELETE FROM tbl_user WHERE name = \"" + name + "\";";
    if (sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL) != SQLITE_OK)
         return false;
    return true;
}

bool SqliteUserRepos::GetUser(const string& name, UserInfo *ui) {
    assert(ui != NULL);

    int ret;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM tbl_user WHERE name = \"" + name + "\";";

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(), &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        ui->uid = sqlite3_column_int64(stmt, 0);
        ui->passwd.assign((const char *)(sqlite3_column_text(stmt, 2)));
    }

    sqlite3_finalize(stmt);

    return (ret == SQLITE_ROW) ? true : false;
}

bool SqliteUserRepos::SetUser(const string &name, const UserInfo &ui) {

    string sql = "UPDATE tbl_user SET passwd = \"" + ui.passwd + "\"";
    sql += " WHERE name = \"" + name + "\";";

    if (sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteSpaceRepos::GetSpaceInfo(int64_t uid, const string& space,
            SpaceInfo *si) {
    assert(si != NULL);

    int ret;
    sqlite3_stmt *stmt;
    stringstream ss;
    ss << "SELECT * FROM tbl_space WHERE owner = \"";
    ss << uid << "\" and name = \"" << space << "\";";

    ret = sqlite3_prepare_v2(db_, ss.str().c_str(), ss.str().size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        si->id = sqlite3_column_int64(stmt, 0);
        si->name.assign((const char *)(sqlite3_column_text(stmt, 1)));
        si->owner = sqlite3_column_int64(stmt, 2);
        si->type = sqlite3_column_int(stmt, 3);
        si->cred.assign((const char*)sqlite3_column_text(stmt, 4));
        si->msaid0 = sqlite3_column_int64(stmt, 5);
        si->msaid1 = sqlite3_column_int64(stmt, 6);
        si->msaid2 = sqlite3_column_int64(stmt, 7);
    }

    sqlite3_finalize(stmt);

    return (ret == SQLITE_ROW) ? true : false;
}

int SqliteSpaceRepos::GetSpaceList(int64_t uid, list<SpaceInfoPtr> *lsi) {
    assert(lsi != NULL);

    int ret;
    sqlite3_stmt *stmt;
    stringstream ss;
    ss << "SELECT * FROM tbl_space WHERE owner = \"";
    ss << uid << "\";";

    ret = sqlite3_prepare_v2(db_, ss.str().c_str(), ss.str().size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    SpaceInfoPtr sip;

    ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        sip = SpaceInfoPtr(new SpaceInfo);

        sip->id = sqlite3_column_int64(stmt, 0);
        sip->name.assign((const char *)(sqlite3_column_text(stmt, 1)));
        sip->owner = sqlite3_column_int64(stmt, 2);
        sip->type = sqlite3_column_int(stmt, 3);
        sip->cred.assign((const char*)sqlite3_column_text(stmt, 4));
        sip->msaid0 = sqlite3_column_int64(stmt, 5);
        sip->msaid1 = sqlite3_column_int64(stmt, 6);
        sip->msaid2 = sqlite3_column_int64(stmt, 7);

        lsi->push_back(sip);
        ret = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    return lsi->size();
}

bool SqliteSpaceRepos::CreateSpace(const string& name, int64_t uid,
        const std::string& space, const int type) {

    if (IsHaveSpace(uid, space))
        return false;

    stringstream ss;
    ss << "INSERT INTO tbl_space VALUES(NULL, \"";
    ss << space << "\",\"";
    ss << uid << "\",\"";
    ss << type << "\",\"";
    ss << GetCred(name, space) << "\",\"";

    int64_t msaid[3];
    if (!repos_->GetMetaRepos()->AssignMeta(uid, space, msaid)) {
        return false;
    }

    ss <<  msaid[0] <<  "\",\"" << msaid[1] << "\",\"" << msaid[2];
    ss << "\");";

    if (sqlite3_exec(db_, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        return false;
    }

    return true;
}

bool SqliteSpaceRepos::DeleteSpace(int64_t uid, const string& space) {

    stringstream ss;
    ss << "DELETE FROM tbl_space WHERE owner = \"";
    ss << uid << "\" and name = \"" << space << "\";";

    if (sqlite3_exec(db_, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteSpaceRepos::RenameSpace(int64_t uid, const string& oldspace,
            const string& newspace) {

    if (IsHaveSpace(uid, newspace))
        return false;

    stringstream ss;
    ss << "UPDATE tbl_space SET name = \"" << newspace;
    ss << "\" WHERE owner = \"" << uid << "\" and name = \"";
    ss << oldspace << "\";";

    if (sqlite3_exec(db_, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteMetaRepos::AddMeta(const string& name, const string& addr,
        int port, int type) {
    
    if (IsHaveMeta(name))
        return false;

    stringstream ss;
    ss << "INSERT INTO tbl_msa VALUES(NULL,\"";
    ss << name << "\",\"";
    ss << type << "\",\"";
    ss << addr << "\",\"";
    ss << addr << "\",\"";
    ss << port << "\",";
    ss << "\"0\"";
    ss << ")";

    if (sqlite3_exec(db_, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteMetaRepos::RmvMeta(int64_t mid) {

    stringstream ss;
    ss << "DELETE FROM tbl_msa WHERE id  = \"";
    ss << mid << "\";";

    if (sqlite3_exec(db_, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK)
        return false;

    return true;
}

bool SqliteMetaRepos::GetMeta(int64_t mid, MetaInfo *msi) {
    assert(msi != NULL);

    int ret;
    sqlite3_stmt *stmt;
    stringstream ss;
    ss << "SELECT * FROM tbl_msa WHERE id = \"";
    ss << mid << "\";";

    ret = sqlite3_prepare_v2(db_, ss.str().c_str(), ss.str().size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        msi->id = mid;
        msi->name.assign((const char *)(sqlite3_column_text(stmt, 1)));
        msi->type = sqlite3_column_int(stmt, 2);
        msi->addrv4.assign((const char*)sqlite3_column_text(stmt, 3));
        msi->addrv6.assign((const char*)sqlite3_column_text(stmt, 4));
        msi->port = sqlite3_column_int(stmt, 5);
        msi->hosted = sqlite3_column_int64(stmt, 6);
    }

    sqlite3_finalize(stmt);
    return (ret == SQLITE_ROW) ? true : false;
}

bool SqliteMetaRepos::GetMeta(const string& name, MetaInfo *msi) {
    assert(msi != NULL);

    int ret;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM tbl_msa WHERE name = \"";
    sql += name + "\";";

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        msi->id = sqlite3_column_int64(stmt, 0);
        msi->name.assign((const char *)(sqlite3_column_text(stmt, 1)));
        msi->type = sqlite3_column_int(stmt, 2);
        msi->addrv4.assign((const char*)sqlite3_column_text(stmt, 3));
        msi->addrv6.assign((const char*)sqlite3_column_text(stmt, 4));
        msi->port = sqlite3_column_int(stmt, 5);
        msi->hosted = sqlite3_column_int64(stmt, 6);
    }

    sqlite3_finalize(stmt);
    return (ret == SQLITE_ROW) ? true : false;
}

bool SqliteMetaRepos::GetMeta(list<shared_ptr<MetaInfo> > *lmsi) {
    return false;
}


bool SqliteMetaRepos::AssignMeta(int64_t uid, const string& space,
        int64_t *msaid) {
    int ret;
    int count = 0;
    string sql  = "SELECT * FROM tbl_msa ORDER BY hosted ASC;";
    sqlite3_stmt *stmt;

    ret = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(),
            &stmt, NULL);
    if (ret != SQLITE_OK) {
        return false;
    }

    ret = sqlite3_step(stmt);
    while ((ret == SQLITE_ROW) && (count < 3)) {
        msaid[count] = sqlite3_column_int64(stmt, 0);
        count++;
    }

    sqlite3_finalize(stmt);

    return (count == 0)? false:true;
}

string SqliteSpaceRepos::GetCred(const string& name, const string& space) {
    time_t  tm;
    crypto_md5 md;

    time(&tm);
    struct tm tmp;
    char   buf[32];
    string stm = asctime_r(localtime_r(&tm, &tmp), buf);
    string message = name + space + stm;
    md.set(message);

    return md.calculate();
}
}}
