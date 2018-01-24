#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

namespace item_proto_lib
{
    const int32_t ITEM_NAME_MAX_LEN     = 24;
    const int32_t ITEM_LIMIT_MAX_NUM    = 2;
    const int32_t ITEM_APPLY_MAX_NUM    = 3;
    const int32_t ITEM_VALUES_MAX_NUM   = 6;
    const int32_t ITEM_SOCKET_MAX_NUM   = 3;

    /* Item proto TEA key */
    const uint32_t ITEMPROTO_KEY[4] =
    {
        173217,
        72619434,
        408587239,
        27973291
    };
}

#endif
