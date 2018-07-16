
#ifndef C_OPENTIMESTAMPS_COMMON_H
#define C_OPENTIMESTAMPS_COMMON_H

#include <sstream>
#include <iomanip>

namespace ots{

extern std::string toHex(const unsigned char *data, const size_t len);
extern std::string toHex(const unsigned char data);
extern char* toBytes(const std::string& hex);
extern bool compare(const unsigned char *a, const size_t a_len, const unsigned char *b, const size_t b_len);

} // namespace ots

#endif //C_OPENTIMESTAMPS_COMMON_H
