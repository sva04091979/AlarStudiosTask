#pragma once

template<typename CodeType>
class TArchive {
public:
	using TUnpack = typename CodeType::TUnpack;
	using TPack = typename CodeType::TPack;
public:
	static TPack Code(const TUnpack& in) { return CodeType::Code(in); }
	static TUnpack Decode(const TPack& in) { return CodeType::Decode(in); }
};

