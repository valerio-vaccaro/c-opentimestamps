
#include "Op.h"

namespace ots{

	uint8_t OpAppend::TAG=0xf0;
	uint8_t OpPrepend::TAG=0xf1;
	uint8_t OpSha1::TAG=0x01;
	uint8_t OpSha256::TAG=0x08;
	uint8_t OpRipemd160::TAG=0x03;

	const int32_t MAX_MESSAGE_LENGHT = 4096;
	const int32_t Op::MAX_RESULT_LENGTH = 4096;

Op* Op::deserialize(Deserialize *ctx){
	uint8_t tag = ctx->read8();
	return Op::deserializeFromTag(ctx, tag);
}

Op* Op::deserializeFromTag(Deserialize *ctx, uint8_t tag){
	if (tag == OpSha256::TAG) {
		return new OpSha256();
	} else if (tag == OpSha1::TAG) {
		return new OpSha1();
	} else if (tag == OpRipemd160::TAG) {
		return new OpRipemd160();
	} else if (tag == OpAppend::TAG) {
		uint8_t arg[Op::MAX_RESULT_LENGTH];
		uint32_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpAppend(arg,len);
	} else if (tag == OpPrepend::TAG) {
		uint8_t arg[Op::MAX_RESULT_LENGTH];
		uint32_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpPrepend(arg,len);
	}
	return nullptr;
}

} // namespace ots
