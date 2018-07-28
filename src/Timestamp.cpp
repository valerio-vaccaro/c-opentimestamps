
#include "Timestamp.h"

namespace ots{

void Timestamp::do_tag_or_attestation(Timestamp *timestamp, Deserialize *ctx, unsigned char tag){
	if (tag == 0x00){
		TimeAttestation *attestation = TimeAttestation::deserialize(ctx);
		timestamp->attestations.push_back(attestation);
	} else {
		Op *op = Op::deserializeFromTag(ctx, tag);

		unsigned char * result = new unsigned char [Op::MAX_RESULT_LENGTH];
		size_t resultLen = op->call(timestamp->msg, timestamp->len, result);

		Timestamp *stamp = Timestamp::deserialize(ctx, result, resultLen);
		timestamp->ops.insert(std::pair <Op*, Timestamp*> (op, stamp));
	}
}

Timestamp* Timestamp::deserialize(Deserialize *ctx, unsigned char *initialMsg, size_t len ){
	assert (nullptr != ctx);
	assert (nullptr != initialMsg);
	assert (len > 0);

	Timestamp *timestamp = new Timestamp(initialMsg, len);
	unsigned char tag = ctx->read8();
	while ( tag == 0xff ) {
		do_tag_or_attestation(timestamp, ctx, ctx->read8());
		tag = ctx->read8();
	}
	do_tag_or_attestation(timestamp, ctx, tag);
	return timestamp;
}

void Timestamp::serialize(Serialize *ctx) {

	// TODO: sort attestations
	if (this->attestations.size()>1) {
		for (TimeAttestation *attestation: this->attestations){
			ctx->write8(0xff);
			ctx->write8(0x00);
			if (*this->attestations.back() == *attestation){
				attestation->serialize(ctx);
			}
		}
	}

	if (this->ops.empty()) {
		ctx->write8(0x00);
		if(!this->attestations.empty()) {
			this->attestations.back()->serialize(ctx);
		}
	} else if (this->ops.size() > 0) {
		if (!this->attestations.empty()) {
			ctx->write8(0xff);
			ctx->write8(0x00);
			this->attestations.back()->serialize(ctx);
		}
		// TODO: sort ops
		int counter = 0;
		for (const auto &entry: this->ops) {
			counter++;
			if (counter < this->ops.size()) {
				ctx->write8(0xff);
			}
			entry.first->serialize(ctx);
			entry.second->serialize(ctx);
		}
	}
}


	Timestamp* Timestamp::add(Op *op) {
		// Add key
		// Returns the value associated with that key
		if (this->ops.count(op)>0) {
			return this->ops.at(op);
		}
		unsigned char *buffer = new uint8_t [op->length()];
		op->call(this->msg,this->len,buffer);
		Timestamp *stamp = new Timestamp(buffer,op->length());
		this->ops.insert(std::pair <Op*, Timestamp*> (op, stamp));
		return stamp;
	}

	void Timestamp::merge(Timestamp *other) {
		// Add all operations and attestations from another timestamp to this one
		if (!compare(this->msg, this->len, other->msg, other->len)) {
			// Can't merge timestamps for different messages together
			return;
		}
		for (TimeAttestation *attestation: other->attestations) {
			this->attestations.push_back(attestation);
		};
		for (const auto &entry: other->ops) {
			Op *other_op = entry.first;
			Timestamp *other_op_stamp = entry.second;

			Timestamp *our_op_stamp = this->add(other_op);
			our_op_stamp->merge(other_op_stamp);
		}
	}

} // namespace ots
