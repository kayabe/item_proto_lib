#include "stdafx.h"
#include "item_proto_lib.h"

using namespace System;
using namespace cliext;

namespace item_proto_lib
{
    CItemProtoManager::CItemProtoManager()
    {
        m_itemVector        = gcnew vector<CItem^>();
        m_version           = 1;
        m_fourCC            = GetFourCC('M', 'I', 'P', 'X');
        m_headerFourCC      = GetFourCC('M', 'C', 'O', 'Z');
        m_encryptedFourCC   = GetFourCC('M', 'C', 'O', 'Z');

        lzo_init();
    }

    CItemProtoManager::~CItemProtoManager()
    {
    }

    TError CItemProtoManager::ReadProto(String^ name)
    {
        TError ret = TError::ERR_OK;

        array<char>^ arrName = Array::ConvertAll(name->ToCharArray(), GetConverter<wchar_t, char>());
        pin_ptr<char> pname = &arrName[0];

        std::ifstream file(pname, std::ios::binary);

        if (file.is_open())
        {
            ret = ProcessProto(file);
            file.close();
        }
        else
        {
            ret = TError::ERR_FILE_INACCESSIBLE;
        }

        return ret;
    }

    TError CItemProtoManager::WriteProto(String^ name)
    {
        TError ret = TError::ERR_OK;

        array<char>^ arrName = Array::ConvertAll(name->ToCharArray(), GetConverter<wchar_t, char>());
        pin_ptr<char> pname = &arrName[0];

        std::ofstream file(pname, std::ios::binary);

        if (file.is_open())
        {
            ret = Write(file);
            file.close();
        }
        else
        {
            ret = TError::ERR_FILE_INACCESSIBLE;
        }

        return ret;
    }

    TError CItemProtoManager::Write(std::ofstream& file)
    {
        TError ret = TError::ERR_OK;

        /*
        * >> DWORD fourCC           -
        * >> DWORD version           |
        * >> DWORD stride            | TPreHeader
        * >> DWORD elementsCount     |
        * >> DWORD dataSize         -
        * >> TProtoHeader header
        ----- ENCRYPT -----
        * >> DWORD fourCC
        ----- COMPRESS -----
        * >> DATA
        */

        TPreHeader preHeader;
        preHeader.dwFourCC = m_fourCC;
        preHeader.dwVersion = m_version;
        preHeader.dwStride = sizeof(TItemTable);
        preHeader.dwCount = m_itemVector->size();
        preHeader.dwDataSize = m_itemVector->size() * sizeof(TItemTable);

        TProtoHeader header;
        header.dwFourCC = m_headerFourCC;
        header.dwRealSize = m_itemVector->size() * sizeof(TItemTable);
        header.dwCompressedSize = 0;
        header.dwEncryptSize = 0;

        CBuffer^ itemsBuffer = gcnew CBuffer(header.dwRealSize);
        for (int32_t i = 0; i < m_itemVector->size(); ++i)
        {
            itemsBuffer->Write(ItemToTable(m_itemVector[i]));
        }

        uint32_t dataSize = sizeof(uint32_t) + (header.dwRealSize + (header.dwRealSize / 64) + 16 + 3) + 8;
        CBuffer^ compressedBuffer = gcnew CBuffer(dataSize);
        compressedBuffer->Write(m_encryptedFourCC);

        CBuffer^ workMemBuffer = gcnew CBuffer(LZO1X_1_MEM_COMPRESS);
        lzo_uint compressedSize = 0;
        int lzoRet = lzo1x_1_compress((const unsigned char *)itemsBuffer->GetPtr(), itemsBuffer->GetDataSize(), (unsigned char*)compressedBuffer->GetWritePtr(), &compressedSize, (void *)workMemBuffer->GetPtr());
        if (LZO_E_OK == lzoRet)
        {
            CBuffer^ encryptedBuffer = gcnew CBuffer(dataSize);

            header.dwCompressedSize = compressedSize;
            header.dwEncryptSize = xtea::CXTea::Encrypt((const uint32_t *)compressedBuffer->GetPtr(), (uint32_t *)encryptedBuffer->GetPtr(), compressedSize + 19);

            file.write((const char*)&preHeader, sizeof(preHeader));
            file.write((const char*)&header, sizeof(TProtoHeader));
            file.write(encryptedBuffer->GetPtr(), header.dwEncryptSize);
        }
        else
        {
            ret = TError::ERR_FAILED_COMPRESSION;
        }

        return ret;
    }

