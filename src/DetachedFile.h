
#ifndef C_OPENTIMESTAMPS_DETACHEDFILE_H
#define C_OPENTIMESTAMPS_DETACHEDFILE_H


#include <cstdint>
#include "Timestamp.h"
#include "Op.h"
#include "Common.h"

class DetachedFile {
protected:
	OpCrypto *fileHashOp;
	Timestamp *timestamp;
public:
	const uint8_t HEADER_MAGIC[31] = {(uint8_t) 0x00, (uint8_t) 0x4f, (uint8_t) 0x70, (uint8_t) 0x65, (uint8_t) 0x6e,
								  (uint8_t) 0x54, (uint8_t) 0x69, (uint8_t) 0x6d, (uint8_t) 0x65, (uint8_t) 0x73,
								  (uint8_t) 0x74, (uint8_t) 0x61, (uint8_t) 0x6d, (uint8_t) 0x70, (uint8_t) 0x73, (uint8_t) 0x00, (uint8_t) 0x00,
								  (uint8_t) 0x50, (uint8_t) 0x72, (uint8_t) 0x6f, (uint8_t) 0x6f, (uint8_t) 0x66, (uint8_t) 0x00,
								  (uint8_t) 0xbf, (uint8_t) 0x89, (uint8_t) 0xe2, (uint8_t) 0xe8, (uint8_t) 0x84, (uint8_t) 0xe8, (uint8_t) 0x92,
								  (uint8_t) 0x94};
	const uint8_t MAJOR_VERSION = 1;

	DetachedFile(OpCrypto *fileHashOp, Timestamp *timestamp){
		this->fileHashOp = fileHashOp;
		this->timestamp = timestamp;
	}

	DetachedFile(OpCrypto* fileHashOp, uint8_t* msg, uint32_t len){
		this->fileHashOp = fileHashOp;
		uint8_t hash[this->fileHashOp->length()];
		this->fileHashOp->call(msg, len, hash);
		this->timestamp = new Timestamp(hash, this->fileHashOp->length());
	}

	OpCrypto * getFileHashOp() const {
		return this->fileHashOp;
	}
	uint8_t* fileDigest() const {
		return this->timestamp->getDigest();
	}
	uint32_t fileDigestLenght() const {
		return this->timestamp->getDigestLenght();
	}
	Timestamp* getTimestamp() const {
		return this->timestamp;
	}

};

inline std::ostream& operator<<(std::ostream& out, const DetachedFile* detached) {
	out << "fileHashOp: " << detached->getFileHashOp() <<
		"timestamp: " << hexStr(detached->fileDigest(), detached->fileDigestLenght()) << '\n';
	return out;
}

#endif //C_OPENTIMESTAMPS_DETACHEDFILE_H
