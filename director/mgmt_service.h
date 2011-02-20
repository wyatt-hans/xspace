// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "MgmtService.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace portals;

class MgmtServiceHandler : virtual public MgmtServiceIf {
 public:
  MgmtServiceHandler() {
    // Your initialization goes here
  }

  int32_t Setup(const std::string& name, const std::string& passwd) {
    // Your implementation goes here
    printf("Setup\n");
  }

  int32_t Close() {
    // Your implementation goes here
    printf("Close\n");
  }

  int64_t CreateUser(const std::string& name, const std::string& passwd) {
    // Your implementation goes here
    printf("CreateUser\n");
  }

  int64_t GetUserID(const std::string& name) {
    // Your implementation goes here
    printf("GetUserID\n");
  }

  int32_t DeleteUser(const int64_t userID) {
    // Your implementation goes here
    printf("DeleteUser\n");
  }

  int32_t ChangePasswd(const int64_t userID, const std::string& oldpasswd, const std::string& newpasswd) {
    // Your implementation goes here
    printf("ChangePasswd\n");
  }

  int32_t CreateSpace(const int64_t userID, const std::string& space, const space_type_e::type type) {
    // Your implementation goes here
    printf("CreateSpace\n");
  }

  int32_t DeleteSpace(const int64_t userID, const std::string& space) {
    // Your implementation goes here
    printf("DeleteSpace\n");
  }

  void Getspace_info_tList(std::vector<space_info_t> & _return, const int64_t userID) {
    // Your implementation goes here
    printf("Getspace_info_tList\n");
  }

  void GetSpaceList(space_list_t& _return, const int64_t userID) {
    // Your implementation goes here
    printf("GetSpaceList\n");
  }

  void GetSpaceInfo(space_info_t& _return, const int64_t userID, const int64_t spaceID) {
    // Your implementation goes here
    printf("GetSpaceInfo\n");
  }

  int64_t CreateMetaServer(const meta_info_t& metaInfo) {
    // Your implementation goes here
    printf("CreateMetaServer\n");
  }

  int32_t DeleteMetaServer(const int64_t metaID, const std::string& key) {
    // Your implementation goes here
    printf("DeleteMetaServer\n");
  }

  void GetMetaServer(meta_info_t& _return, const int64_t metaID) {
    // Your implementation goes here
    printf("GetMetaServer\n");
  }

  void GetMetaServerList(std::vector<meta_info_t> & _return) {
    // Your implementation goes here
    printf("GetMetaServerList\n");
  }

};
