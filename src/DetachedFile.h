
#ifndef C_OPENTIMESTAMPS_DETACHEDFILE_H
#define C_OPENTIMESTAMPS_DETACHEDFILE_H


#include <cstdint>
#include "Timestamp.h"
#include "Op.h"
#include "Common.h"

namespace ots{

class DetachedFile {
protected:
	OpCrypto *fileHashOp;
	Timestamp *timestamp;
public:
	static const unsigned char MAJOR_VERSION;
	static const unsigned char HEADER_MAGIC[31];

	DetachedFile(OpCrypto *fileHashOp, Timestamp *timestamp):
	fileHashOp (fileHashOp),
	timestamp (timestamp){
	}

	virtual ~DetachedFile (){
		assert (nullptr != this->timestamp);
		delete this->timestamp;
	}

/*
	DetachedFile(OpCrypto* fileHashOp, const unsigned char* msg, const size_t len){
		this->fileHashOp = fileHashOp;
		unsigned char hash[this->fileHashOp->length()];
		this->fileHashOp->call(msg, len, hash);
		this->timestamp = new Timestamp(hash, this->fileHashOp->length());
	}
*/
	OpCrypto * getFileHashOp() const {
		return this->fileHashOp;
	}
	const unsigned char* fileDigest() const {
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

} // namespace ots

#endif //C_OPENTIMESTAMPS_DETACHEDFILE_H
