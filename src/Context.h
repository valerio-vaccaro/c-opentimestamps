//
// Created by luca on 20/06/2018.
//

#ifndef C_OPENTIMESTAMPS_SERIALIZE_H
#define C_OPENTIMESTAMPS_SERIALIZE_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include "Common.h"

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

	uint8_t read8(){
		uint8_t obj;
		stream->read((char*)&obj, 1);
		return obj;
	}
	uint32_t read32(){
		uint32_t obj;
		stream->read((char*)&obj, 1);
		return obj;
	}
	void read(uint8_t* buffer, size_t len){
		//stream->read((char*)&buffer, len);
		std::size_t n = 0;
		while( len > 0 && stream->good() ) {
			stream->read(reinterpret_cast<char *>(&buffer[n]), len );
			int i = stream->gcount();
			n += i;
			len -= i;
		}
	}

	uint32_t readVaruint() {
		uint32_t value = 0;
		uint8_t shift = 0;
		uint8_t b;
		do {
			b = read8();
			value |= (b & 0b01111111) << shift;
			shift += 7;
		} while ((b & 0b10000000) != 0b00000000);

		return value;
	}

	uint8_t readVaruints(uint8_t* buffer, const size_t len){
		uint32_t bufferLen = readVaruint();
		if ((bufferLen & 0xff) > len) {
			return 0;
		}
		read(buffer, bufferLen);
		return bufferLen;
	}

	bool assertMagic(const uint8_t* expectedMagic, const uint32_t len) {
		uint8_t buffer[len];
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
	std::ostringstream *stream;
	int len;
	Serialize(std::ostream *ostringstream) : stream(stream), len(0) {}
	~Serialize() {}

	std::ostream* getStream(){
		return stream;
	}

	void write(const uint8_t *buffer, const size_t len) {
		stream->write((char *) buffer, len);
		this->len+=len;
	}

	void write8(const uint8_t obj) {
		stream->write((char *) &obj, 1);
		this->len++;
	}

	void write32(const uint32_t obj) {
		uint8_t d[4] = {0};
		for (int i=0; i<4 ;++i)
			d[i] = ((uint8_t*)&obj)[3-i];
		stream->write((char *) d, 4);
		this->len++;
	}

	void writeVaruint(const uint32_t value) {
		uint32_t val = value;
		if ((val) == 0b00000000) {
			write8(0x00);
		} else {
			while (val != 0) {
				uint8_t b = (uint8_t) ((val&0xff) & 0b01111111);
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

	void writeVaruints(const uint8_t *buffer, const size_t len) {
		writeVaruint(len);
		write(buffer, len);
	}

};

inline std::ostream& operator<<(std::ostream& out, Serialize* serialize) {
	return out;
}

#endif //C_OPENTIMESTAMPS_SERIALIZE_H
