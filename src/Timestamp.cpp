
#include "Timestamp.h"

void Timestamp::do_tag_or_attestation(Timestamp *timestamp, Deserialize ctx, uint8_t tag){
	if (tag == 0x00){
		TimeAttestation *attestation = TimeAttestation::deserialize(ctx);
		timestamp->attestations.push_back(attestation);
	} else {
		Op *op = Op::deserializeFromTag(ctx, tag);

		uint8_t result[Op::MAX_RESULT_LENGTH];
		uint8_t resultLen = op->call(timestamp->msg, timestamp->len, result);

		Timestamp *stamp = Timestamp::deserialize(ctx, result, resultLen);
		timestamp->ops.insert(std::pair <Op*, Timestamp*> (op, stamp));
	}
}

Timestamp* Timestamp::deserialize(Deserialize ctx, uint8_t *initialMsg, uint32_t len ){
	Timestamp *timestamp = new Timestamp(initialMsg, len);
	uint8_t tag = ctx.read8();
	while ( tag == 0xff ) {
		do_tag_or_attestation(timestamp, ctx, ctx.read8());
		tag = ctx.read8();
	}
	do_tag_or_attestation(timestamp, ctx, tag);
	return timestamp;
}

void Timestamp::serialize(Serialize ctx) {

	// TODO: sort attestations
	if (!this->attestations.empty()) {
		for (TimeAttestation *attestation: this->attestations){
			ctx.write8(0xff);
			ctx.write8(0x00);
			if (*this->attestations.back() == *attestation){
				attestation->serialize(ctx);
			}
		}
	}

	if (this->ops.empty()) {
		ctx.write8(0x00);
		if(!this->attestations.empty()) {
			this->attestations.back()->serialize(ctx);
		}
	} else if (this->ops.size() > 0) {
		if (!this->attestations.empty()) {
			ctx.write8(0xff);
			ctx.write8(0x00);
			this->attestations.back()->serialize(ctx);
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