
#include <algorithm>
#include <iostream>
#include <random>

#include "Archive.h"
#include "SimpleCode.h"
#include "Time.h"

using TArch = TArchive<TSimpleCode>;
using TUnpack = TArch::TUnpack;
using TPack = TArch::TPack;

constexpr size_t kInputSize = 1000000;

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);
	TUnpack data(kInputSize);
	std::generate(data.begin(), data.end(), [&distrib, &gen] {return distrib(gen); });
	auto time=std::make_unique<TTime>();
	TPack pack = TArch::Pack(data);
	TUnpack unpack = TArch::Unpack(pack);
	time.reset();
	std::cout << data.size() << std::endl;
	std::cout << pack.size() << std::endl;
	std::cout << unpack.size() << std::endl;
	std::cout << std::boolalpha << (data == unpack) << std::endl;
}