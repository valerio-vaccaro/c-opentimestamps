
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
	static const uint8_t MAJOR_VERSION;
	static const uint8_t HEADER_MAGIC[31];

	DetachedFile(OpCrypto *fileHashOp, Timestamp *timestamp){
		this->fileHashOp = fileHashOp;
		this->timestamp = timestamp;
	}

	DetachedFile(OpCrypto* fileHashOp, const uint8_t* msg, const uint32_t len){
		this->fileHashOp = fileHashOp;
		uint8_t hash[this->fileHashOp->length()];
		this->fileHashOp->call(msg, len, hash);
		this->timestamp = new Timestamp(hash, this->fileHashOp->length());
	}

	OpCrypto * getFileHashOp() const {
		return this->fileHashOp;
	}
	const uint8_t* fileDigest() const {
		return this->timestamp->getDigest();
	}
	uint32_t fileDigestLenght() const {
		return this->timestamp->getDigestLenght();
	}
	Timestamp* getTimestamp() const {
		return this->timestamp;
	}
	void serialize(Serialize *ctx);
	static DetachedFile* deserialize(Deserialize *ctx);

};

inline std::ostream& operator<<(std::ostream& out, const DetachedFile &detached) {
	out << "op: " << *detached.getFileHashOp() <<
		"digest: " << toHex(detached.fileDigest(), detached.fileDigestLenght()) << '\n' <<
		*detached.getTimestamp() << std::endl;
	return out;
}

#endif //C_OPENTIMESTAMPS_DETACHEDFILE_H
