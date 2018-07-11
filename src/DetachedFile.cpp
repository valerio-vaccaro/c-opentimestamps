#include "DetachedFile.h"

const uint8_t DetachedFile::HEADER_MAGIC[31] = {(uint8_t) 0x00, (uint8_t) 0x4f, (uint8_t) 0x70, (uint8_t) 0x65, (uint8_t) 0x6e,
								  (uint8_t) 0x54, (uint8_t) 0x69, (uint8_t) 0x6d, (uint8_t) 0x65, (uint8_t) 0x73,
								  (uint8_t) 0x74, (uint8_t) 0x61, (uint8_t) 0x6d, (uint8_t) 0x70, (uint8_t) 0x73, (uint8_t) 0x00, (uint8_t) 0x00,
								  (uint8_t) 0x50, (uint8_t) 0x72, (uint8_t) 0x6f, (uint8_t) 0x6f, (uint8_t) 0x66, (uint8_t) 0x00,
								  (uint8_t) 0xbf, (uint8_t) 0x89, (uint8_t) 0xe2, (uint8_t) 0xe8, (uint8_t) 0x84, (uint8_t) 0xe8, (uint8_t) 0x92,
								  (uint8_t) 0x94};
const uint8_t DetachedFile::MAJOR_VERSION = 1;

void DetachedFile::serialize(Serialize *ctx){
	ctx->write(HEADER_MAGIC, sizeof(HEADER_MAGIC));
	ctx->writeVaruint(MAJOR_VERSION);
	this->fileHashOp->serialize(ctx);
	ctx->write(this->timestamp->msg,this->timestamp->len);
	this->timestamp->serialize(ctx);
}

DetachedFile* DetachedFile::deserialize(Deserialize *ctx) {
	ctx->assertMagic(HEADER_MAGIC, sizeof(HEADER_MAGIC));
	uint8_t major = ctx->readVaruint();
	if (major != MAJOR_VERSION) {
		return nullptr;
	}
	OpCrypto *fileHashOp = (OpCrypto *) OpCrypto::deserialize(ctx);
	uint8_t *fileHash = new uint8_t [fileHashOp->length()];
	ctx->read(fileHash, fileHashOp->length());
	Timestamp *timestamp = Timestamp::deserialize(ctx, fileHash, fileHashOp->length());
	ctx->assertEof();
	return new DetachedFile(fileHashOp, timestamp);
}