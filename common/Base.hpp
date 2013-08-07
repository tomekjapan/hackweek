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

	inline UInt16 ntoh_UInt16 (const UInt8* p_data) throw ()
	{
		return static_cast<UInt16>( p_data[0] )
			|  static_cast<UInt16>( p_data[1] ) << 8;
	}

	inline UInt32 ntoh_UInt32 (const UInt8* p_data) throw ()
	{
		return static_cast<UInt32>( p_data[0] )
			|  static_cast<UInt32>( p_data[1] ) << 8
			|  static_cast<UInt32>( p_data[2] ) << 16
			|  static_cast<UInt32>( p_data[3] ) << 24;
	}

	inline void hton_UInt16 (UInt8* p_data, UInt16 v) throw ()
	{
		p_data[0] = v & 0xFF;
		p_data[1] = (v >> 8) & 0xFF;
	}

	inline void hton_UInt32 (UInt8* p_data, UInt32 v) throw ()
	{
		p_data[0] = v & 0xFF;
		p_data[1] = (v >> 8) & 0xFF;
		p_data[2] = (v >> 16) & 0xFF;
		p_data[3] = (v >> 24) & 0xFF;
	}

}

#endif
