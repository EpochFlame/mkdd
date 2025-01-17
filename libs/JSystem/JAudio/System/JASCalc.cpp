#include <math.h>
#include <dolphin/os.h>
#include <JSYstem/JUtility/JUTDbg.h>
#include <JSystem/JAudio/System/JASCalc.h>

namespace JASCalc
{
    // There is some sort of pattern, this probably got calculated in some way
    // first index increases by 227 and every 32th iteration 1 more gets added
    // second index decreases by 21 and every 4th iteration 1 more gets added
    // third index decreases by 142 and every 2nd iteration 1 more gets added
    // third index decreases by 116 and every iteration that's a power of 2 1 more gets added
    const s16 CUTOFF_TO_IIR_TABLE[128][4] = {
        {0x0F5C, 0x0A3D, 0x4665, 0x3999},
        {0x103F, 0x0A28, 0x45D7, 0x3925},
        {0x1122, 0x0A14, 0x454A, 0x38B0},
        {0x1205, 0x09FF, 0x44BC, 0x383C},
        {0x12E8, 0x09EA, 0x442E, 0x37C8},
        {0x13CB, 0x09D6, 0x43A0, 0x3754},
        {0x14AE, 0x09C1, 0x4312, 0x36E0},
        {0x1591, 0x09AC, 0x4284, 0x366C},
        {0x1674, 0x0998, 0x41F6, 0x35F8},
        {0x1757, 0x0983, 0x4168, 0x3584},
        {0x183A, 0x096E, 0x40DA, 0x3510},
        {0x191D, 0x095A, 0x404C, 0x349C},
        {0x1A00, 0x0945, 0x3FBE, 0x3427},
        {0x1AE3, 0x0931, 0x3F31, 0x33B3},
        {0x1BC6, 0x091C, 0x3EA3, 0x333F},
        {0x1CA9, 0x0907, 0x3E15, 0x32CB},
        {0x1D8C, 0x08F3, 0x3D87, 0x3257},
        {0x1E6F, 0x08DE, 0x3CF9, 0x31E3},
        {0x1F52, 0x08C9, 0x3C6B, 0x316F},
        {0x2035, 0x08B5, 0x3BDD, 0x30FB},
        {0x2118, 0x08A0, 0x3B4F, 0x3087},
        {0x21FC, 0x088B, 0x3AC1, 0x3012},
        {0x22DF, 0x0877, 0x3A33, 0x2F9E},
        {0x23C2, 0x0862, 0x39A6, 0x2F2A},
        {0x24A5, 0x084D, 0x3918, 0x2EB6},
        {0x2588, 0x0839, 0x388A, 0x2E42},
        {0x266B, 0x0824, 0x37FC, 0x2DCE},
        {0x274E, 0x0810, 0x376E, 0x2D5A},
        {0x2831, 0x07FB, 0x36E0, 0x2CE6},
        {0x2914, 0x07E6, 0x3652, 0x2C72},
        {0x29F7, 0x07D2, 0x35C4, 0x2BFE},
        {0x2ADA, 0x07BD, 0x3536, 0x2B89},
        {0x2BBD, 0x07A8, 0x34A8, 0x2B15},
        {0x2CA0, 0x0794, 0x341B, 0x2AA1},
        {0x2D83, 0x077F, 0x338D, 0x2A2D},
        {0x2E66, 0x076A, 0x32FF, 0x29B9},
        {0x2F49, 0x0756, 0x3271, 0x2945},
        {0x302C, 0x0741, 0x31E3, 0x28D1},
        {0x310F, 0x072D, 0x3155, 0x285D},
        {0x31F2, 0x0718, 0x30C7, 0x27E9},
        {0x32D5, 0x0703, 0x3039, 0x2775},
        {0x33B8, 0x06EF, 0x2FAB, 0x2700},
        {0x349C, 0x06DA, 0x2F1D, 0x268C},
        {0x357F, 0x06C5, 0x2E8F, 0x2618},
        {0x3662, 0x06B1, 0x2E02, 0x25A4},
        {0x3745, 0x069C, 0x2D74, 0x2530},
        {0x3828, 0x0687, 0x2CE6, 0x24BC},
        {0x390B, 0x0673, 0x2C58, 0x2448},
        {0x39EE, 0x065E, 0x2BCA, 0x23D4},
        {0x3AD1, 0x0649, 0x2B3C, 0x2360},
        {0x3BB4, 0x0635, 0x2AAE, 0x22EB},
        {0x3C97, 0x0620, 0x2A20, 0x2277},
        {0x3D7A, 0x060C, 0x2992, 0x2203},
        {0x3E5D, 0x05F7, 0x2904, 0x218F},
        {0x3F40, 0x05E2, 0x2877, 0x211B},
        {0x4023, 0x05CE, 0x27E9, 0x20A7},
        {0x4106, 0x05B9, 0x275B, 0x2033},
        {0x41E9, 0x05A4, 0x26CD, 0x1FBF},
        {0x42CC, 0x0590, 0x263F, 0x1F4B},
        {0x43AF, 0x057B, 0x25B1, 0x1ED7},
        {0x4492, 0x0566, 0x2523, 0x1E62},
        {0x4575, 0x0552, 0x2495, 0x1DEE},
        {0x4658, 0x053D, 0x2407, 0x1D7A},
        {0x473B, 0x0529, 0x2379, 0x1D06},
        {0x481F, 0x0514, 0x22EB, 0x1C92},
        {0x4902, 0x04FF, 0x225E, 0x1C1E},
        {0x49E5, 0x04EB, 0x21D0, 0x1BAA},
        {0x4AC8, 0x04D6, 0x2142, 0x1B36},
        {0x4BAB, 0x04C1, 0x20B4, 0x1AC2},
        {0x4C8E, 0x04AD, 0x2026, 0x1A4E},
        {0x4D71, 0x0498, 0x1F98, 0x19D9},
        {0x4E54, 0x0483, 0x1F0A, 0x1965},
        {0x4F37, 0x046F, 0x1E7C, 0x18F1},
        {0x501A, 0x045A, 0x1DEE, 0x187D},
        {0x50FD, 0x0445, 0x1D60, 0x1809},
        {0x51E0, 0x0431, 0x1CD3, 0x1795},
        {0x52C3, 0x041C, 0x1C45, 0x1721},
        {0x53A6, 0x0408, 0x1BB7, 0x16AD},
        {0x5489, 0x03F3, 0x1B29, 0x1639},
        {0x556C, 0x03DE, 0x1A9B, 0x15C4},
        {0x564F, 0x03CA, 0x1A0D, 0x1550},
        {0x5732, 0x03B5, 0x197F, 0x14DC},
        {0x5815, 0x03A0, 0x18F1, 0x1468},
        {0x58F8, 0x038C, 0x1863, 0x13F4},
        {0x59DB, 0x0377, 0x17D5, 0x1380},
        {0x5ABF, 0x0362, 0x1747, 0x130C},
        {0x5BA2, 0x034E, 0x16BA, 0x1298},
        {0x5C85, 0x0339, 0x162C, 0x1224},
        {0x5D68, 0x0324, 0x159E, 0x11B0},
        {0x5E4B, 0x0310, 0x1510, 0x113B},
        {0x5F2E, 0x02FB, 0x1482, 0x10C7},
        {0x6011, 0x02E7, 0x13F4, 0x1053},
        {0x60F4, 0x02D2, 0x1366, 0x0FDF},
        {0x61D7, 0x02BD, 0x12D8, 0x0F6B},
        {0x62BA, 0x02A9, 0x124A, 0x0EF7},
        {0x639D, 0x0294, 0x11BC, 0x0E83},
        {0x6480, 0x027F, 0x112F, 0x0E0F},
        {0x6563, 0x026B, 0x10A1, 0x0D9B},
        {0x6646, 0x0256, 0x1013, 0x0D27},
        {0x6729, 0x0241, 0x0F85, 0x0CB2},
        {0x680C, 0x022D, 0x0EF7, 0x0C3E},
        {0x68EF, 0x0218, 0x0E69, 0x0BCA},
        {0x69D2, 0x0204, 0x0DDB, 0x0B56},
        {0x6AB5, 0x01EF, 0x0D4D, 0x0AE2},
        {0x6B98, 0x01DA, 0x0CBF, 0x0A6E},
        {0x6C7B, 0x01C6, 0x0C31, 0x09FA},
        {0x6D5F, 0x01B1, 0x0BA3, 0x0986},
        {0x6E42, 0x019C, 0x0B16, 0x0912},
        {0x6F25, 0x0188, 0x0A88, 0x089D},
        {0x7008, 0x0173, 0x09FA, 0x0829},
        {0x70EB, 0x015E, 0x096C, 0x07B5},
        {0x71CE, 0x014A, 0x08DE, 0x0741},
        {0x72B1, 0x0135, 0x0850, 0x06CD},
        {0x7394, 0x0120, 0x07C2, 0x0659},
        {0x7477, 0x010C, 0x0734, 0x05E5},
        {0x755A, 0x00F7, 0x06A6, 0x0571},
        {0x763D, 0x00E3, 0x0618, 0x04FD},
        {0x7720, 0x00CE, 0x058B, 0x0489},
        {0x7803, 0x00B9, 0x04FD, 0x0414},
        {0x78E6, 0x00A5, 0x046F, 0x03A0},
        {0x79C9, 0x0090, 0x03E1, 0x032C},
        {0x7AAC, 0x007B, 0x0353, 0x02B8},
        {0x7B8F, 0x0067, 0x02C5, 0x0244},
        {0x7C72, 0x0052, 0x0237, 0x01D0},
        {0x7D55, 0x003D, 0x01A9, 0x015C},
        {0x7E38, 0x0029, 0x011B, 0x00E8},
        {0x7F1B, 0x0014, 0x008D, 0x0074},
        {0x7FFF, 0x0000, 0x0000, 0x0000}};
    
