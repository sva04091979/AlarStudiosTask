#pragma once

#define _SILENCE_AMP_DEPRECATION_WARNINGS

#include <amp.h>
#include <vector>

class TSimpleCode {
public:
	using TUnpack = std::vector<int>;
	using TPack = std::vector<unsigned int>;
public:
	static TPack Pack(const TUnpack& in);
	static TUnpack Unpack(const TPack& in);
};

