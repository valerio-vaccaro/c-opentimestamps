
#ifndef C_OPENTIMESTAMPS_ATTESTATION_H
#define C_OPENTIMESTAMPS_ATTESTATION_H


#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>

class TimeAttestation {
protected:
	static const int TAG_SIZE = 8;
	static const int  MAX_PAYLOAD_SIZE = 8192;
	const uint8_t *TAG;
public:

	TimeAttestation(const uint8_t *tag) : TAG(tag){}
	virtual bool operator==(const TimeAttestation& other) const;
	virtual void serialize_payload(Serialize ctx) const;
	void serialize(Serialize ctx) const {
		ctx.write(this->TAG,this->TAG_SIZE);

		std::ostringstream buf;
		Serialize payload_ctx(&buf);
		this->serialize_payload(payload_ctx);
		ctx.getStream()->operator<<(payload_ctx.getStream());
	}
};

class PendingAttestation : TimeAttestation{
private:
	uint8_t *uri;
	uint8_t len;
public:
	const uint8_t TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x83, (uint8_t) 0xdf, (uint8_t) 0xe3, (uint8_t) 0x0d, (uint8_t) 0x2e, (uint8_t) 0xf9, (uint8_t) 0x0c, (uint8_t) 0x8e};

	PendingAttestation(uint8_t* msg, uint8_t len): TimeAttestation(TAG){
		this->uri = (uint8_t*)malloc(len);
		this->len = len;
	}
	uint8_t * getUri() {
		return this->uri;
	}
	static bool checkUri(uint8_t* uri);

	bool operator==(const PendingAttestation& other) const {
		return strncmp((char*)this->uri,(char*)other.uri,this->len);
	}
};


class BitcoinBlockHeaderAttestation : TimeAttestation{
private:
	uint32_t height;
public:
	const uint8_t TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x05, (uint8_t) 0x88, (uint8_t) 0x96, (uint8_t) 0x0d, (uint8_t) 0x73, (uint8_t) 0xd7, (uint8_t) 0x19, (uint8_t) 0x01};

	BitcoinBlockHeaderAttestation(uint32_t height): TimeAttestation(TAG){
		this->height = height;
	}
	uint32_t getHeight() {
		return this->height;
	}
	bool operator==(const BitcoinBlockHeaderAttestation& other) const {
		return this->height == other.height;
	}
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
