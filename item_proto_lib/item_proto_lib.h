#ifndef _ITEM_PROTO_LIB_H_
#define _ITEM_PROTO_LIB_H_

namespace item_proto_lib
{
    public enum class TError : int32_t
    {
        /* Everything is ok */
        ERR_OK,

        /* Errors for reading */
        ERR_FILE_INACCESSIBLE,
        ERR_WRONG_FIRST_FOURCC,
        ERR_WRONG_VERSION_OR_TABLESIZE,
        ERR_WRONG_HEADER_FOURCC,
        ERR_WRONG_ENCRYPTED_FOURCC,
        ERR_CORRUPTED,

        /* Errors for writing */
        ERR_FAILED_COMPRESSION,
    };

    public ref class CItemProtoManager
    {
    public:
        CItemProtoManager();
        ~CItemProtoManager();

        /* Tries to read proto. Returns a TError error code if fails. ERR_OK if succeeds. You can handle this to display error messages. */
        TError ReadProto(System::String^ name);
        TError WriteProto(System::String^ name);

        /* Returns the item at a specified index. */
        item::CItem^ GetItem(int32_t index);
        /* Returns the current number of items. */
        int32_t GetCount();

        /* Deletes item at a specified index. */
        void DeleteItem(int32_t index);
        /* Adds item after it's VNUM neighbour and returns its position. Returns -1 if VNUM already exists. */
        int32_t AddItem(item::CItem^ item);
        /* Adds item at the end of the list. This method is slightly faster since there's no search. */
        void AddBack(item::CItem^ item);

        /* Sort items by vnum - they are NOT sorted by default. */
        void SortItems();

        /* Sets version */
        void SetVersion(uint32_t version);
        /* Sets the FourCC for the beginning of the file. */
        void SetFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D);
        /* Sets the FourCC for he THeader structure. */
        void SetHeaderFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D);
        /* Sets the FourCC for the beginning */
        void SetEncryptedFourCC(uint8_t A, uint8_t B, uint8_t C, uint8_t D);

    private:
        TError ProcessProto(std::ifstream& file);
        TError ReadItems(std::ifstream& file, uint32_t count, uint32_t size);

        TError Write(std::ofstream& file);

    private:
        cliext::vector<item::CItem^>^ m_itemVector;
        uint32_t m_version;
        uint32_t m_fourCC;
        uint32_t m_headerFourCC;
        uint32_t m_encryptedFourCC;
    };
}

#endif _ITEM_PROTO_LIB_H_
