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

    bool Setup();
    void Close();

    bool GetOption(const string& optkey, string& optval);
    bool SetOption(const string& optkey, const string& optval);

    bool GetOptions(map<string,string> optkeyvals);

    private:
    string cred_;
    string addr01_;
    int    port01_;
};


class Tanent {
    public:
    typedef shared_ptr<SpaceClient> SpaceClientPtr;

    bool Setup(const string& file);
    bool Setup(const string& name, const string& passwd);
    bool Close();

    SpaceClientPtr GetSpace(const string& space);
};

}}
#endif  // CLIENT_CLIENT_H_
