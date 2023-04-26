#pragma once

#define _SILENCE_AMP_DEPRECATION_WARNINGS

#include <algorithm>
#include <amp.h>
#include <vector>

class TSimpleCode {
public:
	using TUnpack = std::vector<int>;
	using TPack = std::vector<unsigned int>;
public:
	static TPack Code(const TUnpack& in);
	static TUnpack Decode(const TPack& in);
private:
	static void MakeDecode(const unsigned int* itIn, int* itRet) restrict(amp);
	//static void CompPack(TVal val, unsigned int step) restrict(amp);
	//static TVal CompUnpack(size_t step, TVal first, TVal second);
};