    TError CItemProtoManager::ProcessProto(std::ifstream& file)
    {
        TError ret = TError::ERR_OK;

        TPreHeader preHeader;
        ReadData(file, preHeader);

        if (preHeader.dwFourCC == m_fourCC)
        {
            if ((preHeader.dwVersion == m_version) && (preHeader.dwStride == sizeof(TItemTable)))
            {
                ret = ReadItems(file, preHeader.dwCount, preHeader.dwDataSize);
            }
            else
            {
                ret = TError::ERR_WRONG_VERSION_OR_TABLESIZE;
            }
        }
        else
        {
            ret = TError::ERR_WRONG_FIRST_FOURCC;
        }

        return ret;
    }

    TError CItemProtoManager::ReadItems(std::ifstream& file, uint32_t count, uint32_t size)
    {
        TError ret = TError::ERR_OK;

        TProtoHeader header;
        ReadData(file, header);

        if (header.dwFourCC != m_headerFourCC)
        {
            ret = TError::ERR_WRONG_HEADER_FOURCC;
        }
        else
        {
            CBuffer^ srcBuffer = gcnew CBuffer(header.dwEncryptSize);
            file.read(srcBuffer->GetPtr(), header.dwEncryptSize);

            CBuffer^ compBuffer = gcnew CBuffer(header.dwEncryptSize);
            xtea::CXTea::Decrypt((const uint32_t *)srcBuffer->GetPtr(), (uint32_t *)compBuffer->GetPtr(), header.dwEncryptSize);

            if ((*(uint32_t *)compBuffer->GetPtr()) != m_encryptedFourCC)
            {
                ret = TError::ERR_WRONG_ENCRYPTED_FOURCC;
            }
            else
            {
                CBuffer^ data = gcnew CBuffer(header.dwRealSize);
                lzo_uint uiSize = 0;

                lzo1x_decompress((const unsigned char *)(compBuffer->GetPtr() + sizeof(uint32_t)), header.dwCompressedSize, (unsigned char *)data->GetPtr(), &uiSize, nullptr);

                if (uiSize != header.dwRealSize)
                {
                    ret = TError::ERR_CORRUPTED;
                }
                else
                {
                    TItemTable * table = reinterpret_cast<TItemTable *>(data->GetPtr());

                    for (uint32_t i = 0; i < count; ++i)
                    {
                        m_itemVector->push_back(gcnew CItem(table[i]));
                    }
                }
            }
        }

        return ret;
    }

    CItem^ CItemProtoManager::GetItem(int32_t index)
    {
        CItem ^ ret = nullptr;

        if (index >= m_itemVector->size())
        {
            throw gcnew IndexOutOfRangeException;
        }
        else
        {
            ret = m_itemVector[index];
        }

        return ret;
    }

    int32_t CItemProtoManager::GetCount()
    {
        return m_itemVector->size();
    }

    void CItemProtoManager::DeleteItem(int32_t index)
    {
        if (index < m_itemVector->size())
        {
            m_itemVector->erase(m_itemVector->begin() + index);
        }
    }

    int32_t CItemProtoManager::AddItem(CItem^ item)
    {
        int32_t ret = -1;

        if (item != nullptr)
        {
            int32_t index = -1;
            while (m_itemVector[++index]->dwVnum < item->dwVnum);

            if (m_itemVector[index]->dwVnum > item->dwVnum)
            {
                ret = m_itemVector->insert_n(index, 1, item);
            }
        }

        return ret;
    }

    void CItemProtoManager::AddBack(CItem^ item)
    {
        if (item != nullptr)
        {
            m_itemVector->push_back(item);
        }
    }

    void CItemProtoManager::SortItems()
    {
        cliext::sort(m_itemVector->begin(), m_itemVector->end(),  SortPredicate);
    }

    void CItemProtoManager::SetVersion(uint32_t version)
    {
        m_version = version;
    }

    void CItemProtoManager::SetFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D)
    {
        m_fourCC =  GetFourCC(A, B, C, D);
    }

    void CItemProtoManager::SetHeaderFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D)
    {
        m_headerFourCC =  GetFourCC(A, B, C, D);
    }

    void CItemProtoManager::SetEncryptedFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D)
    {
        m_encryptedFourCC =  GetFourCC(A, B, C, D);
    }
}
