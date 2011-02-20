/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef SpaceService_H
#define SpaceService_H

#include <TProcessor.h>
#include "director_types.h"

namespace xspace { namespace director {

class SpaceServiceIf {
 public:
  virtual ~SpaceServiceIf() {}
  virtual bool Setup(const std::string& name, const std::string& passwd) = 0;
  virtual void Close() = 0;
  virtual void GetSpaceInfo(SpaceInfoRsp& _return, const std::string& space) = 0;
  virtual void GetSpaceList(SpaceListRsp& _return) = 0;
  virtual bool CreateSpace(const std::string& space, const SpaceType::type type) = 0;
  virtual bool DeleteSpace(const std::string& space) = 0;
  virtual bool RenameSpace(const std::string& oldspace, const std::string& newspace) = 0;
};

class SpaceServiceNull : virtual public SpaceServiceIf {
 public:
  virtual ~SpaceServiceNull() {}
  bool Setup(const std::string& /* name */, const std::string& /* passwd */) {
    bool _return = false;
    return _return;
  }
  void Close() {
    return;
  }
  void GetSpaceInfo(SpaceInfoRsp& /* _return */, const std::string& /* space */) {
    return;
  }
  void GetSpaceList(SpaceListRsp& /* _return */) {
    return;
  }
  bool CreateSpace(const std::string& /* space */, const SpaceType::type /* type */) {
    bool _return = false;
    return _return;
  }
  bool DeleteSpace(const std::string& /* space */) {
    bool _return = false;
    return _return;
  }
  bool RenameSpace(const std::string& /* oldspace */, const std::string& /* newspace */) {
    bool _return = false;
    return _return;
  }
};

typedef struct _SpaceService_Setup_args__isset {
  _SpaceService_Setup_args__isset() : name(false), passwd(false) {}
  bool name;
  bool passwd;
} _SpaceService_Setup_args__isset;

class SpaceService_Setup_args {
 public:

  SpaceService_Setup_args() : name(""), passwd("") {
  }

  virtual ~SpaceService_Setup_args() throw() {}

  std::string name;
  std::string passwd;

  _SpaceService_Setup_args__isset __isset;

