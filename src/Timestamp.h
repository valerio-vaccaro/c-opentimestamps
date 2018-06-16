//
// Created by luca on 16/06/2018.
//

#ifndef C_OPENTIMESTAMPS_TIMESTAMP_H
#define C_OPENTIMESTAMPS_TIMESTAMP_H


#include <cstdint>
#include <map>
#include <list>
#include "Op.h"
#include "Attestation.h"

class Timestamp {
protected:
	uint8_t *msg;
	std::map<Op, Timestamp> ops;
	std::list<TimeAttestation> attestations;
public:
	Timestamp(uint8_t *msg){
		memcpy(this->msg, msg, sizeof(msg));
	}
	uint8_t* getDigest(){
		return this->msg;
	}

	static Timestamp deserialize(uint8_t *ots, uint8_t *initialMsg);
	void serialize(uint8_t* output);
	void merge(Timestamp other);
	char* strTree();
	bool isTimestampComplete();
	bool equals(Timestamp timestamp);
	Timestamp* add(Op op);
};


#endif //C_OPENTIMESTAMPS_TIMESTAMP_H
