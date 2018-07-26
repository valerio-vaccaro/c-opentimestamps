
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
	const unsigned char *msg;
	const size_t len;
	std::map<Op*, Timestamp*, less_op> ops;
	std::list<TimeAttestation*> attestations;

	Timestamp(const unsigned char *msg, size_t len):
	msg(msg),
	len(len){
	}
	
	virtual ~Timestamp (){
		for_each (this->attestations.begin (), this->attestations.end (), [](TimeAttestation* el){
			assert (NULL != el);
			delete el;
		});
		for_each (this->ops.begin (), this->ops.end (), [](std::pair <Op*, Timestamp*> el){
			assert (NULL != el.first);
			assert (NULL != el.second);
			delete el.first;
			delete el.second;
		});

		assert (NULL != msg);
		delete [] msg;
	}

	const unsigned char* getDigest() const{
		assert (NULL != this->msg);
		return this->msg;
	}
	size_t getDigestLenght(){
		return this->len;
	}

	void merge(Timestamp *other);
	//TODO: bool isTimestampComplete();
	//TODO: bool equals(Timestamp timestamp);
	Timestamp* add(Op *op);

	static void do_tag_or_attestation(Timestamp* timestamp, Deserialize *ctx, unsigned char tag);
	static Timestamp* deserialize(Deserialize *ctx, unsigned char *initialMsg, size_t len );
	void serialize(Serialize *ctx);
};

inline std::ostream& operator<<(std::ostream& out, const Timestamp &timestamp) {

	if (!timestamp.attestations.empty()) {
		for (const auto &attestation: timestamp.attestations){
			if (PendingAttestation* pending = dynamic_cast<PendingAttestation *>(attestation)) {
				out << *pending;
			}else if (BitcoinBlockHeaderAttestation* bitcoin = dynamic_cast<BitcoinBlockHeaderAttestation *>(attestation)) {
				out << *bitcoin;
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
