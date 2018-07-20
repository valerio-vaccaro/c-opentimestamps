//
// Created by luca on 20/06/2018.
//

#ifndef C_OPENTIMESTAMPS_SERIALIZE_H
#define C_OPENTIMESTAMPS_SERIALIZE_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include "Common.h"

namespace ots{

struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
};

class Deserialize
{
private:
public:
	std::istream* stream;
	Deserialize(std::istream* stream) : stream(stream) {}
	~Deserialize() {}

	unsigned char read8(){
		unsigned char obj;
		stream->read((char*)&obj, 1);
		return obj;
	}
	void read(unsigned char* buffer, size_t len){
		//stream->read((char*)&buffer, len);
		std::size_t n = 0;
		while( len > 0 && stream->good() ) {
			stream->read(reinterpret_cast<char *>(&buffer[n]), len );
			int i = stream->gcount();
			n += i;
			len -= i;
		}
	}

	size_t readVaruint() {
		size_t value = 0;
		unsigned char shift = 0;
		unsigned char b;
		do {
			b = read8();
			value |= (b & 0b01111111) << shift;
			shift += 7;
		} while ((b & 0b10000000) != 0b00000000);

		return value;
	}

	unsigned char readVaruints(unsigned char* buffer, const size_t len){
		size_t bufferLen = readVaruint();
		if ((bufferLen & 0xff) > len) {
			return 0;
		}
		read(buffer, bufferLen);
		return bufferLen;
	}

	bool assertMagic(const unsigned char* expectedMagic, const size_t len) {
		unsigned char buffer[len];
		read(buffer, len);
		if (compare(expectedMagic, len, buffer, len)) {
			return true;
		} else {
			return false;
		}
	}

	bool assertEof(){
		return stream->eof();
	}
};


class Serialize {
private:
public:
	std::vector<unsigned char> data;
	int len;
	Serialize() : len(0) {}
	~Serialize() {}

	void write(const unsigned char *buffer, const size_t len) {
		this->len+=len;
		std::copy (buffer, buffer + len, std::back_inserter(this->data));
	}

	void write8(const unsigned char obj) {
		this->len++;
		data.push_back(obj);
	}

	void write32(const uint32_t obj) {
		unsigned char d[4];
		for (int i=0; i<4 ;++i)
			d[i] = ((unsigned char*)&obj)[3-i];
		write(d, 4);
		this->len++;
	}

	void writeVaruint(const size_t value) {
		size_t val = value;
		if ((val) == 0b00000000) {
			write8(0x00);
		} else {
			while (val != 0) {
				unsigned char b = (unsigned char) ((val&0xff) & 0b01111111);
				if ((val) > 0b01111111) {
					b |= 0b10000000;
				}
				write8(b);
				if ((val) <= 0b01111111) {
					break;
				}
				val = val >> 7;
			}
		}
	}

	void writeVaruints(const unsigned char *buffer, const size_t len) {
		writeVaruint(len);
		write(buffer, len);
	}

};

inline std::ostream& operator<<(std::ostream& out, Serialize* serialize) {
	for(int i=0;i<serialize->len;i++) {
		out << ots::byteToHex(serialize->data[i]);
	}
	return out;
}

} // namespace ots

#endif //C_OPENTIMESTAMPS_SERIALIZE_H
