/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

namespace cpp xspace.director
namespace php xspace.director


enum SpaceType {
    SPACE_TYPE_FILE = 0,
    SPACE_TYPE_TABLE,
    SPACE_TYPE_OBJECT,
}

struct AddressPair {
    1:string  addr,
    2:i32     port,
}

struct SpaceInfoRsp {
    1:i32                   ret,
    2:i64                   id,
    3:SpaceType             type,
    4:string                cred;
    5:list<AddressPair>     locate,
}

struct SpaceListRsp {
    1:i32           ret,
    2:list<string>  spaces,
}

service SpaceService {

    bool Setup(1:string name, 2:string passwd);
    void Close();

    SpaceInfoRsp GetSpaceInfo(1:string space);
    SpaceListRsp GetSpaceList();
    
    bool CreateSpace(1:string space, 2:SpaceType type);
    bool DeleteSpace(1:string space);
    bool RenameSpace(1:string oldspace, 2:string newspace);
}

service MetaService {
    bool Setup(1:i64 id, 2:string cred);
    void Close();
}

struct MgmtUserInfo {
    1:i64    uid,
    2:string name,
    3:string passwd,
}

service MgmtService {
    bool Setup(1:string name, 2:string passwd);
    void Close();

    // user management
    bool CreateUser(1:string name, 2:string passwd);
    bool DeleteUser(1:string name);
    MgmtUserInfo GetUserInfo(1:string name);
    bool SetUserInfo(1:string name, 2:MgmtUserInfo ui);
    
    // space management
    bool CreateSpace(1:i64 uid, 2:string space, 3:SpaceType type);
    bool DeleteSpace(1:i64 uid, 2:string space);
    SpaceListRsp GetSpaceList(1:i64 uid);
    SpaceInfoRsp GetSpaceInfo(1:i64 uid, 2:string space);
 
    // meta server management
    bool AddMetaServer(1:string addr, 2:i32 port);
    bool RmvMetaServer(1:i64 msaid);
    list<i64> GetAllMetaServer();
    list<i64> GetMetaServer(1:i64 uid, 2:string space);
}
