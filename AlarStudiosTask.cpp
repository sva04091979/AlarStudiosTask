
#include <algorithm>
#include <iostream>
#include <optional>
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
	//int n = 0;
	//std::generate(data.begin(), data.end(), [&n] {return ++n; });
	auto time=std::make_unique<TTime>();
	TPack pack = TArch::Code(data);
	TUnpack unpack = TArch::Decode(pack);
	time.reset();
	//for (auto& it : data)
	//	std::cout << it << ' ';
	//std::cout << std::endl;
	//std::cout << std::endl;
	//for (auto& it : pack)
	//	std::cout << it << ' ';
	//std::cout << std::endl;
	//std::cout << std::endl;
	//for (auto& it : unpack)
	//	std::cout << it << ' ';
	//std::cout << std::endl;
	std::cout << data.size() << std::endl;
	std::cout << pack.size() << std::endl;
	std::cout << unpack.size() << std::endl;
	std::cout << std::boolalpha << (data == unpack) << std::endl;
}