#include <iostream>
#include "src/Op.h"
using namespace std;

int main() {

	// EXAMPLE
	uint8_t text[] = "Hello, World!";
	OpSha256 *opSha256 = new OpSha256();
	uint8_t hash[SHA256_DIGEST_LENGTH];
	opSha256->call(text,hash);

	uint8_t output[sizeof(hash)+sizeof(text)];
	OpAppend *opAppend = new OpAppend(hash);
	opAppend->call(text,output);

	int i = 0;
	for(i = 0; i < sizeof(output); i++)
	{
		printf("%02x", output[i]);
	}

	return 0;
}