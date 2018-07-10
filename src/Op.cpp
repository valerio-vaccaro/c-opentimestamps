
#include "Op.h"

Op* Op::deserialize(Deserialize *ctx){
	uint8_t tag = ctx->read8();
	return Op::deserializeFromTag(ctx, tag);
}

Op* Op::deserializeFromTag(Deserialize *ctx, uint8_t tag){
	if (tag == OpSha256().TAG) {
		return
				new OpSha256();
	} else if (tag == OpSha1().TAG) {
		return new OpSha1();
	} else if (tag == OpRipemd160().TAG) {
		return new OpRipemd160();
	} else if (tag == OpAppend({},0).TAG) {
		uint8_t arg[MAX_RESULT_LENGTH];
		uint32_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpAppend(arg,len);
	} else if (tag == OpPrepend({},0).TAG) {
		uint8_t arg[MAX_RESULT_LENGTH];
		uint32_t len = ctx->readVaruints(arg, Op::MAX_RESULT_LENGTH);
		return new OpPrepend(arg,len);
	}
	return nullptr;
}
