#pragma once

#include <cstdio>
#include <cstdint>

namespace SCPEndianImpl{

    template<typename UINT>
    UINT byteswap(UINT const a);

   

    template<>
    constexpr uint16_t byteswap(uint16_t const a){
        constexpr uint8_t b[] = {
            8 * (2 - 1)
        };

        auto const x =
            (0x00ffULL & a) << b[0] |
            (0xff00ULL & a) >> b[0]
        ;

        return static_cast<uint16_t>(x);
    }

    template<>
    constexpr uint32_t byteswap(uint32_t const a){
        constexpr uint8_t b[] = {
            8 * (2 - 1),
            8 * (4 - 1)
        };

        auto const x =
            (0x000000ffULL & a) << b[1] |
            (0x0000ff00ULL & a) << b[0] |
            (0x00ff0000ULL & a) >> b[0] |
            (0xff000000ULL & a) >> b[1]
        ;

        return static_cast<uint32_t>(x);
    }

    template<>
    constexpr uint64_t byteswap(uint64_t const a){
        constexpr uint8_t b[] = {
            8 * (2 - 1),
            8 * (4 - 1),
            8 * (6 - 1),
            8 * (8 - 1)
        };

        auto const x =
            (0x00000000000000ffULL & a) << b[3] |
            (0x000000000000ff00ULL & a) << b[2] |
            (0x0000000000ff0000ULL & a) << b[1] |
            (0x00000000ff000000ULL & a) << b[0] |
            (0x000000ff00000000ULL & a) >> b[0] |
            (0x0000ff0000000000ULL & a) >> b[1] |
            (0x00ff000000000000ULL & a) >> b[2] |
            (0xff00000000000000ULL & a) >> b[3]
        ;

        return static_cast<uint64_t>(x);
    }
	template <>
	constexpr float byteswap(float const a)
	{

		union fswap {
			float f;
			uint32_t ui32;
		} swapper    = {a};
		swapper.ui32 = byteswap(swapper.ui32);
		return swapper.f;
	}
    // ==============================

    class isBE{
        constexpr static uint32_t u4 = 1;
        constexpr static uint8_t  u1  = (const uint8_t &) u4;
    public:
        constexpr static bool value = u1 == 0;
    };

    // ==============================

    template<bool b>
    struct be_tag{};

    // ==============================

    template<typename UINT>
    constexpr UINT to_le(UINT const a, be_tag<false>){
        return a;
    }

    template<typename UINT>
    constexpr UINT to_le(UINT const a, be_tag<true>){
        return byteswap(a);
    }

} // namespace myendian_impl_

constexpr inline uint16_t INTEL_SHORT(uint16_t const a){
    return SCPEndianImpl::to_le(a, SCPEndianImpl::be_tag<SCPEndianImpl::isBE::value>{});
}

constexpr inline uint32_t INTEL_INT(uint32_t const a){
    return SCPEndianImpl::to_le(a, SCPEndianImpl::be_tag<SCPEndianImpl::isBE::value>{});
}

constexpr inline uint64_t INTEL_LONG(uint64_t const a){
    return SCPEndianImpl::to_le(a, SCPEndianImpl::be_tag<SCPEndianImpl::isBE::value>{});
}

constexpr inline float INTEL_FLOAT(float const a){
	return SCPEndianImpl::to_le(a, SCPEndianImpl::be_tag<SCPEndianImpl::isBE::value>{});
}

constexpr inline float INTEL_FLOAT(const float* a)
{
	return SCPEndianImpl::to_le(*a, SCPEndianImpl::be_tag<SCPEndianImpl::isBE::value>{});
}