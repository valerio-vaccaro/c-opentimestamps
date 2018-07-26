#include "DetachedFile.h"

namespace ots{

const unsigned char DetachedFile::HEADER_MAGIC[31] = {(unsigned char) 0x00, (unsigned char) 0x4f, (unsigned char) 0x70, (unsigned char) 0x65, (unsigned char) 0x6e,
								  (unsigned char) 0x54, (unsigned char) 0x69, (unsigned char) 0x6d, (unsigned char) 0x65, (unsigned char) 0x73,
								  (unsigned char) 0x74, (unsigned char) 0x61, (unsigned char) 0x6d, (unsigned char) 0x70, (unsigned char) 0x73, (unsigned char) 0x00, (unsigned char) 0x00,
								  (unsigned char) 0x50, (unsigned char) 0x72, (unsigned char) 0x6f, (unsigned char) 0x6f, (unsigned char) 0x66, (unsigned char) 0x00,
								  (unsigned char) 0xbf, (unsigned char) 0x89, (unsigned char) 0xe2, (unsigned char) 0xe8, (unsigned char) 0x84, (unsigned char) 0xe8, (unsigned char) 0x92,
								  (unsigned char) 0x94};
const unsigned char DetachedFile::MAJOR_VERSION = 1;

void DetachedFile::serialize(Serialize *ctx){
	ctx->write(HEADER_MAGIC, sizeof(HEADER_MAGIC));
	ctx->writeVaruint(MAJOR_VERSION);
	this->fileHashOp->serialize(ctx);
	ctx->write(this->timestamp->msg,this->timestamp->len);
	this->timestamp->serialize(ctx);
}

DetachedFile* DetachedFile::deserialize(Deserialize *ctx) {
	ctx->assertMagic(HEADER_MAGIC, sizeof(HEADER_MAGIC));
	unsigned char major = ctx->readVaruint();
	if (major != MAJOR_VERSION) {
		return nullptr;
	}
	OpCrypto *fileHashOp = (OpCrypto *) OpCrypto::deserialize(ctx);
	unsigned char *fileHash = new unsigned char [fileHashOp->length()];
	ctx->read(fileHash, fileHashOp->length());
	Timestamp *timestamp = Timestamp::deserialize(ctx, fileHash, fileHashOp->length());
	ctx->assertEof();
	return new DetachedFile(fileHashOp, timestamp);
}

} // namespace ots
