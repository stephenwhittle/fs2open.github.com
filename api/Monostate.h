//could maybe make this the base class for all the interfaces and that way you simply need to use them, rather than having to edit this and the Interface class

class mono_nvi
{
public:
  explicit mono_nvi 
    ( std::shared_ptr< abstract_base > p ) 
  {
    if ( p == nullptr )
      throw ( something );
    if ( mp != nullptr )
      throw ( something_else );
    mp = p;
  }
  mono_nvi () 
  {
    if ( mp == nullptr )
      throw ( something );
  }
  inline void foo () 
  {
    mp->foo ();
  }
private:
  static std::shared_ptr< abstract_base > mp;
};
std::shared_ptr< abstract_base > mono_nvi::mp;
/*
My last article in Overload [Schmidt15] generated a response from Szymon Gatner. Szymon suggested that all of the functions duplicated from the abstract base class to the Mono NVI class could be replaced with an overloaded pointer operator.

The set of functions represented by foo() from listing 3 (page 11), implemented like this:

  inline void foo ()
  {
    mp->foo (); // call to the virtual function
                // in the concrete class
  }
could be replaced with one pointer operator overload function

  T* operator-> ()
  {
    return  mp.get ();
  }
and used something like this instead of the method used in Listings 5 and 11 (pages 11 and 13):

  typedef mono_nvi_template< abstract_base >
    mono_nvi;
  void nested_function ( void )
  {
    mono_nvi  mnvi;  // assumes its non-default
                 // constructor has 
    mnvi->foo (); // been called
  }
My first impression focused on the unusual way that the pointer operator was being used with a non-pointer variable. Szymon pointed out that “for classes of pointer semantics I think it is something C++ are used to by now. We already have unique_ptr and shared_ptr (and auto_ptr for a long time), also iterators have pointer semantics.”

I encouraged Szymon to write up his idea for Overload. (Fran is always looking for good material; sometimes she has to settle for mine.) He said he would be “happy if you would like to side-note it in your next article”. Here it is, Szymon. Thanks, Bob
*/