    void imixcopy(const s16 *s1, const s16 *s2, s16 *dst, u32 n)
    {
        for (n; n != 0; n--)
        {
            *dst++ = *((s16 *)s1)++;
            *dst++ = *((s16 *)s2)++;
        }
    }

    void bcopyfast(const void *src, void *dest, u32 size)
    {
#line 226
        JUT_ASSERT((reinterpret_cast<u32>(src) & 0x03) == 0);
        JUT_ASSERT((reinterpret_cast<u32>(dest) & 0x03) == 0);
        JUT_ASSERT((size & 0x0f) == 0);
        u32 copy1, copy2, copy3, copy4;

        u32 *usrc = (u32 *)src;
        u32 *udest = (u32 *)dest;

        for (size = size / (4 * sizeof(u32)); size != 0; size--)
        {
            copy1 = *((u32 *)usrc)++;
            copy2 = *((u32 *)usrc)++;
            copy3 = *((u32 *)usrc)++;
            copy4 = *((u32 *)usrc)++;

            *udest++ = copy1;
            *udest++ = copy2;
            *udest++ = copy3;
            *udest++ = copy4;
        }
    }

    void bcopy(const void *src, void *dest, u32 size)
    {
        u32 *usrc;
        u32 *udest;

        u8 *bsrc = (u8 *)src;
        u8 *bdest = (u8 *)dest;

        u8 endbitsSrc = (reinterpret_cast<u32>(bsrc) & 0x03);
        u8 enbitsDst = (reinterpret_cast<u32>(bdest) & 0x03);
        if ((endbitsSrc) == (enbitsDst) && (size & 0x0f) == 0)
        {
            bcopyfast(src, dest, size);
        }
        else if ((endbitsSrc == enbitsDst) && (size >= 16))
        {
            if (endbitsSrc != 0)
            {
                for (endbitsSrc = 4 - endbitsSrc; endbitsSrc != 0; endbitsSrc--)
                {
                    *bdest++ = (u32)*bsrc++;
                    size--;
                }
            }

            udest = (u32 *)bdest;
            usrc = (u32 *)bsrc;

            for (; size >= 4; size -= 4)
            {
                *udest++ = *usrc++;
            }

            if (size != 0)
            {
                bdest = (u8 *)udest;
                bsrc = (u8 *)usrc;

                for (; size != 0; size--)
                {
                    *bdest++ = (u32)*bsrc++;
                }
            }
        }
        else
        {
            for (; size != 0; size--)
            {
                *bdest++ = (u32)*bsrc++;
            }
        }
    }

