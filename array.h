/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Braeden Pope
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() 
   { 
      return iterator(&__elems_[0]);
   }
   iterator end()   
   { 
      return iterator(&__elems_[N]);
   }

   // 
   // Access
   //

   // Subscript
   T& operator [] (size_t index)       
   { 
      return __elems_[index];
   }
   const T& operator [] (size_t index) const 
   { 
      return __elems_[index];
   }

   // At
   T& at(size_t index)  
   { 
      if (index <= N)
         return __elems_[index];
      throw std::out_of_range("Out of Range");
   }
   const T& at(size_t index) const 
   { 
      if (index <= N)
         return __elems_[index];
      throw std::out_of_range("Out of Range");
      
   }

   // Front and Back
   T& front()       
   { 
      return __elems_[0];
   }
   T& back()        
   { 
      return __elems_[N - 1];
   }
   const T& front() const 
   { 
      return __elems_[0];
   }
   const T& back()  const 
   { 
      return __elems_[N - 1];
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const 
   { 
      return N;           
   }
   bool empty() const 
   { 
      return N == 0;
   }
   
   //
   // Member Variable
   //

   // statically allocated array of T
   T __elems_[N];    
};



/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator
{
   friend class ::TestArray; // give unit tests access to the privates
public:
   // constructors, destructors, and assignment operator
   iterator()      
   { 
      p = nullptr;
   }
   iterator(T * p)  
   {
      this->p = p;
   }
   iterator(const iterator & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return p != rhs.p;
   }
   bool operator == (const iterator & rhs) const 
   { 
      return p == rhs.p;
   }
   
   // dereference operator
   T & operator * ()       
   { 
      return *p;
   }
   const T & operator * () const 
   { 
      return *p;
   }
   
   // prefix increment
   iterator & operator ++ ()
   {
      ++p;
      return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++p;
      return temp;
   }
   
private:
   T * p;
};

}; // namespace custom
