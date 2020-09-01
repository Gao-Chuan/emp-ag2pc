#include <emp-tool/emp-tool.h>
#include "test/single_execution.h"
using namespace std;
using namespace emp;

int main(int argc, char** argv) {
	int party, port;
	parse_party_and_port(argv, &party, &port);
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port);
//	io->set_nodelay();

	string data;
	if (party == ALICE)
	{
		ifstream in("./flag1.txt");
		getline(in, data);
	} else if (party == BOB) {
		ifstream in("./flag2.txt");
		getline(in, data);
	}

	const char * flag = data.c_str();
	test(party, io, "/home/chuan/git/emp-ag2pc/bin/circuit.txt", "", flag); 
	delete io;
	return 0;
}