    void bzerofast(void *dest, u32 size)
    {
#line 336
        JUT_ASSERT((reinterpret_cast<u32>(dest) & 0x03) == 0);
        JUT_ASSERT((size & 0x0f) == 0);

        u32 *udest = (u32 *)dest;

        for (size = size / (4 * sizeof(u32)); size != 0; size--)
        {
            *udest++ = 0;
            *udest++ = 0;
            *udest++ = 0;
            *udest++ = 0;
        }
    }

    void bzero(void *dest, u32 size)
    {
        u32 *udest;
        u8 *bdest = (u8 *)dest;
        if ((size & 0x1f) == 0 && (reinterpret_cast<u32>(dest) & 0x1f) == 0)
        {
            DCZeroRange(dest, size);
            return;
        }

        u8 alignedbitsDst = reinterpret_cast<u32>(bdest) & 0x3;

        if ((size & 0xf) == 0 && alignedbitsDst == 0)
        {
            bzerofast(dest, size);
            return;
        }

        if (size >= 16)
        {
            if (alignedbitsDst != 0)
            {
                for (alignedbitsDst = 4 - alignedbitsDst; alignedbitsDst != 0; alignedbitsDst--)
                {
                    *bdest++ = 0;
                    size--;
                }
            }

            udest = (u32 *)bdest;
            for (; size >= 4; size -= 4)
            {
                *udest++ = 0;
            }

            if (size != 0)
            {
                bdest = (u8 *)udest;
                for (; size != 0; size--)
                {
                    *bdest++ = 0;
                }
            }
        }
        else
        {
            for (; size != 0; size--)
            {
                *bdest++ = 0;
            }
        }
    }

