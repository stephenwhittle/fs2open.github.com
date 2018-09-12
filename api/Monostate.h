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