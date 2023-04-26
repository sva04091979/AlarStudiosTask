#pragma once

#include <algorithm>
#include <amp.h>
#include <optional>
#include <vector>

class TSimpleCode {
	static constexpr unsigned char kMask[] = { 0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F };
	using TVal = unsigned char;
	using TRem = std::optional<TVal>;
	using TComp = std::pair<TVal, TRem>;
public:
	using TUnpack = std::vector<int>;
	using TPack = std::vector<unsigned char>;
public:
	static TPack Code(const TUnpack& in);
	static TUnpack Decode(const TPack& in);
private:
	static void MakeCode(TUnpack::const_iterator itIn, TUnpack::const_iterator endIn, TPack::iterator itRet);
	static void MakeCode(const TUnpack::value_type* itIn, TPack::value_type* itRet);
	static TComp CompPack(TVal val, size_t step);
	static TVal CompUnpack(size_t step, TVal first, TVal second);
};

