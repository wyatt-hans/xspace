
#include "crypto_md5.h"

#include <iostream>
#include <string>

namespace xspace { namespace misc {
using std::string;

void crypto_md5::reset() {
    message_.clear();
}

void crypto_md5::update(string& message) {
    message_ += message;
}

void crypto_md5::set(string& message) {
    message_ = message;
}

std::string crypto_md5::calculate() {
    CryptoPP::Weak::MD5 hash;
    hash.CalculateDigest(digest_, (const byte*)message_.c_str(), message_.size());
    
    CryptoPP::HexEncoder encoder;
    string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest_, sizeof(digest_));
    encoder.MessageEnd();

    return output;
}
} }
