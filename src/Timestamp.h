
#ifndef C_OPENTIMESTAMPS_TIMESTAMP_H
#define C_OPENTIMESTAMPS_TIMESTAMP_H


#include <cstdint>
#include <map>
#include <list>
#include "Op.h"
#include "Attestation.h"

class Timestamp {
public:
	const uint8_t *msg;
	const uint32_t len;
	std::map<Op*, Timestamp*, less_op> ops;
	std::list<TimeAttestation> attestations;

	Timestamp(const uint8_t *msg, uint32_t len):
	msg(msg),
	len(len){
	}

	const uint8_t* getDigest() const{
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

	void serialize(Serialize ctx) {

		// TODO: sort attestations
		if (!this->attestations.empty()) {
			for (const TimeAttestation &attestation: this->attestations){
				ctx.write8(0xff);
				ctx.write8(0x00);
				if (this->attestations.back() == attestation){
					attestation.serialize(ctx);
				}
			}
		}

		if (this->ops.empty()) {
			ctx.write8(0x00);
			if(!this->attestations.empty()) {
				this->attestations.back().serialize(ctx);
			}
		} else if (this->ops.size() > 0) {
			if (!this->attestations.empty()) {
				ctx.write8(0xff);
				ctx.write8(0x00);
				this->attestations.back().serialize(ctx);
			}
			// TODO: sort ops
			int counter = 0;
			for (const auto &entry: this->ops) {
				counter++;
				if (counter < this->ops.size()) {
					ctx.write8(0xff);
				}
				entry.first->serialize(ctx);
				entry.second->serialize(ctx);
			}
		}
	}

};

inline std::ostream& operator<<(std::ostream& out, const Timestamp &timestamp) {

	if (!timestamp.attestations.empty()) {
		for (const auto &attestation: timestamp.attestations){
			//out << attestation;
		}
	}

	for (const auto &entry: timestamp.ops){

		if(timestamp.ops.size()>1){
			out <<  " -> ";
		}
		out << entry.first->tagName();
		out << entry.second;
	}
	return out;
}

#endif //C_OPENTIMESTAMPS_TIMESTAMP_H
