
#include "Op.h"

namespace ots{

	unsigned char OpAppend::TAG=0xf0;
	unsigned char OpPrepend::TAG=0xf1;
	unsigned char OpSha1::TAG=0x01;
	unsigned char OpSha256::TAG=0x08;
	unsigned char OpRipemd160::TAG=0x03;

	const size_t MAX_MESSAGE_LENGHT = 4096;
	const size_t Op::MAX_RESULT_LENGTH = 4096;

Op* Op::deserialize(Deserialize *ctx){
	unsigned char tag = ctx->read8();
	return Op::deserializeFromTag(ctx, tag);
}

Op* Op::deserializeFromTag(Deserialize *ctx, unsigned char tag){
	if (tag == OpSha256::TAG) {
		return new OpSha256();
	} else if (tag == OpSha1::TAG) {
		return new OpSha1();
	} else if (tag == OpRipemd160::TAG) {
		return new OpRipemd160();
	} else if (tag == OpAppend::TAG) {
		unsigned char arg[Op::MAX_RESULT_LENGTH];
		size_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpAppend(arg,len);
	} else if (tag == OpPrepend::TAG) {
		unsigned char arg[Op::MAX_RESULT_LENGTH];
		size_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpPrepend(arg,len);
	}
	return nullptr;
}

} // namespace ots
