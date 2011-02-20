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
    typedef int64_t fid_t;
    typedef int64_t did_t;

    int fstat();
 
    int mkdir(const string& path, const string& mode);
    int rmdir(const string& path);
    int remove(const string& path);

    did_t opendir(const string& path);
    int closedir(did_t did);
    void seekdir(did_t did, long offset);
    long telldir(did_t did);
    int readdir(did_t did, const dirent &entry, dirent **result);

    fid_t fopen(const string& path, const string& mode);
    int fclose(fid_t fid);

    size_t fread(void *ptr, size_t size, size_t nmemb, fid_t fid);
    sizt_t fread(fid_t fid, void *buf, size_t size);

    size_t fwrite(const void *ptr, size_t size, size_t nmemb, fid_t fid);
    size_t fwrite(fid_t fid, const void *buf, size_t size);

    int fseek(fid_t fid, long offset, int whence);
    long ftell(fid_t fid);
    int feof(fid_t fid);
    int ferror(fid_t fid);

    private:
};

}}
#endif  // CLIENT_FSPACE_H_
