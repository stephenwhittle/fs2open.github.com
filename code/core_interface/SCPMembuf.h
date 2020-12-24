#include <iosfwd>
#include <iostream>
class SCPMembuf : public std::streambuf
{
public:
	using byte = char;
	SCPMembuf() {};

	SCPMembuf(const byte* Data, std::uintmax_t Length) 
	{
		setg(const_cast<byte*>(Data), const_cast<byte*>(Data), const_cast<byte*>(Data + Length));
	};

	SCPMembuf(const byte* Begin, const byte* End)
	{
		setg(const_cast<byte*>(Begin), const_cast<byte*>(Begin), const_cast<byte*>(End));
	};

	SCPMembuf(const SCPMembuf& Other) : std::streambuf(Other){};

	SCPMembuf(const SCPMembuf&& Other) noexcept : std::streambuf(Other) {};

	SCPMembuf& operator=(const SCPMembuf& Other)
	{
		SCPMembuf TmpBuf(Other);
		std::swap(*this, TmpBuf);
		return *this;
	}
	SCPMembuf& operator=(SCPMembuf&& Other)
	{
		std::swap(*this, Other);
		return *this;
	}

protected:
	
	pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
	{
		if (which != std::ios_base::in)
		{
			throw std::invalid_argument("basic_memstreambuf::seekoff[which]");
		}

		if (dir == std::ios_base::beg)
		{
			if (off >= 0 && off < egptr() - eback())
			{
				setg(eback(), eback() + off, egptr());
			}
			else
			{
				throw std::out_of_range("basic_memstreambuf::seekoff[beg]");
			}
		}
		else if (dir == std::ios_base::cur)
		{
			if ((off >= 0 && off < egptr() - gptr()) || (off < 0 && std::abs(off) < gptr() - eback()))
			{
				gbump((int)off);
			}
			else if (off) //handle offset of 0
			{
				throw std::out_of_range("basic_memstreambuf::seekoff[cur]");
			}
		}
		else if (dir == std::ios_base::end)
		{
			if (off <= 0 && std::abs(off) < egptr() - eback())
			{
				setg(eback(), egptr() + off, egptr());
			}
			else
			{
				throw std::out_of_range("basic_memstreambuf::seekoff[end]");
			}
		}
		else
		{
			throw std::invalid_argument("basic_memstreambuf::seekoff[dir]");
		}

		return gptr() - eback();
	}
	virtual pos_type seekpos(pos_type pos, std::ios_base::openmode which = std::ios_base::in) override
	{
		if (which != std::ios_base::in)
		{
			throw std::invalid_argument("basic_memstreambuf::seekpos[which]");
		}

		if (pos < egptr() - eback())
		{
			setg(eback(), eback() + pos, egptr());
		}
		else if ((eback() + pos) >= egptr())
		{
			setg(eback(), egptr(), egptr());
		}

		return pos;
	}
	virtual std::streamsize showmanyc() override
	{
		const auto* ptr = gptr();
		const auto* end = egptr();

		return (ptr <= end) ? (end - ptr) : 0;
	}

	virtual int_type underflow() override
	{
		const auto* ptr = gptr();

		if (ptr >= egptr())
		{
			return traits_type::eof();
		}

		return traits_type::to_int_type(*ptr);
	}
	virtual std::streamsize xsgetn(char_type* s, std::streamsize count) override
	{
		if (count == 0)
			return 0;

		const auto* ptr = gptr();
		const auto to_read = std::min(count, static_cast<std::streamsize>(egptr() - ptr));

		if (to_read == 0)
		{
			return traits_type::eof();
		}
		else
		{
			std::memcpy(s, ptr, to_read);
			gbump(to_read);
			return to_read;
		}
	}
};
