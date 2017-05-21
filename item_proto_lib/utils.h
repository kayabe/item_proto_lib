#ifndef _UTILS_HEADER_
#define _UTILS_HEADER_

namespace item_proto_lib
{
    namespace utils
    {
        /* Used to convert arrays of data by casting - use with care */
        template <typename TYPEIn, typename TYPEOut>
        TYPEOut ConvertFunc(TYPEIn input)
        {
            return static_cast<TYPEOut>(input);
        }

        /* Create converter */
        template <typename TYPEIn, typename TYPEOut>
        System::Converter<TYPEIn, TYPEOut>^ GetConverter()
        {
            return gcnew System::Converter<TYPEIn, TYPEOut>(ConvertFunc<TYPEIn, TYPEOut>);
        }

        /* Compute FourCC */
        static uint32_t GetFourCC(int8_t A, int8_t B, int8_t C, int8_t D)
        {
            uint32_t ch0 = static_cast<uint32_t>(A) << 0;
            uint32_t ch1 = static_cast<uint32_t>(B) << 8;
            uint32_t ch2 = static_cast<uint32_t>(C) << 16;
            uint32_t ch3 = static_cast<uint32_t>(D) << 24;

            return (ch0 | ch1 | ch2 | ch3);
        }

        /* Read sizeof(TYPE) bytes of data from file */
        template <typename TYPE>
        void ReadData(std::ifstream& file, TYPE& dst)
        {
            char * bytes = new char[sizeof(TYPE)];
            file.read(bytes, sizeof(TYPE));
            memcpy(&dst, bytes, sizeof(TYPE));
            delete[] bytes;
        }

        /* Write sizeof(TYPE) bytes of data to file */
        template <typename TYPE>
        void WriteData(std::ofstream& file, const TYPE& data)
        {
            char * bytes = new char[sizeof(TYPE)];
            memcpy(bytes, &data, sizeof(TYPE));
            file.write(bytes, sizeof(TYPE));
            delete[] bytes;
        }

        /* Sorting predicate */
        static bool SortPredicate(item::CItem^ left, item::CItem^ right)
        {
            return (left->dwVnum < right->dwVnum);
        }

        /* Convert from managed CItem to unmanaged TItemTable */
        static table::TItemTable ItemToTable(item::CItem^ item)
        {
            table::TItemTable itemTable;

            itemTable.dwVnum = item->dwVnum;
            itemTable.dwVnumRange = item->dwVnumRange;

            array<char>^ arr = System::Array::ConvertAll(item->szLocaleName->ToCharArray(), GetConverter<wchar_t, char>());
            for (uint32_t i = 0; i < arr->Length; ++i)
            {
                itemTable.szLocaleName[i] = arr[i];
            }
            itemTable.szLocaleName[arr->Length] = 0;

            arr = System::Array::ConvertAll(item->szName->ToCharArray(), GetConverter<wchar_t, char>());
            for (uint32_t i = 0; i < arr->Length; ++i)
            {
                itemTable.szName[i] = arr[i];
            }
            itemTable.szName[arr->Length] = 0;

            itemTable.bType = item->bType;
            itemTable.bSubType = item->bSubType;

            itemTable.bWeight = item->bWeight;
            itemTable.bSize = item->bSize;

            itemTable.dwAntiFlags = item->dwAntiFlags;
            itemTable.dwFlags = item->dwFlags;
            itemTable.dwWearFlags = item->dwWearFlags;
            itemTable.dwImmuneFlag = item->dwImmuneFlag;

            itemTable.dwIBuyItemPrice = item->dwIBuyItemPrice;
            itemTable.dwISellItemPrice = item->dwISellItemPrice;

            for (uint32_t i = 0; i < cnst::ITEM_LIMIT_MAX_NUM; ++i)
            {
                itemTable.aLimits[i].bType = item->aLimits[i]->bType;
                itemTable.aLimits[i].lValue = item->aLimits[i]->lValue;
            }

            for (uint32_t i = 0; i < cnst::ITEM_APPLY_MAX_NUM; ++i)
            {
                itemTable.aApplies[i].bType = item->aApplies[i]->bType;
                itemTable.aApplies[i].lValue = item->aApplies[i]->lValue;
            }

            for (uint32_t i = 0; i < cnst::ITEM_VALUES_MAX_NUM; ++i)
            {
                itemTable.alValues[i] = item->alValues[i];
            }

            for (uint32_t i = 0; i < cnst::ITEM_SOCKET_MAX_NUM; ++i)
            {
                itemTable.alSockets[i] = item->alSockets[i];
            }

            itemTable.dwRefinedVnum = item->dwRefinedVnum;
            itemTable.wRefineSet = item->wRefineSet;
            itemTable.bAlterToMagicItemPct = item->bAlterToMagicItemPct;
            itemTable.bSpecular = item->bSpecular;
            itemTable.bGainSocketPct = item->bGainSocketPct;

            return itemTable;
        }
    }
}

#endif
