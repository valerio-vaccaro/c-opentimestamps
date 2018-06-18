
#include <iostream>
#include "src/Op.h"
#include "src/Timestamp.h"
#include "src/DetachedFile.h"
using namespace std;

int main() {

	// EXAMPLE
	uint8_t text[] = "Hello, World!";
	cout << text << "\n";

	OpSha256 op;
	cout << op << "\n";

	Timestamp timestamp (text, sizeof(text));
	//cout << timestamp << "\n";

	uint8_t msg[] = "Hello, you!";
	Timestamp subStamp (msg, sizeof(msg));
	timestamp.ops.insert(std::make_pair(&op,&subStamp));
	cout << timestamp << "\n";

	DetachedFile detached (&op, text, sizeof(text));
	cout << detached << "\n";

	return 0;
}
