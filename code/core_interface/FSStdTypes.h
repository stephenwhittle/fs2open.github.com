#pragma once 

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <array>

const size_t INVALID_SIZE = static_cast<size_t>(-1);

template< typename T >
using SCP_vector = std::vector< T, std::allocator< T > >;

template< typename T >
using SCP_list = std::list< T, std::allocator< T > >;

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SCP_string;

typedef std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > SCP_stringstream;

template< typename T, typename U >
using SCP_map = std::map<T, U, std::less<T>, std::allocator<std::pair<const T, U> > >;

template< typename T, typename U >
using SCP_multimap = std::multimap<T, U, std::less<T>, std::allocator<std::pair<const T, U> > >;

template< typename T >
using SCP_queue = std::queue< T, std::deque< T, std::allocator< T > > >;

template< typename T >
using SCP_deque = std::deque< T, std::allocator< T > >;

template <typename T, typename... Args>
std::array<T, sizeof...(Args)> make_array(Args&&... refs)
{
	return {std::forward<T>(refs)...};
}

class SCP_buffer
{
private:
	std::unique_ptr<char[]> InternalData;
public:
	using iterator = char*;
	using const_iterator = const char*;

	SCP_buffer(size_t Size)
		:Size(Size)
	{
		InternalData = std::make_unique<char[]>(Size);
	}

	SCP_buffer(SCP_buffer&& Source) noexcept
		:Size(Source.Size),
		InternalData(std::move(Source.InternalData)) {};
	
	SCP_buffer(const SCP_buffer&) = delete;
	SCP_buffer& operator=(const SCP_buffer&) = delete;
	SCP_buffer& operator=(SCP_buffer&& Source) noexcept
	{
		if (this != &Source)
		{
			InternalData = std::move(Source.InternalData);
			Size = std::move(Source.Size);
		}
		return *this;
	}
	size_t Size;
	
	SCP_buffer CopyRange(const_iterator Start, const_iterator End) 
	{
		SCP_buffer RangedCopy = SCP_buffer(End - Start);
		std::copy(Start, End, RangedCopy.begin());
		return RangedCopy;
	}

	SCP_buffer Clone() const
	{
		SCP_buffer MyClone = SCP_buffer(Size);
		std::copy(begin(), end(), MyClone.begin());
		return MyClone;
	}
	
	inline char* const Data() const { return InternalData.get(); }
	inline char* const begin() const { return InternalData.get(); }
	inline char* const end() const { return InternalData.get() + Size; }
	inline char& operator[](size_t Index) const { return InternalData[Index]; }
};

#if __cplusplus < 201402L
template <class T, bool>
struct enum_hasher_util {
	inline size_t operator()(const T& elem) { return std::hash<T>()(elem); }
};

template <class T>
struct enum_hasher_util<T, true> {
	inline size_t operator()(const T& elem)
	{
		typedef typename std::underlying_type<T>::type enumType;
		return std::hash<enumType>()(static_cast<enumType>(elem));
	}
};

/**
 * @brief An enum class compatible hashing class
 *
 * This is the same as std::hash except that is can handle enum classes properly using their underlying type. This is
 * only required for C++ < 14 since after that the standard mandates that enum classes should be hashable.
 *
 * @tparam T The type that should be hashed
 */
template <class T>
struct SCP_hash {
	inline size_t operator()(const T& elem) const { return enum_hasher_util<T, std::is_enum<T>::value>()(elem); }
};

#else
// No need for the special hash class so just use the standard hash
template <typename T>
using SCP_hash = std::hash<T>;
#endif

template <typename Key, typename T, typename Hash = SCP_hash<Key>, typename KeyEqual = std::equal_to<Key>>
using SCP_unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, std::allocator<std::pair<const Key, T>>>;