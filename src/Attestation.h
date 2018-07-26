
#ifndef C_OPENTIMESTAMPS_ATTESTATION_H
#define C_OPENTIMESTAMPS_ATTESTATION_H


#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Context.h"

namespace ots{

class TimeAttestation {
protected:
	static const int TAG_SIZE = 8;
	static const int  MAX_PAYLOAD_SIZE = 8192;
	const unsigned char *TAG;
public:
	TimeAttestation(const unsigned char *tag) : TAG(tag){}
	virtual ~TimeAttestation(){}
	virtual bool operator==(TimeAttestation& other);
	virtual void serialize_payload(Serialize *ctx) const = 0;
	void serialize(Serialize *ctx) const;
	static TimeAttestation* deserialize(Deserialize *ctx);
};

class PendingAttestation : public TimeAttestation{
private:
	unsigned char *uri;
	size_t len;
	static const size_t MAX_URI_LENGTH = 1000;
public:
	static const unsigned char TAG[TimeAttestation::TAG_SIZE];
	PendingAttestation(unsigned char* msg, unsigned char len): TimeAttestation(TAG){
		this->len = len;
		this->uri = new unsigned char [len];
		std::copy(msg, msg + len, this->uri);
	}
	unsigned char * getUri() const {
		return this->uri;
	}
	unsigned char getUriLen() const {
		return this->len;
	}
	static bool checkUri(unsigned char* uri, size_t len){
		// TODO
		return true;
	}

	bool operator==(TimeAttestation& other) override {
		PendingAttestation *ptr = dynamic_cast<PendingAttestation *>(&other);
		return compare(this->uri,this->len,ptr->uri,this->len);
	}
	void serialize_payload(Serialize *ctx) const override;
	static PendingAttestation* deserialize(Deserialize *ctx);
};


class BitcoinBlockHeaderAttestation : public TimeAttestation{
private:
	uint32_t height;
public:
	static const unsigned char TAG[TimeAttestation::TAG_SIZE];

	BitcoinBlockHeaderAttestation(uint32_t height): TimeAttestation(TAG){
		this->height = height;
	}
	uint32_t getHeight() const {
		return this->height;
	}
	bool operator==(const BitcoinBlockHeaderAttestation& other) const {
		return this->height == other.height;
	}

	void serialize_payload(Serialize *ctx) const override;
	static BitcoinBlockHeaderAttestation* deserialize(Deserialize *ctx);
};

inline std::ostream& operator<<(std::ostream& out, const PendingAttestation &attestation) {
	out <<  "PendingAttestation(";
	out.write((char*) attestation.getUri(), attestation.getUriLen());
	out << ")";
	return out;
}

inline std::ostream& operator<<(std::ostream& out, const BitcoinBlockHeaderAttestation &attestation) {
	out <<  "BitcoinBlockHeaderAttestation("  << attestation.getHeight() <<  + ")";
	return out;
}

} // namespace ots

#endif //C_OPENTIMESTAMPS_ATTESTATION_H
