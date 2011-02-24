/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <sqlite3.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>

namespace xspace { namespace client {

using std::string;

class SpaceClient {
    public:
    virtual ~SpaceClient() { }

    bool Setup(const string& name, const string& passwd);
    void Close();

    virtual bool GetOption(const string& optkey, string& optval);
    virtual bool SetOption(const string& optkey, const string& optval);
    virtual bool GetOptions(map<string,string> optkeyvals);

    private:
    string cred_;
    string addr01_;
    int    port01_;
};


class Client {
    public:
    typedef shared_ptr<SpaceClient> SpaceClientPtr;

    bool Setup(const string& file);
    bool Setup(const string& name, const string& passwd);
    bool Close();

    SpaceClientPtr GetSpace(const string& space);
    
    bool CreateSpace(const string& space, const SpaceType& type);
    bool DeleteSpace(const string& space);
    bool RenameSpace(const string& oldspace, const string& newspace);
};

}}
#endif  // CLIENT_CLIENT_H_
