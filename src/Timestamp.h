
#ifndef C_OPENTIMESTAMPS_TIMESTAMP_H
#define C_OPENTIMESTAMPS_TIMESTAMP_H


#include <cstdint>
#include <map>
#include <list>
#include "Op.h"
#include "Attestation.h"

namespace ots{

class Timestamp {
public:
	const uint8_t *msg;
	const uint32_t len;
	std::map<Op*, Timestamp*, less_op> ops;
	std::list<TimeAttestation*> attestations;

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
	void merge(Timestamp other);
	bool isTimestampComplete();
	bool equals(Timestamp timestamp);
	Timestamp* add(Op op);

	static void do_tag_or_attestation(Timestamp* timestamp, Deserialize *ctx, uint8_t tag);
	static Timestamp* deserialize(Deserialize *ctx, uint8_t *initialMsg, uint32_t len );
	void serialize(Serialize *ctx);
};

inline std::ostream& operator<<(std::ostream& out, const Timestamp &timestamp) {

	if (!timestamp.attestations.empty()) {
		for (const auto &attestation: timestamp.attestations){
			if (PendingAttestation* pending = dynamic_cast<PendingAttestation *>(attestation)) {
				out << pending;
			}else if (BitcoinBlockHeaderAttestation* bitcoin = dynamic_cast<BitcoinBlockHeaderAttestation *>(attestation)) {
				out << bitcoin;
			}
		}
	}

	for (const auto &entry: timestamp.ops){

		if(timestamp.ops.size()>1){
			out <<  " -> ";
		}

		if (OpUnary* unary = dynamic_cast<OpUnary *>(entry.first)) {
			out << *unary;
		}else if (OpBinary* binary = dynamic_cast<OpBinary *>(entry.first)) {
			out << *binary;
		}
		out << *entry.second;
	}
	return out;
}

} // namespace ots

#endif //C_OPENTIMESTAMPS_TIMESTAMP_H
