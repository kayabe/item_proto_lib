#include "Stdafx.h"
#include "xtea.h"

namespace item_proto_lib
{
    namespace xtea
    {
        void CXTea::Decode(const uint32_t sz, const uint32_t sy, const uint32_t * key, uint32_t * dest)
        {
#pragma warning(disable:4307)
            register uint32_t y = sy, z = sz, sum = TEA_DELTA * TEA_ROUND;
#pragma warning(default:4307)

            uint32_t n = TEA_ROUND;

            while (n-- > 0)
            {
                z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
                sum -= TEA_DELTA;
                y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
            }

            *(dest++) = y;
            *dest = z;
        }

        void CXTea::Encode(const uint32_t sz, const uint32_t sy, const uint32_t * key, uint32_t * dest)
        {
            register uint32_t y = sy, z = sz, sum = 0;
            uint32_t n = TEA_ROUND;

            while (n-- > 0)
            {
                y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
                sum += TEA_DELTA;
                z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
            }

            *(dest++) = y;
            *dest = z;
        }

        void CXTea::Decrypt(const uint32_t* src, uint32_t * dst, uint32_t size)
        {
            uint32_t i;
            uint32_t resize;

            if (size % 8 != 0)
            {
                resize = size + 8 - (size % 8);
            }
            else
            {
                resize = size;
            }

            for (i = 0; i < resize >> 3; i++, dst += 2, src += 2)
            {
                uint32_t test = resize >> 3;
                Decode(*(src + 1), *src, cnst::ITEMPROTO_KEY, dst);
            }
        }

        uint32_t CXTea::Encrypt(const uint32_t * src, uint32_t * dst, uint32_t size)
        {
            int i;
            int resize;

            if (size % 8 != 0)
            {
                resize = size + 8 - (size % 8);
                memset((char *)src + size, 0, resize - size);
            }
            else
            {
                resize = size;
            }

            for (i = 0; i < resize >> 3; i++, dst += 2, src += 2)
            {
                Encode(*(src + 1), *src, cnst::ITEMPROTO_KEY, dst);
            }

            return resize;
        }
    }
}
