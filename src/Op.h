
#ifndef C_OPENTIMESTAMPS_OP_H
#define C_OPENTIMESTAMPS_OP_H


#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include "openssl/sha.h"
#include "openssl/ripemd.h"
#include "Common.h"

class Op {
public:
	const int32_t MAX_MESSAGE_LENGHT = 4096;
	const uint8_t TAG = 0x00;
	std::string TAG_NAME = "\0";

	Op(){};
	virtual uint8_t tag() = 0;
	virtual std::string tagName() = 0;
	virtual int length() = 0;
	virtual int call(uint8_t *msg, int32_t len, uint8_t *output) = 0;

	//virtual uint32_t serialize(uint8_t *bytes) = 0;
	//virtual int deserialize(uint8_t *bytes, uint32_t len) = 0;

};

class OpBinary : public Op {
public:
	uint8_t* arg;
	uint32_t len;
	OpBinary(uint8_t* arg, uint32_t len) : Op(){
		this->arg = arg;
		this->len = len;
	}
};

class OpUnary : public Op {
public:
	OpUnary() : Op(){}
};

class OpCrypto : public OpUnary {
public:
	OpCrypto() : OpUnary(){}
};

// Define operators

inline std::ostream& operator<<(std::ostream& out, OpBinary* op) {
	out << op->tagName() << " " << hexStr(op->arg, op->len) << "\n";
	return out;
}

inline std::ostream& operator<<(std::ostream& out, OpUnary* op) {
	out << op->tagName() << "\n";
	return out;
}

struct less_op: std::binary_function<const Op *, const Op *, bool>
{
	bool  operator() (Op *a, Op *b) const {
		return a->tag() > b->tag();
	}
};

// Binary class
class OpAppend : public OpBinary {
public:
	const uint8_t TAG = 0xf0;
	const std::string TAG_NAME = "append";

	OpAppend(uint8_t *arg, int32_t len) : OpBinary(arg, len) {}

	uint8_t tag() override {
		return TAG;
	}
	std::string tagName() override {
		return TAG_NAME;
	}
	int length() override {
		return sizeof(this->arg);
	}
	int call(uint8_t *msg, int32_t len, uint8_t *output) override {
		memcpy(output, this->arg, this->len);
		memcpy(output+this->len, msg, len);
		return this->len+len;
	}

};

class OpPrepend : public OpBinary {
public:
	const uint8_t TAG = 0xf1;
	const std::string TAG_NAME = "prepend";
	OpPrepend(uint8_t *arg, int32_t len) : OpBinary(arg, len) {}

	uint8_t tag() override {
		return TAG;
	}
	std::string tagName() override {
		return TAG_NAME;
	}
	int length() override {
		return sizeof(arg);
	}
	int call(uint8_t *msg, int32_t len, uint8_t *output) override {
		memcpy(output, msg, len);
		memcpy(output+len, this->arg, this->len);
		return this->len+len;
	}
};

// Crypto-Unary class
class OpSha1 : public OpCrypto {
public:
	const uint8_t TAG = 0x02;
	const std::string TAG_NAME = "sha1";

	uint8_t tag() override {
		return TAG;
	}
	std::string tagName() override {
		return TAG_NAME;
	}
	int length() override {
		return SHA_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, int32_t len, uint8_t *hash) override {
		SHA_CTX sha1;
		SHA1_Init(&sha1);
		SHA1_Update(&sha1, msg, len);
		SHA1_Final(hash, &sha1);
		return SHA_DIGEST_LENGTH;
	}
};

class OpSha256 : public OpCrypto {
public:
	const uint8_t TAG = 0x02;
	const std::string TAG_NAME = "sha256";

	uint8_t tag() override {
		return TAG;
	}
	std::string tagName() override {
		return TAG_NAME;
	}
	int length() override {
		return SHA256_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, int32_t len, uint8_t *hash) override {
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, msg, len);
		SHA256_Final(hash, &sha256);
		return SHA256_DIGEST_LENGTH;
	}
};

class OpRipemd160 : public OpCrypto {
public:
	const uint8_t TAG = 0x03;
	const std::string TAG_NAME = "ripemd160";

	uint8_t tag() override {
		return TAG;
	}
	std::string tagName() override {
		return TAG_NAME;
	}
	int length() override {
		return RIPEMD160_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, int32_t len, uint8_t *hash) override {
		RIPEMD160_CTX ripemd160;
		RIPEMD160_Init(&ripemd160);
		RIPEMD160_Update(&ripemd160, msg, len);
		RIPEMD160_Final(hash, &ripemd160);
		return RIPEMD160_DIGEST_LENGTH;
	}
};


#endif //C_OPENTIMESTAMPS_OP_H
