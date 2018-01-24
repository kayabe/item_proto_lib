#ifndef _BUFFER_H_
#define _BUFFER_H_

namespace item_proto_lib
{
    public ref class CBuffer
    {
    public:
        CBuffer(uint32_t size) : m_size(size), m_dataSize(0)
        {
            m_buffer = new char[size];
            memset(m_buffer, 0, size);
        }

        ~CBuffer()
        {
            delete[] m_buffer;
        }

        template <typename TYPE>
        bool Write(const TYPE& data)
        {
            bool bRet = false;

            if (m_dataSize + sizeof(TYPE) <= m_size)
            {
                bRet = true;

                memcpy(m_buffer + m_dataSize, &data, sizeof(TYPE));
                m_dataSize += sizeof(TYPE);
            }

            return bRet;
        }

        /* Can't pass by (const) reference managed class member variables */
        bool Write(uint32_t data)
        {
            return Write(static_cast<int32_t>(data));
        }

        char* GetPtr()
        {
            return m_buffer;
        }

        char* GetWritePtr()
        {
            return m_buffer + m_dataSize;
        }

        uint32_t GetBufferSize()
        {
            return m_size;
        }

        uint32_t GetDataSize()
        {
            return m_dataSize;
        }

    private:
        char * m_buffer;
        uint32_t m_size;
        uint32_t m_dataSize;
    };
}

#endif