  bool operator == (const SpaceService_Setup_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(passwd == rhs.passwd))
      return false;
    return true;
  }
  bool operator != (const SpaceService_Setup_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_Setup_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_Setup_pargs {
 public:


  virtual ~SpaceService_Setup_pargs() throw() {}

  const std::string* name;
  const std::string* passwd;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_Setup_result__isset {
  _SpaceService_Setup_result__isset() : success(false) {}
  bool success;
} _SpaceService_Setup_result__isset;

class SpaceService_Setup_result {
 public:

  SpaceService_Setup_result() : success(0) {
  }

  virtual ~SpaceService_Setup_result() throw() {}

  bool success;

  _SpaceService_Setup_result__isset __isset;

  bool operator == (const SpaceService_Setup_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_Setup_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_Setup_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_Setup_presult__isset {
  _SpaceService_Setup_presult__isset() : success(false) {}
  bool success;
} _SpaceService_Setup_presult__isset;

class SpaceService_Setup_presult {
 public:


  virtual ~SpaceService_Setup_presult() throw() {}

  bool* success;

  _SpaceService_Setup_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class SpaceService_Close_args {
 public:

  SpaceService_Close_args() {
  }

  virtual ~SpaceService_Close_args() throw() {}


  bool operator == (const SpaceService_Close_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const SpaceService_Close_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_Close_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_Close_pargs {
 public:


  virtual ~SpaceService_Close_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_Close_result {
 public:

  SpaceService_Close_result() {
  }

  virtual ~SpaceService_Close_result() throw() {}


  bool operator == (const SpaceService_Close_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const SpaceService_Close_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_Close_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_Close_presult {
 public:


  virtual ~SpaceService_Close_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _SpaceService_GetSpaceInfo_args__isset {
  _SpaceService_GetSpaceInfo_args__isset() : space(false) {}
  bool space;
} _SpaceService_GetSpaceInfo_args__isset;

class SpaceService_GetSpaceInfo_args {
 public:

  SpaceService_GetSpaceInfo_args() : space("") {
  }

  virtual ~SpaceService_GetSpaceInfo_args() throw() {}

  std::string space;

  _SpaceService_GetSpaceInfo_args__isset __isset;

  bool operator == (const SpaceService_GetSpaceInfo_args & rhs) const
  {
    if (!(space == rhs.space))
      return false;
    return true;
  }
  bool operator != (const SpaceService_GetSpaceInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_GetSpaceInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_GetSpaceInfo_pargs {
 public:


  virtual ~SpaceService_GetSpaceInfo_pargs() throw() {}

  const std::string* space;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_GetSpaceInfo_result__isset {
  _SpaceService_GetSpaceInfo_result__isset() : success(false) {}
  bool success;
} _SpaceService_GetSpaceInfo_result__isset;

class SpaceService_GetSpaceInfo_result {
 public:

  SpaceService_GetSpaceInfo_result() {
  }

  virtual ~SpaceService_GetSpaceInfo_result() throw() {}

  SpaceInfoRsp success;

  _SpaceService_GetSpaceInfo_result__isset __isset;

  bool operator == (const SpaceService_GetSpaceInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_GetSpaceInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_GetSpaceInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_GetSpaceInfo_presult__isset {
  _SpaceService_GetSpaceInfo_presult__isset() : success(false) {}
  bool success;
} _SpaceService_GetSpaceInfo_presult__isset;

class SpaceService_GetSpaceInfo_presult {
 public:


  virtual ~SpaceService_GetSpaceInfo_presult() throw() {}

  SpaceInfoRsp* success;

  _SpaceService_GetSpaceInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class SpaceService_GetSpaceList_args {
 public:

  SpaceService_GetSpaceList_args() {
  }

  virtual ~SpaceService_GetSpaceList_args() throw() {}


  bool operator == (const SpaceService_GetSpaceList_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const SpaceService_GetSpaceList_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_GetSpaceList_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_GetSpaceList_pargs {
 public:


  virtual ~SpaceService_GetSpaceList_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_GetSpaceList_result__isset {
  _SpaceService_GetSpaceList_result__isset() : success(false) {}
  bool success;
} _SpaceService_GetSpaceList_result__isset;

class SpaceService_GetSpaceList_result {
 public:

  SpaceService_GetSpaceList_result() {
  }

  virtual ~SpaceService_GetSpaceList_result() throw() {}

  SpaceListRsp success;

  _SpaceService_GetSpaceList_result__isset __isset;

  bool operator == (const SpaceService_GetSpaceList_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_GetSpaceList_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_GetSpaceList_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_GetSpaceList_presult__isset {
  _SpaceService_GetSpaceList_presult__isset() : success(false) {}
  bool success;
} _SpaceService_GetSpaceList_presult__isset;

class SpaceService_GetSpaceList_presult {
 public:


  virtual ~SpaceService_GetSpaceList_presult() throw() {}

  SpaceListRsp* success;

  _SpaceService_GetSpaceList_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _SpaceService_CreateSpace_args__isset {
  _SpaceService_CreateSpace_args__isset() : space(false), type(false) {}
  bool space;
  bool type;
} _SpaceService_CreateSpace_args__isset;

class SpaceService_CreateSpace_args {
 public:

  SpaceService_CreateSpace_args() : space("") {
  }

  virtual ~SpaceService_CreateSpace_args() throw() {}

  std::string space;
  SpaceType::type type;

  _SpaceService_CreateSpace_args__isset __isset;

  bool operator == (const SpaceService_CreateSpace_args & rhs) const
  {
    if (!(space == rhs.space))
      return false;
    if (!(type == rhs.type))
      return false;
    return true;
  }
  bool operator != (const SpaceService_CreateSpace_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_CreateSpace_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_CreateSpace_pargs {
 public:


  virtual ~SpaceService_CreateSpace_pargs() throw() {}

  const std::string* space;
  const SpaceType::type* type;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_CreateSpace_result__isset {
  _SpaceService_CreateSpace_result__isset() : success(false) {}
  bool success;
} _SpaceService_CreateSpace_result__isset;

class SpaceService_CreateSpace_result {
 public:

  SpaceService_CreateSpace_result() : success(0) {
  }

  virtual ~SpaceService_CreateSpace_result() throw() {}

  bool success;

  _SpaceService_CreateSpace_result__isset __isset;

  bool operator == (const SpaceService_CreateSpace_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_CreateSpace_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_CreateSpace_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_CreateSpace_presult__isset {
  _SpaceService_CreateSpace_presult__isset() : success(false) {}
  bool success;
} _SpaceService_CreateSpace_presult__isset;

class SpaceService_CreateSpace_presult {
 public:


  virtual ~SpaceService_CreateSpace_presult() throw() {}

  bool* success;

  _SpaceService_CreateSpace_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _SpaceService_DeleteSpace_args__isset {
  _SpaceService_DeleteSpace_args__isset() : space(false) {}
  bool space;
} _SpaceService_DeleteSpace_args__isset;

class SpaceService_DeleteSpace_args {
 public:

  SpaceService_DeleteSpace_args() : space("") {
  }

  virtual ~SpaceService_DeleteSpace_args() throw() {}

  std::string space;

  _SpaceService_DeleteSpace_args__isset __isset;

  bool operator == (const SpaceService_DeleteSpace_args & rhs) const
  {
    if (!(space == rhs.space))
      return false;
    return true;
  }
  bool operator != (const SpaceService_DeleteSpace_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_DeleteSpace_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_DeleteSpace_pargs {
 public:


  virtual ~SpaceService_DeleteSpace_pargs() throw() {}

  const std::string* space;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_DeleteSpace_result__isset {
  _SpaceService_DeleteSpace_result__isset() : success(false) {}
  bool success;
} _SpaceService_DeleteSpace_result__isset;

class SpaceService_DeleteSpace_result {
 public:

  SpaceService_DeleteSpace_result() : success(0) {
  }

  virtual ~SpaceService_DeleteSpace_result() throw() {}

  bool success;

  _SpaceService_DeleteSpace_result__isset __isset;

  bool operator == (const SpaceService_DeleteSpace_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_DeleteSpace_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_DeleteSpace_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_DeleteSpace_presult__isset {
  _SpaceService_DeleteSpace_presult__isset() : success(false) {}
  bool success;
} _SpaceService_DeleteSpace_presult__isset;

class SpaceService_DeleteSpace_presult {
 public:


  virtual ~SpaceService_DeleteSpace_presult() throw() {}

  bool* success;

  _SpaceService_DeleteSpace_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _SpaceService_RenameSpace_args__isset {
  _SpaceService_RenameSpace_args__isset() : oldspace(false), newspace(false) {}
  bool oldspace;
  bool newspace;
} _SpaceService_RenameSpace_args__isset;

class SpaceService_RenameSpace_args {
 public:

  SpaceService_RenameSpace_args() : oldspace(""), newspace("") {
  }

  virtual ~SpaceService_RenameSpace_args() throw() {}

  std::string oldspace;
  std::string newspace;

  _SpaceService_RenameSpace_args__isset __isset;

  bool operator == (const SpaceService_RenameSpace_args & rhs) const
  {
    if (!(oldspace == rhs.oldspace))
      return false;
    if (!(newspace == rhs.newspace))
      return false;
    return true;
  }
  bool operator != (const SpaceService_RenameSpace_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_RenameSpace_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SpaceService_RenameSpace_pargs {
 public:


  virtual ~SpaceService_RenameSpace_pargs() throw() {}

  const std::string* oldspace;
  const std::string* newspace;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_RenameSpace_result__isset {
  _SpaceService_RenameSpace_result__isset() : success(false) {}
  bool success;
} _SpaceService_RenameSpace_result__isset;

class SpaceService_RenameSpace_result {
 public:

  SpaceService_RenameSpace_result() : success(0) {
  }

  virtual ~SpaceService_RenameSpace_result() throw() {}

  bool success;

  _SpaceService_RenameSpace_result__isset __isset;

  bool operator == (const SpaceService_RenameSpace_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SpaceService_RenameSpace_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpaceService_RenameSpace_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _SpaceService_RenameSpace_presult__isset {
  _SpaceService_RenameSpace_presult__isset() : success(false) {}
  bool success;
} _SpaceService_RenameSpace_presult__isset;

class SpaceService_RenameSpace_presult {
 public:


  virtual ~SpaceService_RenameSpace_presult() throw() {}

  bool* success;

  _SpaceService_RenameSpace_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class SpaceServiceClient : virtual public SpaceServiceIf {
 public:
  SpaceServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  SpaceServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool Setup(const std::string& name, const std::string& passwd);
  void send_Setup(const std::string& name, const std::string& passwd);
  bool recv_Setup();
  void Close();
  void send_Close();
  void recv_Close();
  void GetSpaceInfo(SpaceInfoRsp& _return, const std::string& space);
  void send_GetSpaceInfo(const std::string& space);
  void recv_GetSpaceInfo(SpaceInfoRsp& _return);
  void GetSpaceList(SpaceListRsp& _return);
  void send_GetSpaceList();
  void recv_GetSpaceList(SpaceListRsp& _return);
  bool CreateSpace(const std::string& space, const SpaceType::type type);
  void send_CreateSpace(const std::string& space, const SpaceType::type type);
  bool recv_CreateSpace();
  bool DeleteSpace(const std::string& space);
  void send_DeleteSpace(const std::string& space);
  bool recv_DeleteSpace();
  bool RenameSpace(const std::string& oldspace, const std::string& newspace);
  void send_RenameSpace(const std::string& oldspace, const std::string& newspace);
  bool recv_RenameSpace();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class SpaceServiceProcessor : virtual public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<SpaceServiceIf> iface_;
  virtual bool process_fn(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid, int socket);
 private:
  std::map<std::string, void (SpaceServiceProcessor::*)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*)> processMap_;
  void process_Setup(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_Close(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_GetSpaceInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_GetSpaceList(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_CreateSpace(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_DeleteSpace(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_RenameSpace(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
 public:
  SpaceServiceProcessor(boost::shared_ptr<SpaceServiceIf> iface) :
    iface_(iface) {
    processMap_["Setup"] = &SpaceServiceProcessor::process_Setup;
    processMap_["Close"] = &SpaceServiceProcessor::process_Close;
    processMap_["GetSpaceInfo"] = &SpaceServiceProcessor::process_GetSpaceInfo;
    processMap_["GetSpaceList"] = &SpaceServiceProcessor::process_GetSpaceList;
    processMap_["CreateSpace"] = &SpaceServiceProcessor::process_CreateSpace;
    processMap_["DeleteSpace"] = &SpaceServiceProcessor::process_DeleteSpace;
    processMap_["RenameSpace"] = &SpaceServiceProcessor::process_RenameSpace;
  }

  virtual bool process(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot, int socket);
  virtual ~SpaceServiceProcessor() {}
};

class SpaceServiceMultiface : virtual public SpaceServiceIf {
 public:
  SpaceServiceMultiface(std::vector<boost::shared_ptr<SpaceServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~SpaceServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<SpaceServiceIf> > ifaces_;
  SpaceServiceMultiface() {}
  void add(boost::shared_ptr<SpaceServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool Setup(const std::string& name, const std::string& passwd) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->Setup(name, passwd);
      } else {
        ifaces_[i]->Setup(name, passwd);
      }
    }
  }

  void Close() {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      ifaces_[i]->Close();
    }
  }

  void GetSpaceInfo(SpaceInfoRsp& _return, const std::string& space) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        ifaces_[i]->GetSpaceInfo(_return, space);
        return;
      } else {
        ifaces_[i]->GetSpaceInfo(_return, space);
      }
    }
  }

  void GetSpaceList(SpaceListRsp& _return) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        ifaces_[i]->GetSpaceList(_return);
        return;
      } else {
        ifaces_[i]->GetSpaceList(_return);
      }
    }
  }

  bool CreateSpace(const std::string& space, const SpaceType::type type) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->CreateSpace(space, type);
      } else {
        ifaces_[i]->CreateSpace(space, type);
      }
    }
  }

  bool DeleteSpace(const std::string& space) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->DeleteSpace(space);
      } else {
        ifaces_[i]->DeleteSpace(space);
      }
    }
  }

  bool RenameSpace(const std::string& oldspace, const std::string& newspace) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->RenameSpace(oldspace, newspace);
      } else {
        ifaces_[i]->RenameSpace(oldspace, newspace);
      }
    }
  }

};

}} // namespace

#endif