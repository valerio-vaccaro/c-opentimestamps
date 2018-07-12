#include "Attestation.h"
#include "Common.h"

namespace ots{

const uint8_t PendingAttestation::TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x83, (uint8_t) 0xdf, (uint8_t) 0xe3, (uint8_t) 0x0d, (uint8_t) 0x2e, (uint8_t) 0xf9, (uint8_t) 0x0c, (uint8_t) 0x8e};
const uint8_t BitcoinBlockHeaderAttestation::TAG[TimeAttestation::TAG_SIZE] = {(uint8_t) 0x05, (uint8_t) 0x88, (uint8_t) 0x96, (uint8_t) 0x0d, (uint8_t) 0x73, (uint8_t) 0xd7, (uint8_t) 0x19, (uint8_t) 0x01};


void TimeAttestation::serialize(Serialize *ctx) const {
	std::ostringstream buf;
	Serialize payload_ctx(&buf);
	payload_ctx.stream = &buf;
	this->serialize_payload(&payload_ctx);

	uint8_t* payload = (uint8_t*)buf.str().data();
	uint8_t *buffer = new uint8_t [payload_ctx.len];
	for (int i = 0;i<payload_ctx.len;i++){
		buffer[i]=payload[i];
	}

	if (const PendingAttestation* pending = dynamic_cast<const PendingAttestation *>(this)) {
		ctx->write(pending->TAG,pending->TAG_SIZE);
	} else if (const BitcoinBlockHeaderAttestation* bitcoin = dynamic_cast<const BitcoinBlockHeaderAttestation *>(this)) {
		ctx->write(bitcoin->TAG,bitcoin->TAG_SIZE);
	}
	ctx->writeVaruint(payload_ctx.len);
	ctx->write(buffer, payload_ctx.len);
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
	uint8_t tag[TAG_SIZE];
	ctx->read(tag, TAG_SIZE);
	uint8_t serializedAttestation[MAX_PAYLOAD_SIZE];
	uint32_t len = ctx->readVaruints(serializedAttestation, MAX_PAYLOAD_SIZE);

	std::istringstream in((char*)serializedAttestation,len);
	Deserialize payloadCtx(&in);
	if (compare(tag,TAG_SIZE,PendingAttestation::TAG,TAG_SIZE)){
		attestation = PendingAttestation::deserialize(&payloadCtx);
	} else if (compare(tag,TAG_SIZE,BitcoinBlockHeaderAttestation::TAG,TAG_SIZE)) {
		attestation = BitcoinBlockHeaderAttestation::deserialize(&payloadCtx);
	}
	payloadCtx.assertEof();
	return attestation;
}

PendingAttestation* PendingAttestation::deserialize(Deserialize *ctx){
	uint8_t buffer[MAX_URI_LENGTH];
	uint32_t len = ctx->readVaruints(buffer, MAX_URI_LENGTH);
	// check uri
	return new PendingAttestation(buffer, len);
}

BitcoinBlockHeaderAttestation* BitcoinBlockHeaderAttestation::deserialize(Deserialize *ctx){
	uint32_t height = ctx->readVaruint();
	return new BitcoinBlockHeaderAttestation(height);
}

} // namespace ots
