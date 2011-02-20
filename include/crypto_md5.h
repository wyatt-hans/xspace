#ifndef XSPACE_MISC_CRYPTO_MD5_H_
#define XSPACE_MISC_CRYPTO_MD5_H_

#include <string>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

namespace xspace { namespace misc {

class crypto_md5 {
 public:
     void reset();
     void update(std::string& message);
     void set(std::string& message);
     std::string calculate();
 private:
    unsigned char digest_[CryptoPP::Weak::MD5::DIGESTSIZE];
    std::string message_;
};
} }
#endif  // XSPACE_MISC_CRYPTO_MD5_H_
