#include "Stdafx.h"
#include "item.h"

namespace item_proto_lib
{
    CItemLimit::CItemLimit(uint8_t bType, int32_t lValue)
    {
        this->bType = bType;
        this->lValue = lValue;
    }

    CItemLimit::CItemLimit(const TItemLimit& itemLimit)
    {
        bType = itemLimit.bType;
        lValue = itemLimit.lValue;
    }

    CItemLimit::~CItemLimit()
    {
    }

    CItemApply::CItemApply(uint8_t bType, int32_t lValue)
    {
        this->bType = bType;
        this->lValue = lValue;
    }

    CItemApply::CItemApply(const TItemApply& itemApply)
    {
        bType = itemApply.bType;
        lValue = itemApply.lValue;
    }

    CItemApply::~CItemApply()
    {
    }

    CItem::CItem(uint32_t vnum)
    {
        dwVnum = vnum;
    }

    CItem::CItem(const TItemTable & itemTable)
    {
        dwVnum = itemTable.dwVnum;
        dwVnumRange = itemTable.dwVnumRange;
        szName = gcnew System::String(itemTable.szName);
        szLocaleName = gcnew System::String(itemTable.szLocaleName);
        bType = itemTable.bType;
        bSubType = itemTable.bSubType;

        bWeight = itemTable.bWeight;
        bSize = itemTable.bSize;

        dwAntiFlags = itemTable.dwAntiFlags;
        dwFlags = itemTable.dwFlags;
        dwWearFlags = itemTable.dwWearFlags;
        dwImmuneFlag = itemTable.dwImmuneFlag;

        dwIBuyItemPrice = itemTable.dwIBuyItemPrice;
        dwISellItemPrice = itemTable.dwISellItemPrice;

        aLimits = gcnew array<CItemLimit^>(ITEM_LIMIT_MAX_NUM);
        for (uint32_t i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
        {
            aLimits[i] = gcnew CItemLimit(itemTable.aLimits[i]);
        }

        aApplies = gcnew array<CItemApply^>(ITEM_APPLY_MAX_NUM);
        for (uint32_t i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
        {
            aApplies[i] = gcnew CItemApply(itemTable.aApplies[i]);
        }

        alValues = gcnew array<int32_t>(ITEM_VALUES_MAX_NUM);
        for (uint32_t i = 0; i < ITEM_VALUES_MAX_NUM; ++i)
        {
            alValues[i] = itemTable.alValues[i];
        }

        alSockets = gcnew array<int32_t>(ITEM_SOCKET_MAX_NUM);
        for (uint32_t i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
        {
            alSockets[i] = itemTable.alSockets[i];
        }

        dwRefinedVnum = itemTable.dwRefinedVnum;
        wRefineSet = itemTable.wRefineSet;
        bAlterToMagicItemPct = itemTable.bAlterToMagicItemPct;
        bSpecular = itemTable.bSpecular;
        bGainSocketPct = itemTable.bGainSocketPct;
    }

    CItem::~CItem()
    {
    }
}
