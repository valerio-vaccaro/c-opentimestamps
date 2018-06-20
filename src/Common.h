
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

bool compare(const uint8_t *a, const uint32_t a_len, const uint8_t *b, const uint32_t b_len){
	if (a_len != b_len){
		return false;
	}
	for (int i = 0;i < a_len;i++){
		if (a[i]!=b[i])
			return false;
	}
	return true;
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
