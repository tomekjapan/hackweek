#ifndef ROBOCOM_SHARED_BASE_HPP
#define ROBOCOM_SHARED_BASE_HPP

#ifdef __GNUC__
	typedef __INT8_TYPE__ SInt8;
	typedef __UINT8_TYPE__ UInt8;
	typedef __INT16_TYPE__ SInt16;
	typedef __UINT16_TYPE__ UInt16;
	typedef __INT32_TYPE__ SInt32;
	typedef __UINT32_TYPE__ UInt32;
	typedef __INT64_TYPE__ SInt64;
	typedef __UINT64_TYPE__ UInt64;
#else
#error Port!
#endif

namespace common
{

#ifdef _ORDER_LITTLE_ENDIAN_

	inline UInt16 hton (UInt16 v) throw () { return v; }
	inline UInt32 hton (UInt32 v) throw () { return v; }
	inline UInt16 ntoh (UInt16 v) throw () { return v; }
	inline UInt32 ntoh (UInt32 v) throw () { return v; }

#else

	inline UInt16 swap_bytes (UInt16 v) throw ()
	{
		return ((v & 0x00FF) << 8)
			|  ((v & 0xFF00) >> 8);
	}

	inline UInt32 swap_bytes (UInt32 v) throw ()
	{
		return ((v & 0x000000FF) << 24)
			|  ((v & 0x0000FF00) << 8)
			|  ((v & 0x00FF0000) >> 8)
			|  ((v & 0xFF000000) >> 24);
	}

	inline UInt16 hton (UInt16 v) throw () { return swap_bytes(v); }
	inline UInt32 hton (UInt32 v) throw () { return swap_bytes(v); }
	inline UInt16 ntoh (UInt16 v) throw () { return swap_bytes(v); }
	inline UInt32 ntoh (UInt32 v) throw () { return swap_bytes(v); }

#endif

}

#endif
