/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef CLIENT_FSPACE_H_
#define CLIENT_FSPACE_H_

#include <sqlite3.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>

namespace xspace { namespace client {

using std::string;

class FSpace: public SpaceClient {
    public:
    typedef int64_t fid_type;
    typedef int64_t did_type;

    // file system operates
    int fstat();
    int chmod();
    int chown();

    FilePtr GetFile(const string& path, const string& mode);
 
    // directory operates
    int mkdir(const string& path, const string& mode);
    int rmdir(const string& path);
    int remove(const string& path);

    did_type opendir(const string& path);
    int closedir(did_type did);
    void seekdir(did_type did, long offset);
    long telldir(did_type did);
    int readdir(did_type did, const dirent &entry, dirent **result);

    // file operates
    fid_type fopen(const string& path, const string& mode);
    int fclose(fid_type fid);

    size_t fread(void *ptr, size_t size, size_t nmemb, fid_type fid);
    size_t fread(fid_type fid, void *buf, size_t size);

    size_t fwrite(const void *ptr, size_t size, size_t nmemb, fid_type fid);
    size_t fwrite(fid_type fid, const void *buf, size_t size);

    int fseek(fid_type fid, long offset, int whence);
    long ftell(fid_type fid);
    int feof(fid_type fid);
    int ferror(fid_type fid);

    private:
};

}}
#endif  // CLIENT_FSPACE_H_
