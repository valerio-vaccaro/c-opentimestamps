
#ifndef C_OPENTIMESTAMPS_OP_H
#define C_OPENTIMESTAMPS_OP_H


#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include "openssl/sha.h"
#include "openssl/ripemd.h"
#include "Common.h"
#include "Context.h"

class Op {
protected:
	const int32_t MAX_MESSAGE_LENGHT = 4096;
	const uint8_t TAG;
	const std::string TAG_NAME;
public:
	Op(const uint8_t tag, const std::string &tag_name):
	TAG(tag),
	TAG_NAME(tag_name)
	{};
	uint8_t tag() const{
		return this->TAG;
	}
	std::string tagName() const{
		return this->TAG_NAME;
	}
	virtual int length() = 0;
	virtual int call(const uint8_t *msg, const int32_t len, uint8_t *output) = 0;
	virtual void serialize(Serialize ctx) = 0;
	//virtual const void deserializeFromTag(Op* op, Deserialize ctx, uint8_t tag) = 0;
};

class OpBinary : public Op {
public:
	uint8_t* arg;
	uint32_t len;
	OpBinary(const uint8_t tag, const std::string &tag_name, uint8_t* arg, uint32_t len) :
	Op(tag, tag_name),
	arg(arg),
	len(len){
	}
	void serialize(Serialize ctx) override {
		uint8_t tag = this->tag();
		ctx.writeVaruints(&tag, 1);
		ctx.writeVaruints(arg, len);
	}
};

class OpUnary : public Op {
public:
	OpUnary(const uint8_t tag, const std::string &tag_name) : Op(tag, tag_name){}
	void serialize(Serialize ctx) override {
		uint8_t tag = this->tag();
		ctx.writeVaruints(&tag, 1);
	}
};

class OpCrypto : public OpUnary {
public:
	OpCrypto(const uint8_t tag, const std::string &tag_name) : OpUnary(tag, tag_name){}
};

// Define operators

inline std::ostream& operator<<(std::ostream& out, const OpBinary &op) {
	out << op.tagName() << " " << hexStr(op.arg, op.len) << "\n";
	return out;
}

inline std::ostream& operator<<(std::ostream& out, const OpUnary &op) {
	out << op.tagName() << "\n";
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
	OpAppend(uint8_t *arg, int32_t len) : OpBinary(0xf0, "append", arg, len) {}

	int length() override {
		return this->len;
	}
	int call(const uint8_t *msg, const int32_t len, uint8_t *output) override {
		memcpy(output, this->arg, this->len);
		memcpy(output+this->len, msg, len);
		return this->len+len;
	}
};

class OpPrepend : public OpBinary {
public:
	OpPrepend(uint8_t *arg, int32_t len) : OpBinary(0xf1, "prepend", arg, len) {}

	int length() override {
		return this->len;
	}
	int call(const uint8_t *msg, const int32_t len, uint8_t *output) override {
		memcpy(output, msg, len);
		memcpy(output+len, this->arg, this->len);
		return this->len+len;
	}
};

// Crypto-Unary class
class OpSha1 : public OpCrypto {
public:
	OpSha1 (): OpCrypto(0x01, "sha1"){}

	int length() override {
		return SHA_DIGEST_LENGTH;
	}
	int call(const uint8_t *msg, const int32_t len, uint8_t *hash) override {
		SHA_CTX sha1;
		SHA1_Init(&sha1);
		SHA1_Update(&sha1, msg, len);
		SHA1_Final(hash, &sha1);
		return SHA_DIGEST_LENGTH;
	}
};

class OpSha256 : public OpCrypto {
public:
	OpSha256 (): OpCrypto(0x02, "sha256"){}

	int length() override {
		return SHA256_DIGEST_LENGTH;
	}
	int call(const uint8_t *msg, const int32_t len, uint8_t *hash) override {
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, msg, len);
		SHA256_Final(hash, &sha256);
		return SHA256_DIGEST_LENGTH;
	}
};

class OpRipemd160 : public OpCrypto {
public:
	OpRipemd160 (): OpCrypto(0x03, "ripemd160"){}

	int length() override {
		return RIPEMD160_DIGEST_LENGTH;
	}
	int call(const uint8_t *msg, const int32_t len, uint8_t *hash) override {
		RIPEMD160_CTX ripemd160;
		RIPEMD160_Init(&ripemd160);
		RIPEMD160_Update(&ripemd160, msg, len);
		RIPEMD160_Final(hash, &ripemd160);
		return RIPEMD160_DIGEST_LENGTH;
	}
};


#endif //C_OPENTIMESTAMPS_OP_H
