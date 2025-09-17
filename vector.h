/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T, typename A = std::allocator<T>>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   //
   // Construct
   //
   vector(const A & a = A());
   vector(size_t numElements,                const A & a = A());
   vector(size_t numElements, const T & t,   const A & a = A());
   vector(const std::initializer_list<T>& l, const A & a = A());
   vector(const vector &  rhs);
   vector(      vector && rhs);
  ~vector();

   //
   // Assign
   //
   void swap(vector& rhs)
   {
      // Swap the array
      auto tempPtr = data;
      data = rhs.data;
      rhs.data = tempPtr;
      
      // Swap the capacity
      auto tempCap = numCapacity;
      numCapacity = rhs.numCapacity;
      rhs.numCapacity = tempCap;
      
      // Swap the number of elements
      auto tempSize = numElements;
      numElements = rhs.numElements;
      rhs.numElements = tempSize;
   }
   vector & operator = (const vector & rhs);
   vector & operator = (vector&& rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(data);
   }
   iterator end() 
   { 
      return iterator(data+numElements);
   }

   //
   // Access
   //
         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //
   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //
   void clear()
   {
      // Destroys all the elements
      for (int i = 0; i < numElements; i++)
         alloc.destroy(data + i);
      numElements = 0;
   }
   void pop_back()
   {
      if (numElements == 0)
         return;
      alloc.destroy(data+(numElements--)-1);
   }
   void shrink_to_fit();

   //
   // Status
   //
   size_t  size()          const { return numElements   ;}
   size_t  capacity()      const { return numCapacity   ;}
   bool empty()            const { return numElements==0;}
  
private:
   
   A    alloc;                // use allocator for memory allocation
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class vector <T, A> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           { p = nullptr;        }
   iterator(T* p)                       { this->p = p;        }
   iterator(const iterator& rhs)        { p = rhs.p;          }
   iterator(size_t index, vector<T>& v) { p = v.data + index; }
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return p != rhs.p; }
   bool operator == (const iterator& rhs) const { return p == rhs.p; }

   // dereference operator
   T& operator * ()
   {
      return *p;
   }

   // prefix increment
   iterator& operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp(p);
      p++;
      return temp;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator temp(p);
      p--;
      return temp;
   }

