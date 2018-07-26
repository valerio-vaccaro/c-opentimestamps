
#ifndef C_OPENTIMESTAMPS_COMMON_H
#define C_OPENTIMESTAMPS_COMMON_H

#include <sstream>
#include <iomanip>
#include <assert.h>

namespace ots{
extern std::string toHex(const unsigned char *data, const size_t len);
extern std::string byteToHex(const unsigned char data);
extern void toBytes(const std::string& hex, unsigned char *bytes);
extern bool compare(const unsigned char *a, const size_t a_len, const unsigned char *b, const size_t b_len);

} // namespace ots

#endif //C_OPENTIMESTAMPS_COMMON_H
