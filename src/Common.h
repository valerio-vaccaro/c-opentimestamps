
#ifndef C_OPENTIMESTAMPS_COMMON_H
#define C_OPENTIMESTAMPS_COMMON_H

#include <sstream>
#include <iomanip>

std::string hexStr(const uint8_t *data, const int len)
{
	std::stringstream ss;
	ss<<std::hex;
	for(int i(0);i<len;++i)
		ss<<(int)data[i];
	return ss.str();
}

/*
FIX-ME anyway it's unused
int bytes(std::string &data, uint8_t* bytes){
	const char *pos = data.c_str();
	for (size_t count = 0; count < sizeof bytes/sizeof *bytes; count++) {
		sscanf(pos, "%2hhx", &bytes[count]);
		pos += 2;
	}
	return sizeof(bytes);
}
*/

#endif //C_OPENTIMESTAMPS_COMMON_H