private:
   T* p;
};


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const A & a)
{
   data = nullptr;
   numElements = 0;
   numCapacity = 0;
}


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const T & t, const A & a):vector(a)
{
   resize(num, t);
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const std::initializer_list<T> & l, const A & a) 
{
   // Allocate the memory, set the size, capacity
   numElements = l.size();
   numCapacity = l.size();
   data = alloc.allocate(numCapacity); // Use numCapacity for allocation

   // Copy the data from the list
   int i = 0;
   for (const T& item : l)
   {
      // Use the allocator's construct method to properly copy construct
      alloc.construct(data + i, item);
      i++;
   }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const A & a):vector(a)
{
   resize(num);
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (const vector & rhs):vector(alloc)
{
   *this = rhs;
}
   
/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (vector && rhs):vector(alloc)
{
   *this = std::move(rhs);
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T, typename A>
vector <T, A> :: ~vector()
{
   // Destroy all the data
   for(int i = 0; i < numElements; i++)
   {
      alloc.destroy(data + i);
   }
   
   // Deallocate the memory
   alloc.deallocate(data,numCapacity);
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements)
{
   if (newElements < numElements)      // Shrinks to the new size
      for (int i = newElements; i < numElements; i++)
         alloc.destroy(data+i);
   else if (newElements > numElements) // Increases size
   {
      // Reserve more capacity if needed
      if (newElements > numCapacity)
         reserve(newElements);
      for (int i = numElements; i < newElements; i++)
         alloc.construct(data+i);
   }
   
   // Set the new size
   numElements=newElements;
}

template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements, const T & t)
{
   if (newElements < numElements)      // Shrinks to the new size
      for (int i = newElements; i < numElements; i++)
         alloc.destroy(data+i);
   else if (newElements > numElements) // Increases size
   {
      // Reserve more capacity if needed
      if (newElements > numCapacity)
         reserve(newElements);
      for (int i = numElements; i < newElements; i++)
         alloc.construct(data+i,t);
   }
   
   // Set the new size
   numElements=newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: reserve(size_t newCapacity)
{
   // Do nothing if newCapacity is smaller than current capacity
   if (newCapacity <= numCapacity)
      return;
   
   // Allocate memory for increased capacity
   T * dataNew = alloc.allocate(newCapacity);
   
   // Moves the data from the old array to the newly allocated one
   for (int i = 0; i < numElements; i++)
   {
      new ((void*)(dataNew + i)) T(std::move(data[i]));
      alloc.destroy(data+i);
   }
   
   // Deallocate old array
   alloc.deallocate(data,numCapacity);

   // Set the attributes
   data = dataNew;
   numCapacity = newCapacity;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: shrink_to_fit()
{
   // Do nothing if there is no extra capacity
   if (numElements == numCapacity)
      return;
   
   T * dataNew;
   
   // If there is no elements dont allocate memory
   if (numElements == 0)
   {
      dataNew = nullptr;
   }
   else
      dataNew = alloc.allocate(numElements);
   
   // Copy the old arrays elements to the new
   for (int i = 0; i < numElements; i++)
   {
      new ((void*)(dataNew + i)) T((data[i]));
      alloc.destroy(data + i);
   }
   
   // Deallocate and reassign member variables
   alloc.deallocate(data,numCapacity);
   data = dataNew;
   numCapacity = numElements;
}

/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: operator [] (size_t index)
{
   return data[index];
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: operator [] (size_t index) const
{
   return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: front ()
{
   return data[0];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: front () const
{
   return data[0];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: back()
{
   return data[numElements-1];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: back() const
{
   return data[numElements-1];
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
void vector <T, A> :: push_back (const T & t)
{
   // If empty reserve 1 capacity, otherwise double the capacity
   if (numElements == numCapacity)
      reserve(empty() ? 1 : numCapacity * 2);
   new ((void*)(data + numElements)) T(std::move(t));
   numElements++;
}

template <typename T, typename A>
void vector <T, A> ::push_back(T && t)
{
   // If empty reserve 1 capacity, otherwise double the capacity
   if (numElements == numCapacity)
      reserve(empty() ? 1 : numCapacity * 2);
   new ((void*)(data + numElements)) T(std::move(t));
   numElements++;
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
vector <T, A> & vector <T, A> :: operator = (const vector & rhs)
{
   // If vectors size is the same just copy all of its elements
   if (rhs.size() == size())
      for (int i = 0; i < size(); i++)
         data[i] = rhs.data[i];
   
   // If rhs is bigger
   else if (rhs.size() > size())
   {
      // Reallocate if lhs capacity is smaller than rhs size
      if (rhs.size() <= numCapacity)
      {
         for (int i = 0; i < size(); i++)
            data[i] = rhs.data[i];
         for (int i = size(); i < rhs.size(); i++)
            alloc.construct(data+i, rhs.data[i]);
      }
      
      // If lhs is smaller, reallocate
      else
      {
         T * dataNew = alloc.allocate(rhs.size());
         for (int i = 0; i < rhs.size(); i++)
            alloc.construct(dataNew+i, rhs.data[i]);
         clear();
         alloc.deallocate(data,numCapacity);
         data = dataNew;
         numCapacity = rhs.size();
      }
   }
   
   // If lhs is bigger
   else
   {
      for (int i = 0; i < rhs.size(); i++)
         data[i] = rhs.data[i];
      for (int i = rhs.size(); i < size(); i++)
         alloc.destroy(data+i);
   }
   numElements = rhs.size();
   return *this;
}
template <typename T, typename A>
vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
{
   swap(rhs);
   
   // Destroy rhs if it has capacity
   if (rhs.numCapacity)
   {
      rhs.clear();
      rhs.shrink_to_fit();
   }
   return *this;
}




} // namespace custom
