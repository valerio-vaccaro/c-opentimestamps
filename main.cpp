
#include <iostream>
#include "src/Op.h"
#include "src/Timestamp.h"
#include "src/DetachedFile.h"

int main() {

	// EXAMPLE
	uint8_t text[] = "Hello, World!";
	std::cout << text << std::endl;

	OpSha256 op;
	std::cout << op << std::endl;

	Timestamp timestamp (text, sizeof(text));

	uint8_t msg[] = "Hello, you!";
	Timestamp subStamp (msg, sizeof(msg));
	timestamp.ops.insert(std::make_pair(&op,&subStamp));
	std::cout << timestamp << std::endl;

	DetachedFile detached (&op, text, sizeof(text));
	std::cout << detached << std::endl;

	return 0;
}
