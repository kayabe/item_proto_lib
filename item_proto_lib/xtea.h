#ifndef _XTEA_H_
#define _XTEA_H_

namespace item_proto_lib
{
    namespace xtea
    {
        /* XTEA constants -- DON'T CHANGE! */
        const int32_t TEA_ROUND = 32;
        const int32_t TEA_DELTA = 0x9E3779B9;

        class CXTea
        {
        private:
            CXTea() {};
            CXTea(const CXTea&) {};
            CXTea& operator=(const CXTea&) {};

            static void Decode(const uint32_t sz, const uint32_t sy, const uint32_t *key, uint32_t *dest);
            static void Encode(const uint32_t sz, const uint32_t sy, const uint32_t *key, uint32_t *dest);

        public:
            static void     Decrypt(const uint32_t* src, uint32_t * dst, uint32_t size);
            static uint32_t Encrypt(const uint32_t* src, uint32_t * dst, uint32_t size);
        };
    }
}

#endif
