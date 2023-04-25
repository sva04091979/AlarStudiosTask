#include <iostream>

#include "SimpleCode.h"

// static
TSimpleCode::TPack TSimpleCode::Code(const TUnpack& in) {
	if (!in.size())
		return TPack();
	size_t packSize = in.size() / 8 * 7 + in.size() % 8;
	TPack ret(packSize, 0);
	auto itIn = in.begin();
	auto itRet = ret.begin();
	auto itEnd = itIn + in.size() / 8;
	while (itIn < itEnd) {
		MakeCode(itIn, itIn+8, itRet);
		itIn += 8;
		itRet += 7;
	}
	MakeCode(itIn, in.end(), itRet);
	return ret;
}

// static
TSimpleCode::TUnpack TSimpleCode::Decode(const TPack& in) {
	if (!in.size())
		return TUnpack();
	size_t unpackSize = in.size() / 7 * 8 + in.size() % 7;
	if (!(in.back()&kMask[6]))
		--unpackSize;
	TUnpack ret(unpackSize, 0);
	auto itIn = in.begin();
	auto itRet = ret.begin();
	size_t step = 0;
	while (itRet!= ret.end()) {
		auto itNext = std::next(itIn);
		*itRet = CompUnpack(step, *itIn, itNext == in.end() ? 0 : *itNext);
		if (step == 0)
			++step;
		else if (step == 7) {
			++itIn;
			step = 0;
		}
		else {
			++itIn;
			++step;
		}
		++itRet;
	}
	return ret;
}

// static
void TSimpleCode::MakeCode(TUnpack::const_iterator itIn, TUnpack::const_iterator endIn, TPack::iterator itRet) {
	size_t step = 0;
	while (itIn < endIn) {
		auto val = CompPack(*itIn++, step);
		*itRet |= val.first;
		if (val.second) {
			*++itRet |= *val.second;
		}
		if (step == 7) {
			++itRet;
			step = 0;
		}
		else ++step;
	}
}

TSimpleCode::TComp TSimpleCode::CompPack(TVal val, size_t step) {
	switch (step) {
	case 0: return { static_cast<TVal>(val << 1),{} };
	case 7: return { val,{} };
	default: return { val >> (7 - step), (val & kMask[6 - step]) << (step + 1) };
	}
}

TSimpleCode::TVal TSimpleCode::CompUnpack(size_t step, TVal first, TVal second) {
	switch (step) {
	case 0: return first >> 1;
	case 7: return first & kMask[6];
	default: return ((first & kMask[step - 1]) << (7 - step)) | (second >> (step + 1));
	}
}

