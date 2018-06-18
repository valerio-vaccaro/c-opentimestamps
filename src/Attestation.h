
#ifndef C_OPENTIMESTAMPS_ATTESTATION_H
#define C_OPENTIMESTAMPS_ATTESTATION_H


#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>

class TimeAttestation {
public:
	static const int TAG_SIZE = 8;
	static const int  MAX_PAYLOAD_SIZE = 8192;

	TimeAttestation(){}
};

class PendingAttestation : TimeAttestation{
private:
	uint8_t *uri;
public:
	const uint8_t TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x83, (uint8_t) 0xdf, (uint8_t) 0xe3, (uint8_t) 0x0d, (uint8_t) 0x2e, (uint8_t) 0xf9, (uint8_t) 0x0c, (uint8_t) 0x8e};

	PendingAttestation(uint8_t* msg){
		this->uri = (uint8_t*)malloc(sizeof(msg));
	}
	uint8_t * getUri() {
		return this->uri;
	}
	static bool checkUri(uint8_t* uri);
};


class BitcoinBlockHeaderAttestation : TimeAttestation{
private:
	uint32_t height;
public:
	const uint8_t TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x05, (uint8_t) 0x88, (uint8_t) 0x96, (uint8_t) 0x0d, (uint8_t) 0x73, (uint8_t) 0xd7, (uint8_t) 0x19, (uint8_t) 0x01};

	BitcoinBlockHeaderAttestation(uint32_t height){
		this->height = height;
	}
	uint32_t getHeight() {
		return this->height;
	}
	bool operator==(const BitcoinBlockHeaderAttestation& other) const;
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
