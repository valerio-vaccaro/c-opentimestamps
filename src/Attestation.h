
#ifndef C_OPENTIMESTAMPS_ATTESTATION_H
#define C_OPENTIMESTAMPS_ATTESTATION_H


#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Context.h"

class TimeAttestation {
protected:
	static const int TAG_SIZE = 8;
	static const int  MAX_PAYLOAD_SIZE = 8192;
	const uint8_t *TAG;
public:
	TimeAttestation(const uint8_t *tag) : TAG(tag){}
	virtual bool operator==(TimeAttestation& other);
	virtual void serialize_payload(Serialize *ctx) const = 0;
	void serialize(Serialize *ctx) const;
	static TimeAttestation* deserialize(Deserialize *ctx);
};

class PendingAttestation : public TimeAttestation{
private:
	uint8_t *uri;
	uint8_t len;
	static const uint32_t MAX_URI_LENGTH = 1000;
public:
	static const uint8_t TAG[TimeAttestation::TAG_SIZE];
	PendingAttestation(uint8_t* msg, uint32_t len): TimeAttestation(TAG){
		this->len = len;
		this->uri = (uint8_t*) malloc(len);
		std::copy(msg, msg + len, this->uri);
	}
	uint8_t * getUri() {
		return this->uri;
	}
	static bool checkUri(uint8_t* uri, uint32_t len){
		// TODO
		return true;
	}

	bool operator==(TimeAttestation& other) override {
		PendingAttestation *ptr = dynamic_cast<PendingAttestation *>(&other);
		return strncmp((char*)this->uri,(char*)ptr->uri,this->len);
	}
	void serialize_payload(Serialize *ctx) const override;
	static PendingAttestation* deserialize(Deserialize *ctx);
};


class BitcoinBlockHeaderAttestation : public TimeAttestation{
private:
	uint32_t height;
public:
	static const uint8_t TAG[TimeAttestation::TAG_SIZE];

	BitcoinBlockHeaderAttestation(uint32_t height): TimeAttestation(TAG){
		this->height = height;
	}
	uint32_t getHeight() {
		return this->height;
	}
	bool operator==(const BitcoinBlockHeaderAttestation& other) const {
		return this->height == other.height;
	}

	void serialize_payload(Serialize *ctx) const override;
	static BitcoinBlockHeaderAttestation* deserialize(Deserialize *ctx);
};

inline std::ostream& operator<<(std::ostream& out, PendingAttestation* attestation) {
	out <<  "PendingAttestation("  << attestation->getUri() <<  + ")";
	return out;
}

inline std::ostream& operator<<(std::ostream& out, BitcoinBlockHeaderAttestation* attestation) {
	out <<  "BitcoinBlockHeaderAttestation("  << attestation->getHeight() <<  + ")";
	return out;
}

#endif //C_OPENTIMESTAMPS_ATTESTATION_H
