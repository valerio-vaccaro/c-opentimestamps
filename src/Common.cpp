#include "Common.h"

namespace ots{

std::string toHex(const unsigned char *data, const size_t len);
std::string toHex(const unsigned char data);
char* toBytes(const std::string& hex);
bool compare(const unsigned char *a, const size_t a_len, const unsigned char *b, const size_t b_len);

std::string toHex(const unsigned char data)
{
	std::stringstream ss;
	ss<<std::hex << std::setw(2) << std::setfill('0') << (int)data;
	return ss.str();
}
std::string toHex(const unsigned char *data, const size_t len)
{
	std::stringstream ss;
	ss<<std::hex;
	for(int i(0);i<len;++i)
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss.str();
}

char* toBytes(const std::string& hex){
	//std::vector<char> bytes;
	size_t size = hex.size();
	char *bytes = new char [sizeof(unsigned char) * ((size/2)+1)];
	int j=0;
	for (unsigned int i = 0; i < hex.length(); i += 2){
		std::string byteString = hex.substr(i, 2);
		unsigned char b = (unsigned char) strtol(byteString.c_str(), NULL, 16);
		//bytes.push_back(byte);
		bytes[j] = b;
		j++;
	}
	bytes[j] = '\0';
	return bytes;
}

bool compare(const unsigned char *a, const size_t a_len, const unsigned char *b, const size_t b_len){
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

} // namespace ots
