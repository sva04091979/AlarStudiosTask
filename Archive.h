#pragma once

template<typename CodeType>
class TArchive {
public:
	using TUnpack = typename CodeType::TUnpack;
	using TPack = typename CodeType::TPack;
public:
	static TPack Pack(const TUnpack& in) { return CodeType::Pack(in); }
	static TUnpack Unpack(const TPack& in) { return CodeType::Unpack(in); }
};

