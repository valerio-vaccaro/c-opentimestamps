
#include <iostream>
#include "src/Op.h"
#include "src/Timestamp.h"
#include "src/DetachedFile.h"
#include "src/Common.h"

int main() {

	// OTS EXAMPLE
	const std::string ots("004f70656e54696d657374616d7073000050726f6f6600bf89e2e884e89294010805c4f616a8e5310d19d938cfd769864d7f4ccdc2ca8b479b10af83564b097af9f010e754bf93806a7ebaa680ef7bd0114bf408f010b573e8850cfd9e63d1f043fbb6fc250e08f10457cfa5c4f0086fb1ac8d4e4eb0e70083dfe30d2ef90c8e2e2d68747470733a2f2f616c6963652e6274632e63616c656e6461722e6f70656e74696d657374616d70732e6f7267");
	std::cout << ots << std::endl;

	// DESERIALIZE
	unsigned char otsBytes [ots.length()/2];
	ots::toBytes(ots, otsBytes);

	const size_t len = ots.length()/2;
	std::vector<unsigned char> buffer(otsBytes,otsBytes+len);
	ots::Deserialize ctx(buffer);
	std::unique_ptr <ots::DetachedFile> uniqueDetachedFile (ots::DetachedFile::deserialize(&ctx));
	std::cout << *uniqueDetachedFile << std::endl;

	// SERIALIZE
	ots::Serialize serialize;
	uniqueDetachedFile->serialize(&serialize);
	std::cout << serialize << std::endl;
	return 0;
}

