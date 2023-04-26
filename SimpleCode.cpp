#include <cassert>
#include <iostream>

#include "SimpleCode.h"

// static
TSimpleCode::TPack TSimpleCode::Code(const TUnpack& in) {
	assert(in.size() % 32 == 0);
	if (!in.size())
		return TPack();
	size_t packSize = in.size() / 32 * 7;
	TPack ret(packSize, 0);
	concurrency::extent<2> exIn(in.size() / 32, 32);
	concurrency::extent<2> exPack(in.size() / 32, 7);
	concurrency::array_view<const TUnpack::value_type, 2> arrIn(exIn, in);
	concurrency::array_view<TPack::value_type, 2> arrPack(exPack, ret);
	arrPack.discard_data();
	concurrency::parallel_for_each(arrPack.extent, [=](concurrency::index<2> idx) restrict(amp) {
		unsigned int iLast = 32 * (idx[1]+1) / 7;
		unsigned int cut = 7 - 32 * (idx[1] + 1) % 7;
		unsigned int shift = 7 - cut;
		unsigned int val = static_cast<unsigned int>(arrIn(idx[0], iLast));
		arrPack(idx) = (val >> cut);
		while (shift<32){
			unsigned int val = static_cast<unsigned int>(arrIn(idx[0], --iLast));
			arrPack(idx) |= ( val << shift);
			shift += 7;
		}
								   });
	arrPack.synchronize();
	return ret;
}

// static
TSimpleCode::TUnpack TSimpleCode::Decode(const TPack& in) {
	if (!in.size())
		return TUnpack();
	size_t unpackSize = in.size() / 7 * 32;
	TUnpack ret(unpackSize, 0);
	concurrency::extent<2> exIn(in.size() / 7, 7);
	concurrency::extent<2> exPack(in.size() / 7, 32);
	concurrency::array_view<const TPack::value_type, 2> arrIn(exIn, in);
	concurrency::array_view<TUnpack::value_type, 2> arrPack(exPack, ret);
	arrPack.discard_data();
	concurrency::parallel_for_each(arrPack.extent, [=](concurrency::index<2> idx) restrict(amp) {
		constexpr unsigned int mask = 0xFFFFFFFF;
		unsigned int iFirst = idx[1] * 7 / 32;
		unsigned int iSecond = idx[1] == 31 ? iFirst : (idx[1] + 1) * 7 / 32;
		if (iFirst != iSecond) {
			unsigned cut = (idx[1] + 1) * 7 % 8;
			arrPack(idx) |= (arrIn(idx[0], iSecond) >> (32 - cut));
			arrPack(idx) |= ((arrIn(idx[0], iFirst) & (mask >> 32 - 7 + cut)) << cut);
		}
		else {
			unsigned int shift = (224 - (idx[1] * 7)) % 32;
			unsigned int maskShift = 32 - shift;
			shift -= 7;
			unsigned int val = (arrIn(idx[0], iFirst) & (mask >> maskShift)) >> shift;
			arrPack(idx) = val;
		}
		//if (!idx[1])
		//	MakeDecode(&arrIn(idx[0],0), &arrPack(idx[0],0));
								   });
	arrPack.synchronize();
	return ret;
}

void TSimpleCode::MakeDecode(const unsigned int* itIn, int* itRet) restrict(amp) {
	int* endIt = itRet + 32;
	unsigned int mask = 0xFFFFFFFF;
	int shift = 32 - 7;
	while (itRet < endIt) {
		unsigned int val = *itIn & mask;
		if (!shift) {
			shift = 32 - 7;
			*itRet = (*itIn & mask);
			++itIn;
			mask = 0xFFFFFFFF;
		}
		else if (shift < 0) {
			int delta = -shift;
			*itRet = (val << delta);
			shift += 32;
			*itRet |= (*++itIn >> shift);
			mask = (0xFFFFFFFF >> delta);
			shift -= 7;
		}
		else {
			*itRet = (val >> shift);
			shift -= 7;
			mask >>= 7;
		}
		++itRet;
	}
}

//TSimpleCode::TVal TSimpleCode::CompUnpack(size_t step, TVal first, TVal second) {
//	switch (step) {
//	case 0: return first >> 1;
//	case 7: return first & kMask[6];
//	default: return ((first & kMask[step - 1]) << (7 - step)) | (second >> (step + 1));
//	}
//}

