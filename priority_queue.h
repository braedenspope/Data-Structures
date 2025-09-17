/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h" // for default underlying container

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T, class Container = custom::vector<T>, class Compare = std::less<T>>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT, class CContainer, class CCompare>
   friend void swap(priority_queue<TT, CContainer, CCompare>& lhs, priority_queue<TT, CContainer, CCompare>& rhs);

public:

   //
   // construct
   //
   priority_queue(const Compare & c = Compare())
   {
   }
   priority_queue(const priority_queue &  rhs, const Compare & c = Compare()):container(rhs.container),compare(c)
   {
   }
   priority_queue(priority_queue && rhs, const Compare & c = Compare()):container(std::move(rhs.container)),compare(std::move(c))
   {
   }
   template <class Iterator>
   priority_queue(Iterator first, Iterator last, const Compare & c = Compare()) 
   {
      container.reserve(last-first);
      for (;first != last; push(*(first++)));
   }
   explicit priority_queue (const Compare& c, Container && rhs):compare(c),container(std::move(rhs))
   {
      heapify();
   }
   explicit priority_queue (const Compare& c, Container & rhs):compare(c),container(rhs)
   {
      heapify();
   }
  ~priority_queue() 
   {
   }

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
      return container.size();
   }
   bool empty() const 
   { 
      return container.empty();
   }
   
private:

   void heapify();                            // convert the container in to a heap
   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   Container container;       // underlying container (probably a vector)
   Compare   compare;         // comparision operator

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T, class Container, class Compare>
const T & priority_queue <T, Container, Compare> :: top() const
{
   if (empty())
      throw std::out_of_range(std::string("std:out_of_range"));
   return container[0];
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: pop()
{
   using std::swap;
   if (size() == 0)
      return;
   swap(container[0],container[size()-1]);
   container.pop_back();
   percolateDown(1);
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: push(const T & t)
{
   container.push_back(t);
   size_t index = size() / 2;
   while (index && percolateDown(index))
      index /= 2;
}
template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: push(T && t)
{
   container.push_back(std::move(t));
   size_t index = size() / 2;
   while (index && percolateDown(index))
      index /= 2;
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T, class Container, class Compare>
bool priority_queue <T, Container, Compare> :: percolateDown(size_t indexHeap)
{
   using std::swap;
   size_t ihLeft = indexHeap * 2;      // Left child heap index
   size_t ihRight = indexHeap * 2 + 1; // Right child heap index
   size_t iaLeft = indexHeap * 2 -1;   // Left child array index
   size_t iaRight = indexHeap * 2;     // Right child array index
   
   // If there are no children, return false
   if (ihLeft > size())
      return false;
   
   size_t indexBigger;

   // If the right child exists and is greater than the left, update indexBigger
   if (ihRight <= size() && compare(container[iaLeft], container[iaRight]))
   {
      indexBigger = iaRight;
   }
   else
      indexBigger = iaLeft;
      
   size_t parentAI = indexHeap - 1;
   if (compare(container[parentAI], container[indexBigger]))
   {
      swap(container[parentAI], container[indexBigger]);
      percolateDown(indexBigger+1);
      return true;
   }
   return false;
}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> ::heapify()
{
   for (size_t i = size()/2; i > 0; percolateDown(i--));
}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T, class Container, class Compare>
inline void swap(custom::priority_queue <T, Container, Compare> & lhs,
                 custom::priority_queue <T, Container, Compare> & rhs)
{
   std::swap(lhs.container, rhs.container);
   std::swap(lhs.compare, rhs.compare);
}
   
};