    // currently required because of missing functions
    // JASCalc::hannWindow(short *, unsigned long)
    // JASCalc::hammWindow(short *, unsigned long)
    // JASCalc::fft(float *, float *, unsigned long, long)
    f32 fake1() { return 0.5f; }
    f32 fake2(long x) { return clamp<s16, long>(x); }
    f32 fake3() { return 0.0f; }

    // Thanks Lazy Pony on decomp.me for documenting this!
    /*
     * Calculate 2^x as a float
     */
    f32 pow2(f32 x)
    {
        s32 frac_index = 0;
        union {
            s32 l;
            f32 f;
        } exponent;
        exponent.l = (x >= 0.0f ? (s32)(x - 0.5f) : (s32)(x + 0.5f)); // Shift towards 0 and round
        f32 pannedval = x - exponent.l; // strictly between 1.5 and -1.5

        // 2^x will not fit in an IEEE-754 float
        if(exponent.l > 128) {
            return INFINITY;
        }

        // convert to exponent of IEEE-754 float
        exponent.l += 127;
        exponent.l <<= 23;

        // Calculate the mantissa

        static const f32 scale_frac[] = { 0.0f, 0.5f };
        // { 1 , 1/sqrt2 }
        static const f32 two_to_frac[] = {1.0f, 0.70710677f};
        // coefficients of Taylor polynomial of 2^x - 1 at 0:
        // 2^x - 1 = (log2) * x + (log2)^2 / 2! * x^2 + ...
        static const f32 __two_to_x[] = {
            0.6931472f, 0.24022661,
            0.055502914f, 0.009625022f,
            0.0013131053f, 1.8300806E-4f        
        };

        if (pannedval < 0.0f) {
            frac_index += 1;
        }
        
        f32 ret = pannedval + scale_frac[frac_index];

        // Evaluate Taylor polynomial using Horner's method
        ret = ret * (ret * (ret * (ret * (ret * (ret * __two_to_x[5] + __two_to_x[4]) +
            __two_to_x[3]) + __two_to_x[2]) + __two_to_x[1]) + __two_to_x[0]);
        // 2^n * (corrected mantissa)
        ret = exponent.f * (0.75f * two_to_frac[frac_index] + ((0.25f + ret) * two_to_frac[frac_index]));

        return ret;
    }
}