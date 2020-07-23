#include <iostream>
class SCPMembuf : public std::streambuf
{
  public:
	using byte = char;
	SCPMembuf() : begin_(nullptr), end_(nullptr), current_(nullptr){};

	SCPMembuf(const byte* Data, std::uintmax_t Length) : begin_(Data), end_(Data + Length), current_(Data){};

	SCPMembuf(const byte* Begin, const byte* End) : begin_(Begin), end_(End), current_(Begin){};

	SCPMembuf(const SCPMembuf& Other) : begin_(Other.begin_), end_(Other.end_), current_(Other.current_) {};

	SCPMembuf(const SCPMembuf&& Other) noexcept : begin_(Other.begin_), end_(Other.end_), current_(Other.current_) {};

	SCPMembuf& operator= (const SCPMembuf& Other)
	{
		SCPMembuf TmpBuf(Other);
		std::swap(begin_, TmpBuf.begin_);
		std::swap(end_, TmpBuf.end_);
		std::swap(current_, TmpBuf.current_);

	}
	SCPMembuf& operator= (SCPMembuf&& Other)
	{
		std::swap(begin_, Other.begin_);
		std::swap(end_, Other.end_);
		std::swap(current_, Other.current_);
	}

  protected:
	int_type underflow() override
	{
		return (current_ == end_ ? traits_type::eof() : traits_type::to_int_type(*current_));
	}

	int_type uflow() override
	{
		return (current_ == end_ ? traits_type::eof() : traits_type::to_int_type(*current_++));
	}

	int_type pbackfail(int_type ch) override
	{
		if (current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1])) {
			return traits_type::eof();
		}
		return traits_type::to_int_type(*--current_);
	}
	std::streamsize showmanyc() override { return end_ - current_; }
	const byte* const begin_;
	const byte* const end_;
	const byte* current_;
};
