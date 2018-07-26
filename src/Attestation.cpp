#include "Attestation.h"
#include "Common.h"

namespace ots{

const unsigned char PendingAttestation::TAG[TimeAttestation::TAG_SIZE] = {(unsigned char) 0x83, (unsigned char) 0xdf, (unsigned char) 0xe3, (unsigned char) 0x0d, (unsigned char) 0x2e, (unsigned char) 0xf9, (unsigned char) 0x0c, (unsigned char) 0x8e};
const unsigned char BitcoinBlockHeaderAttestation::TAG[TimeAttestation::TAG_SIZE] = {(unsigned char) 0x05, (unsigned char) 0x88, (unsigned char) 0x96, (unsigned char) 0x0d, (unsigned char) 0x73, (unsigned char) 0xd7, (unsigned char) 0x19, (unsigned char) 0x01};


void TimeAttestation::serialize(Serialize *ctx) const {
	Serialize payload_ctx;
	this->serialize_payload(&payload_ctx);

	unsigned char* payload = (unsigned char* )payload_ctx.data.data();

	if (const PendingAttestation* pending = dynamic_cast<const PendingAttestation *>(this)) {
		ctx->write(pending->TAG,pending->TAG_SIZE);
	} else if (const BitcoinBlockHeaderAttestation* bitcoin = dynamic_cast<const BitcoinBlockHeaderAttestation *>(this)) {
		ctx->write(bitcoin->TAG,bitcoin->TAG_SIZE);
	}
	ctx->writeVaruint(payload_ctx.len);
	ctx->write(payload, payload_ctx.len);
}
bool TimeAttestation::operator==(TimeAttestation& other){
	if (PendingAttestation* pending = dynamic_cast<PendingAttestation *>(&other)) {
		return pending == this;
	} else if (BitcoinBlockHeaderAttestation* bitcoin = dynamic_cast<BitcoinBlockHeaderAttestation *>(&other)) {
		return bitcoin == this;
	}
	return false;
}

void PendingAttestation::serialize_payload(Serialize *ctx) const {
	ctx->writeVaruints(uri, len);
}
void BitcoinBlockHeaderAttestation::serialize_payload(Serialize *ctx) const {
	ctx->writeVaruint(this->height);
}

TimeAttestation* TimeAttestation::deserialize(Deserialize *ctx){
	TimeAttestation *attestation;
	unsigned char tag[TAG_SIZE];
	ctx->read(tag, TAG_SIZE);
	unsigned char serializedAttestation[MAX_PAYLOAD_SIZE];
	size_t len = ctx->readVaruints(serializedAttestation, MAX_PAYLOAD_SIZE);

	std::vector<unsigned char> vector(serializedAttestation,serializedAttestation+len);
	Deserialize payloadCtx(vector);
	if (compare(tag,TAG_SIZE,PendingAttestation::TAG,TAG_SIZE)){
		attestation = PendingAttestation::deserialize(&payloadCtx);
	} else if (compare(tag,TAG_SIZE,BitcoinBlockHeaderAttestation::TAG,TAG_SIZE)) {
		attestation = BitcoinBlockHeaderAttestation::deserialize(&payloadCtx);
	}
	payloadCtx.assertEof();
	return attestation;
}

PendingAttestation* PendingAttestation::deserialize(Deserialize *ctx){
	unsigned char buffer[MAX_URI_LENGTH];
	size_t len = ctx->readVaruints(buffer, MAX_URI_LENGTH);
	// check uri
	return new PendingAttestation(buffer, len);
}

BitcoinBlockHeaderAttestation* BitcoinBlockHeaderAttestation::deserialize(Deserialize *ctx){
	uint32_t height = ctx->readVaruint();
	return new BitcoinBlockHeaderAttestation(height);
}

} // namespace ots
