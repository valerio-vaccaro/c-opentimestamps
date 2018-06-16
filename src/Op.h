//
// Created by luca on 11/06/2018.
//

#ifndef C_OPENTIMESTAMPS_OP_H
#define C_OPENTIMESTAMPS_OP_H


#include <cstdint>
#include <cstdio>
#include <cstring>
#include "openssl/sha.h"
#include "openssl/ripemd.h"

class Op {
public:
	int32_t max_message_lenght = 4096;
	uint8_t tag = 0x00;

	Op(){}
	int length();
	int call(uint8_t *msg, uint8_t *output);
};

class OpBinary : Op {
protected:
	uint8_t* arg;
public:
	OpBinary(uint8_t* arg){
		this->arg = arg;
	}
};

class OpUnary : Op {
protected:
	uint8_t* arg;
public:
	OpUnary(){}
};


class OpCrypto : OpUnary {
public:
	OpCrypto(){}
};



// Binary class
class OpAppend : OpBinary {
public:
	const uint8_t TAG = 0xf0;
	OpAppend(uint8_t *arg) : OpBinary(arg) {}
	int length() {
		return sizeof(this->arg);
	}
	int call(uint8_t *msg, uint8_t *output){
		memcpy(output, this->arg, sizeof(this->arg));
		memcpy(output+sizeof(this->arg), msg, sizeof(msg));
		return sizeof(this->arg)+sizeof(msg);
	}
};

class OpPrepend : OpBinary {
public:
	const uint8_t TAG = 0xf1;
	OpPrepend(uint8_t *arg) : OpBinary(arg) {}
	int length() {
		return sizeof(arg);
	}
	int call(uint8_t *msg, uint8_t *output){
		memcpy(output, msg, sizeof(msg));
		memcpy(output+sizeof(msg), this->arg, sizeof(this->arg));
		return sizeof(this->arg)+sizeof(msg);
	}
};

// Crypto-Unary class
class OpSha1 : OpCrypto {
public:
	const uint8_t TAG = 0x02;

	int length() {
		return SHA_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, uint8_t *hash)
	{
		SHA_CTX sha1;
		SHA1_Init(&sha1);
		SHA1_Update(&sha1, msg, sizeof(msg));
		SHA1_Final(hash, &sha1);
		return SHA_DIGEST_LENGTH;
	}
};

class OpSha256 : OpCrypto {
public:
	const uint8_t TAG = 0x02;
	int length() {
		return SHA256_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, uint8_t *hash)
	{
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, msg, sizeof(msg));
		SHA256_Final(hash, &sha256);
		return SHA256_DIGEST_LENGTH;
	}
};

class OpRipemd160 : OpCrypto {
public:
	const uint8_t TAG = 0x03;
	int length() {
		return RIPEMD160_DIGEST_LENGTH;
	}
	int call(uint8_t *msg, uint8_t *hash)
	{
		RIPEMD160_CTX ripemd160;
		RIPEMD160_Init(&ripemd160);
		RIPEMD160_Update(&ripemd160, msg, sizeof(msg));
		RIPEMD160_Final(hash, &ripemd160);
		return RIPEMD160_DIGEST_LENGTH;
	}
};


#endif //C_OPENTIMESTAMPS_OP_H
