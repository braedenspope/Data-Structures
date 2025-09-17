/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Tyler and Braedon
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A & a = A()) 
   { 
      data = nullptr;
      numCells = 16;
      numElements = 0;
      numBlocks = 0;
      iaFront = 0;
   }
   deque(deque & rhs);
   ~deque()
   {
      clear();
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(0,this);
   }
   iterator end()   
   { 
      return iterator(numElements, this);
   }

   // 
   // Access
   //
   T & front()       
   { 
      return (*this)[0];
   }
   const T & front() const 
   {
      return (*this)[0];
   }
   T & back()
   {
      return (*this)[numElements-1];
   }
   const T & back() const
   {
      return (*this)[numElements-1];
   }
   T & operator[](int id)
   {
      return data[ibFromID(id)][icFromID(id)];
   }
   const T & operator[](int id) const
   {
      return data[ibFromID(id)][icFromID(id)];
   }

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return numElements;      }
   bool   empty() const { return numElements == 0; }
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      return (id + iaFront) % (numCells * numBlocks);
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      return iaFromID(id) / numCells;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      return iaFromID(id) % numCells;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() 
   {
   }
   iterator(int id, deque* d):id(id),d(d)
   {
   }
   iterator(const iterator& rhs):id(rhs.id),d(rhs.d)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      id = rhs.id;
      d = rhs.d;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return id != rhs.id; }
   bool operator == (const iterator& rhs) const { return id == rhs.id; }

   // 
   // Access
   //
   T& operator * ()
   {
      return (*d)[id];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return id - it.id;
   }
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      id++;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator itReturn = *this;
      id++;
      return itReturn;
   }
   iterator& operator -- ()
   {
      id--;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator itReturn = *this;
      id--;
      return itReturn;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) 
{
   data = nullptr;
   numCells = 16;
   numElements = 0;
   numBlocks = 0;
   iaFront = 0;
   *this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   auto itLHS = begin();
   auto itRHS = rhs.begin();
   
   // Copy elements into existing slots in the deque
   while (itLHS != end() && itRHS != rhs.end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }
   
   // Remove excess items
   for (; end() != itLHS; pop_back());

   // Push back remaining items from source
   while (itRHS != rhs.end())
   {
      push_back(*itRHS);
      ++itRHS;
   }
   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
   // Reallocate the array of blocks as needed
   if (numElements == numBlocks * numCells || (ibFromID(numElements) == ibFromID(0) && icFromID(numElements) < icFromID(0)))
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   // Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // Assign the value into the block
   new((void*)(&(data[ib][icFromID(numElements)]))) T(t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
   // Reallocate the array of blocks as needed
   if (numElements == numBlocks * numCells || (ibFromID(numElements) == ibFromID(0) && icFromID(numElements) < icFromID(0)))
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   // Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // Assign the value into the block
   new((void*)(&(data[ib][icFromID(numElements)]))) T(std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   // Find the array index that is in front of iaFront
   int nIA = (iaFront == 0 ? numCells * numBlocks -1 : iaFront -1);
   
   // Reallocate the array of blocks as needed, if the nNIA is in same block as the back then reallocate
   if (numElements == numBlocks * numCells || ibFromID(numElements) == nIA / numCells)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   // Adjust the front array index, wrapping as needed
   iaFront = (iaFront == 0 ? numCells * numBlocks -1 : iaFront -1);
   
   // Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // Assign the value into the block
   new((void*)(&(data[ib][icFromID(0)]))) T(t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
   // Find the array index that is in front of iaFront
   int nIA = (iaFront == 0 ? numCells * numBlocks -1 : iaFront -1);
   
   // Reallocate the array of blocks as needed, if the nNIA is in same block as the back then reallocate
   if (numElements == numBlocks * numCells || ibFromID(numElements) == nIA / numCells)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   // Adjust the front array index, wrapping as needed
   iaFront = (iaFront == 0 ? numCells * numBlocks -1 : iaFront -1);
   
   // Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // Assign the value into the block
   new((void*)(&(data[ib][icFromID(0)]))) T(std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
   // Delete the elements
   for (int id = 0; id < numElements; id++)
      alloc.destroy(&data[ibFromID(id)][icFromID(id)]);
   
   // Delete the blocks themselves
   for (int ib = 0; ib < numBlocks; ib++)
   {
      if (data[ib] != nullptr)
      {
         alloc.deallocate(data[ib], numCells);
         data[ib] = nullptr;
      }
   }
   
   // Set member variables
   numElements = 0;
   iaFront = 0;
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
   // If none return
   if (numElements == 0)
      return;
   
   int idRemove = 0;
   
   // Call the destructor on the front element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);
   
   
   // Delete the block as needed
   if (numElements == 1 || (icFromID(idRemove) == numCells-1 && ibFromID(idRemove) != ibFromID(numElements)))
   {
      alloc.deallocate(data[ibFromID(idRemove)], numCells);
      data[ibFromID(idRemove)] = nullptr;
   }
   // Find the next iaFront
   ++iaFront %= numCells * numBlocks;
   numElements--;
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   // If none return
   if (numElements == 0)
      return;
   
   int idRemove = numElements -1;
   
   // Call the destructor on the back element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);
   
   // Delete the block as needed
   if (numElements == 1 || (icFromID(idRemove) == 0 && ibFromID(idRemove) != ibFromID(0)))
   {
      alloc.deallocate(data[ibFromID(idRemove)], numCells);
      data[ibFromID(idRemove)] = nullptr;
   }
   
   numElements--;
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   // Allocate a new array of pointers that is the requested size
   T** dataNew = new T*[numBlocksNew];
   
   // Copy over the pointers, unwrapping as we go
   int ibNew = 0;
   for (int idOld = 0; idOld < numElements; idOld += numCells)
   {
      dataNew[ibNew] = data[ibFromID(idOld)];
      ibNew++;
   }
   
   // Set all the block pointers to null when there are no blocks to point to
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }
   
   // If the back element is in the front element's block, then move it
   if (numElements > 0 &&
       ibFromID(0) == ibFromID(numElements - 1) &&
       icFromID(0) >  icFromID(numElements - 1))
   {
      size_t ibBackOld = ibFromID(numElements - 1);
      size_t ibBackNew = numElements / numCells;
      dataNew[ibBackNew] = new T[numCells];
      for (size_t ic = 0; ic < icFromID(numElements-1); ic++)
      {
         new ((void*)(&(dataNew[ibBackNew][ic]))) T(std::move(data[ibBackOld][ic]));
      }
   }
   
   // Change the deque's member variables with the new values
   if (data)
      delete data;
   data = dataNew;
   numBlocks = numBlocksNew;
   iaFront = iaFront % numCells;
}




} // namespace custom
