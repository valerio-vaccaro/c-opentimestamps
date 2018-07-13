
#ifndef C_OPENTIMESTAMPS_COMMON_H
#define C_OPENTIMESTAMPS_COMMON_H

#include <sstream>
#include <iomanip>

namespace ots{

extern std::string toHex(const uint8_t *data, const int len);
extern std::string toHex(const uint8_t data);
extern char* toBytes(const std::string& hex);
extern bool compare(const uint8_t *a, const uint32_t a_len, const uint8_t *b, const uint32_t b_len);

} // namespace ots

#endif //C_OPENTIMESTAMPS_COMMON_H
