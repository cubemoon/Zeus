#ifndef ZEUS_BASE_BYTE_BUFFER_H_
#define ZEUS_BASE_BYTE_BUFFER_H_

#include "common.h"

class ByteBuffer
{
#define USING_BIG_ENDIAN

    const static uint32 EMPTY_BUFFER = 0x0000;
    const static uint32 DEFAULT_SIZE = 0x1000;

public:
    ByteBuffer()
        : _rpos(0), _wpos(0)
    {
        _buffer.reserve(EMPTY_BUFFER);
    }

    explicit ByteBuffer(size_t size)
        : _rpos(0), _wpos(0)
    {
        _buffer.reserve(size);
    }

    ByteBuffer(const char* src, size_t size)
        : _rpos(0), _wpos(0)
    {
        append(src, size);
    }

    ByteBuffer(const byte* src, size_t size)
        : _rpos(0), _wpos(0)
    {
        append(src, size);
    }

    explicit ByteBuffer(const ByteBuffer& buf) 
        : _rpos(buf._rpos), _wpos(buf._wpos), _buffer(buf._buffer) 
    {
    }

public:
    const byte* buffer() const
    {
        return &_buffer[0];
    }

    inline size_t size() const
    {
        return _buffer.size();
    }

    void clear()
    {
        _buffer.clear();
        _rpos = _wpos = 0;
    }

public:
    template <typename TYPE> void append(TYPE value)
    {
        append((const byte*)&value, sizeof(value));
    }

    void append(const std::string& value)
    {
        append((const byte*)value.c_str(), value.size() + 1);
    }

    void append(const char* value, size_t size)
    {
        append((const byte*)value, size);
    }

    void append(const byte* value, size_t size)
    {
        if (size == 0) return;
        size_t wsize = _wpos + size;

        //buffer空间不够则扩大
        if (this->size() < wsize)
            _buffer.resize(wsize);

        memcpy(&_buffer[_wpos], value, size);
        _wpos += size;
    }

    void append(const ByteBuffer& value)
    {
        if (value.size() > 0)
        {
            append(value.buffer(), value.size());
        }
    }

public:
    template <typename TYPE> void put(size_t pos, TYPE value)
    {
        put(pos, (const byte*)&value, sizeof(value));
    }

    void put(size_t pos, const byte* src, size_t size)
    {
        assert(pos + size <= this->size());
        memcpy(&_buffer[pos], src, size);
    }

public:
    template <typename T> T read()
    {
        T r = read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }

    template <typename T> T read(size_t pos) const
    {
        if(pos + sizeof(T) > size())
        {
            return (T)0;
        } 
        else
        {
            return *((T*)&_buffer[pos]);
        }
    }

    void read(byte* dest, size_t len)
    {
        if (_rpos + len <= size())
        {
            memcpy(dest, &_buffer[_rpos], len);
        }
        else
        {
            throw "德玛西亚人永不退缩！";
            memset(dest, 0, len);
        }
        _rpos += len;
    }

public:
	ByteBuffer& operator<<(bool value)
    {
		append<char>((char)value);
		return *this;
	}

	ByteBuffer& operator<<(uint8 value)
    {
		append<uint8>(value);
		return *this;
	}

	ByteBuffer& operator<<(uint16 value)
    {
#ifdef USING_BIG_ENDIAN
		append<uint16>(swap16(value));
#else
		append<uint16>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(uint32 value)
    {
#ifdef USING_BIG_ENDIAN
		append<uint32>(swap32(value));
#else
		append<uint32>(value);
#endif
		return *this;
	}

	ByteBuffer& operator<<(uint64 value)
    {
#ifdef USING_BIG_ENDIAN
		append<uint64>(swap64(value));
#else
		append<uint64>(value);
#endif
		return *this;
	}

	ByteBuffer& operator<<(int8 value)
    {
		append<int8>(value);
		return *this;
	}

	ByteBuffer& operator<<(int16 value)
    {
#ifdef USING_BIG_ENDIAN
		append<int16>(swap16(value));
#else
		append<int16>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(int32 value)
    {
#ifdef USING_BIG_ENDIAN
		append<int32>(swap32(value));
#else
		append<int32>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(int64 value)
    {
#ifdef USING_BIG_ENDIAN
		append<int64>(swap64(value));
#else
		append<int64>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(float value)
    {
#ifdef USING_BIG_ENDIAN
		append<float>(swapfloat(value));
#else
		append<float>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(double value)
    {
#ifdef USING_BIG_ENDIAN
		append<double>(swapdouble(value));
#else
		append<double>(value);
#endif
		return *this;
	}

	ByteBuffer &operator<<(const std::string& value)
    {
		append((const byte*)value.c_str(), value.length());
		append((byte)0);
		return *this;
	}

	ByteBuffer &operator<<(const char* str)
    {
		append((const byte*)str, strlen(str));
		append((byte)0);
		return *this;
	}

	ByteBuffer &operator>>(bool& value)
    {
		value = read<char>() > 0 ? true : false;
		return *this;
	}

    ByteBuffer &operator>>(uint8& value)
    {
		value = read<uint8>();
		return *this;
	}

	ByteBuffer &operator>>(uint16& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap16(read<uint16>());
#else
		value = read<uint16>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(uint32& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap32(read<uint32>());
#else
		value = read<uint32>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(uint64& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap64(read<uint64>());
#else
		value = read<uint64>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(int8& value)
    {
		value = read<int8>();
		return *this;
	}

	ByteBuffer &operator>>(int16& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap16(read<int16>());
#else
		value = read<int16>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(int32& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap32(read<int32>());
#else
		value = read<int32>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(int64& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swap64(read<uint64>());
#else
		value = read<int64>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(float& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swapfloat(read<float>());
#else
		value = read<float>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(double& value)
    {
#ifdef USING_BIG_ENDIAN
		value = swapdouble(read<double>());
#else
		value = read<double>();
#endif
		return *this;
	}

	ByteBuffer &operator>>(std::string& value)
    {
		value.clear();
		while (true)
        {
			char c = read<char>();
			if (c == 0)
				break;

			value += c;
		}

		return *this;
	}

public:
    void resize(size_t new_size)
    {
        _buffer.resize(new_size);
        _rpos = 0;
        _wpos = size();
    }

    void reserve(size_t reserve_size)
    {
        if (reserve_size > size())
            _buffer.reserve(reserve_size);
    };

protected:
    std::vector<byte> _buffer;
    size_t _rpos, _wpos;
};

#endif