
#ifndef C_OPENTIMESTAMPS_TIMESTAMP_H
#define C_OPENTIMESTAMPS_TIMESTAMP_H


#include <cstdint>
#include <map>
#include <list>
#include "Op.h"
#include "Attestation.h"

class Timestamp {
public:
	uint8_t *msg;
	uint32_t len;
	std::map<Op, Timestamp*> ops;
	std::list<TimeAttestation> attestations;

	Timestamp(uint8_t *msg, uint32_t len){
		this->len = len;
		this->msg = (uint8_t*) malloc(len);
		memcpy(this->msg, msg, len);
	}

	uint8_t* getDigest(){
		return this->msg;
	}
	uint32_t getDigestLenght(){
		return this->len;
	}

	static Timestamp deserialize(uint8_t *ots, uint8_t *initialMsg, uint32_t len);
	void serialize(uint8_t* output);
	void merge(Timestamp other);
	char* strTree();
	bool isTimestampComplete();
	bool equals(Timestamp timestamp);
	Timestamp* add(Op op);

};

inline std::ostream& operator<<(std::ostream& out, Timestamp* timestamp) {
	out << "\n";
	return out;
}

#endif //C_OPENTIMESTAMPS_TIMESTAMP_H
