#ifndef __HELPER
#define __HELPER
#include <emp-tool/emp-tool.h>
#include "c2pc_config.h"
#include <immintrin.h>

using std::future;
using std::cout;
using std::endl;
using std::flush;
#undef align

namespace emp {

void send_bool_aligned(NetIO* io, const bool * data, int length) {
	uint64_t * data64 = (uint64_t * )data;
	int i = 0;
	for(; i < length/8; ++i) {
		uint64_t tmp = _pext_u64(data64[i], 0x0101010101010101ULL);
		io->send_data(&tmp, 1);
	}
	if (8*i != length)
		io->send_data(data + 8*i, length - 8*i);
}
void recv_bool_aligned(NetIO* io, bool * data, int length) {
	uint64_t * data64 = (uint64_t *) data;
	int i = 0;
	for(; i < length/8; ++i) {
		uint64_t tmp = 0;
		io->recv_data(&tmp, 1);
		data64[i] = _pdep_u64(tmp, 0x0101010101010101ULL);
	}
	if (8*i != length)
		io->recv_data(data + 8*i, length - 8*i);
}
void send_bool(NetIO * io, bool * data, int length) {
	void * ptr = (void *)data;
	size_t space = length;
	void * aligned = std::align(alignof(uint64_t), sizeof(uint64_t), ptr, space);
	int diff = length - space;
	io->send_data(data, diff);
	send_bool_aligned(io, (const bool*)aligned, length - diff);
}

void recv_bool(NetIO * io, bool * data, int length) {
	void * ptr = (void *)data;
	size_t space = length;
	void * aligned = std::align(alignof(uint64_t), sizeof(uint64_t), ptr, space);
	int diff = length - space;
	io->recv_data(data, diff);
	recv_bool_aligned(io, (bool*)aligned, length - diff);
}

template<int B>
void send_partial_block(NetIO * io, const block * data, int length) {
	for(int i = 0; i < length; ++i) {
		io->send_data(&(data[i]), B);
	}
}

template<int B>
void recv_partial_block(NetIO * io, block * data, int length) {
	for(int i = 0; i < length; ++i) {
		io->recv_data(&(data[i]), B);
	}
}
}
#endif// __HELPER