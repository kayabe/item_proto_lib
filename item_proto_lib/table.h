#ifndef _TABLE_H_
#define _TABLE_H_

namespace item_proto_lib
{
#pragma pack(4)
    typedef struct SProtoHeader
    {
        uint32_t    dwFourCC;
        uint32_t    dwEncryptSize;
        uint32_t    dwCompressedSize;
        uint32_t    dwRealSize;
    } TProtoHeader;

    typedef struct SPreHeader
    {
        uint32_t    dwFourCC;
        uint32_t    dwVersion;
        uint32_t    dwStride;
        uint32_t    dwCount;
        uint32_t    dwDataSize;
    } TPreHeader;
#pragma pack()

#pragma pack(push)
#pragma pack(1)

    typedef struct SItemLimit
    {
        uint8_t     bType;
        int32_t     lValue;
    } TItemLimit;

    typedef struct SItemApply
    {
        uint8_t     bType;
        int32_t     lValue;
    } TItemApply;

    typedef struct SItemTable
    {
        uint32_t    dwVnum;
        uint32_t    dwVnumRange;
        char        szName[ITEM_NAME_MAX_LEN + 1];
        char        szLocaleName[ITEM_NAME_MAX_LEN + 1];
        uint8_t     bType;
        uint8_t     bSubType;

        uint8_t     bWeight;
        uint8_t     bSize;

        uint32_t    dwAntiFlags;
        uint32_t    dwFlags;
        uint32_t    dwWearFlags;
        uint32_t    dwImmuneFlag;

        uint32_t    dwIBuyItemPrice;
        uint32_t    dwISellItemPrice;

        TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
        TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
        int32_t     alValues[ITEM_VALUES_MAX_NUM];
        int32_t     alSockets[ITEM_SOCKET_MAX_NUM];
        uint32_t    dwRefinedVnum;
        uint16_t    wRefineSet;
        uint8_t     bAlterToMagicItemPct;
        uint8_t     bSpecular;
        uint8_t     bGainSocketPct;
    } TItemTable;

#pragma pack(pop)
}

#endif
