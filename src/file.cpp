#include <algorithm>

#include "file.h"

// Specifies the initial size of the readline buffer. The buffer is allocated on the heap. The size of
// the buffer grows per std:string allocation rules, which typically doubles the allocation size each
// time more memory is needed. This may be configured by the build system in case target platform is
// sensitive to heap size allocations.
#if !defined(FILE_READLINE_INITIAL_ALLOC_SIZE)
#   define FILE_READLINE_INITIAL_ALLOC_SIZE  512
#endif

// Maximum size of a line while calling file ReadLine. Any line longer than this is truncated and the
// remainder would be read in the next call to ReadLine. Note that the actual amount of memory used
// during ReadLine will likely be 1.5 to 2x larger than this value (varies depending on underlying
// std::string implementation).
#if !defined(FILE_READLINE_MAX_LENGTH)
#   define FILE_READLINE_MAX_LENGTH     8192
#endif


File::File() :
    AbstractFile(),
    m_stream(nullptr),
    m_fileSize(0)
{
}

File::~File()
{
    close();
}

bool File::open(const std::string& filename)
{
    close();

    m_stream = filestream_open(filename.c_str(), RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);

    if (!isOpen())
        return false;

    filestream_seek(m_stream, 0, SEEK_END);

    int64_t fileSize = filestream_tell(m_stream);
    if (fileSize <= 0)
    {
        close();
        return false;
    }

    m_fileSize = static_cast<size_t>(fileSize);

    filestream_seek(m_stream, 0, SEEK_SET);

    return true;
}

bool File::isOpen() const
{
    return (m_stream != nullptr);
}

bool File::isChd() const
{
    return false;
}

void File::close()
{
    if (!isOpen())
        return;

    filestream_close(m_stream);
    m_stream = nullptr;
    m_fileSize = 0;
}

size_t File::size() const
{
    return m_fileSize;
}

int64_t File::pos() const
{
    if (!isOpen())
        return 0;

    return filestream_tell(m_stream);
}

bool File::seek(size_t pos)
{
    if (!isOpen())
        return false;
    
    int64_t expectedPos = static_cast<int64_t>(pos);

    filestream_seek(m_stream, expectedPos, SEEK_SET);

    int64_t resultPos = filestream_tell(m_stream);

    return (expectedPos == resultPos);
}

bool File::skip(size_t off)
{
    return seek(static_cast<size_t>(pos()) + off);
}

bool File::eof() const
{
    if (!isOpen())
        return true;

    return filestream_eof(m_stream) ? true : false;
}

size_t File::readData(void* data, size_t size)
{
    if (!isOpen())
        return size_t(0);

    int64_t reallyRead = filestream_read(m_stream, data, static_cast<int64_t>(size));

    return static_cast<size_t>(reallyRead);
}

size_t File::readAudio(void* data, size_t size)
{
    return readData(data, size);
}

std::string File::readLine()
{
    std::string result;

    if (!isOpen())
        return result;

    result.reserve(FILE_READLINE_INITIAL_ALLOC_SIZE);
   
    int c;
    int len = FILE_READLINE_MAX_LENGTH - 1;

    for(len--; len > 0; len--)
    {
        do
        {
            c = filestream_getc(m_stream);
        } while(c == '\r');
                   
        if ((c == EOF) || (c == '\n'))
            break;

        result += c;
    }

    return result;
}
