#include <deque>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

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