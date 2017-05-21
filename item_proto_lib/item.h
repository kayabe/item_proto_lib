#ifndef _ITEM_H_
#define _ITEM_H_

namespace item_proto_lib
{
    namespace item
    {
        public ref class CItemLimit
        {
        public:
            CItemLimit(uint8_t bType, int32_t lValue);
            CItemLimit(const table::TItemLimit& itemLimit);
            ~CItemLimit();

        public:
            uint8_t bType;
            int32_t lValue;
        };

        public ref class CItemApply
        {
        public:
            CItemApply(uint8_t bType, int32_t lValue);
            CItemApply(const table::TItemApply& itemApply);
            ~CItemApply();

        public:
            uint8_t bType;
            int32_t lValue;
        };

        public ref class CItem
        {
        public:
            CItem(uint32_t vnum);
            CItem(const table::TItemTable& itemTable);
            ~CItem();

        public:
            uint32_t    dwVnum;
            uint32_t    dwVnumRange;
            System::String^     szName;
            System::String^     szLocaleName;
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

            array<CItemLimit^>^ aLimits;
            array<CItemApply^>^ aApplies;
            array<int32_t>^ alValues;
            array<int32_t>^ alSockets;

            uint32_t    dwRefinedVnum;
            uint16_t    wRefineSet;
            uint8_t     bAlterToMagicItemPct;
            uint8_t     bSpecular;
            uint8_t     bGainSocketPct;
        };
    }
}

#endif
