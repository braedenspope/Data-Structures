/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Tyler and Braedon
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   
   //
   // Construct
   //
   
   list(const A& a = A())  
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }
   list(list <T, A> & rhs, const A& a = A()):list()
   {
      *this = rhs;
   }
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A());
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A()):list()
   {
      *this = il;
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A()):list()
   {
      while(first != last)
         push_back(*(first++));
   }
   ~list()
   { 
      clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);
   void swap(list <T, A>& rhs);

   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements     ; }

private:
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //
   Node()
   {
      pNext = pPrev = nullptr;
   }
   Node(const T& data):data(data)
   {
      pNext = pPrev = nullptr;
   }
   Node(T&& data):data(std::move(data))
   {
      pNext = pPrev = nullptr;
   }

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT, typename AA>
   friend class custom::list;
   
public:
   // constructors, destructors, and assignment operator
   iterator()  
   {
      p = nullptr;
   }
   iterator(Node * pRHS)
   {
      p = pRHS;
   }
   iterator(const iterator  & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return p == rhs.p; }
   bool operator != (const iterator & rhs) const { return p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp(p);
      p = p->pNext;
      return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator temp(p);
      p = p->pPrev;
      return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      p = p->pPrev;
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T, A> :: insert(iterator it, const T &  data);
   friend iterator list <T, A> :: insert(iterator it,       T && data);
   friend iterator list <T, A> :: erase(const iterator & it);

private:

   typename list <T, A> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a):list()
{
   // Create nodes for the given size
   for (int i = 0; i < num; i++)
   {
      
      // Make node the head/tail if there is none
      Node* pNew = new Node(t);
      if (!pHead)
      {
         pHead = pTail = pNew;
      }
      
      // Hook the node up to existing and set the tail
      else
      {
         pNew->pPrev=pTail;
         pTail->pNext = pNew;
         pTail = pNew;
      }
      numElements++;
   }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a):list()
{
   // Create nodes for the given size
   for (int i = 0; i < num; i++)
   {
      Node* pNew = new Node();
      
      // Make node the head/tail if there is none
      if (!pHead)
      {
         pHead = pTail = pNew;
      }
      
      // Hook the node up to existing and set the tail
      else
      {
         pNew->pPrev=pTail;
         pTail->pNext = pNew;
         pTail = pNew;
      }
      numElements++;
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}


/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A> & list <T, A> :: operator = (list <T, A> & rhs)
{
   iterator itRHS = rhs.begin();
   iterator itLHS = begin();
   
   // Fill the existing nodes
   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }
      
   // Add new nodes
   if (itRHS != rhs.end())
   {
      while (itRHS != rhs.end())
      {
         push_back(*itRHS);
         ++itRHS;
      }
   }
   
   // Remove all nodes if rhs empty
   else if (rhs.empty())
      clear();
   
   // Remove extra nodes
   else if (itLHS != end())
   {
      Node* p = itLHS.p;
      pTail = p->pPrev;
      Node* pNext = p->pNext;
      while (p != nullptr)
      {
         pNext = p->pNext;
         delete p;
         p = pNext;
         numElements--;
      }
      pTail->pNext = nullptr;
   }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{
   const T * itRHS = rhs.begin();
   iterator itLHS = begin();
   
   // Fill the existing nodes
   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }
      
   // Add new nodes
   if (itRHS != rhs.end())
   {
      while (itRHS != rhs.end())
      {
         push_back(*itRHS);
         ++itRHS;
      }
   }
   
   // Remove all nodes if rhs empty
   else if (rhs.size() ==0)
      clear();
   
   // Remove extra nodes
   else if (itLHS != end())
   {
      Node* p = itLHS.p;
      pTail = p->pPrev;
      Node* pNext = p->pNext;
      while (p != nullptr)
      {
         pNext = p->pNext;
         delete p;
         p = pNext;
         numElements--;
      }
      pTail->pNext = nullptr;
   }
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
   Node* p = pHead;
   
   // Delete all nodes
   while (p)
   {
      Node* pNext = p->pNext;
      delete p;
      p = pNext;
   }
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_back(const T & data)
{
   list<T, A>::Node* pNew = new list<T, A>::Node(data);
   
   // Hook set new nodes prev.
   pNew->pPrev = pTail;
   
   // If there is a tail set its next to the new node.
   if (pTail)
      pTail->pNext = pNew;
   
   // If pNew is alone it is the head.
   else
      pHead = pNew;
   
   // Set the tail
   pTail = pNew;
   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{
   list<T, A>::Node* pNew = new list<T, A>::Node(std::move(data));
   
   // Hook set new nodes prev.
   pNew->pPrev = pTail;
   
   // If there is a tail set its next to the new node.
   if (pTail)
      pTail->pNext = pNew;
   
   // If pNew is alone it is the head.
   else
      pHead = pNew;
   
   // Set the tail
   pTail = pNew;
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list<T, A>::push_front(const T& data)
{
   list<T, A>::Node* pNew = new list<T, A>::Node(data);
   
   // Set pNews next pointer.
   pNew->pNext = pHead;
   
   // If theres an existing head attach new to it.
   if (pHead)
      pHead->pPrev = pNew;
   
   // If alone set the tail.
   else
      pTail = pNew;
   
   // Set the head.
   pHead = pNew;
   numElements++;
}

template <typename T, typename A>
void list<T, A>::push_front(T&& data) 
{
   list<T, A>::Node* pNew = new list<T, A>::Node(std::move(data));
   
   // Set pNews next pointer.
   pNew->pNext = pHead;
   
   // If theres an existing head attach new to it.
   if (pHead)
      pHead->pPrev = pNew;
   
   // If alone set the tail.
   else
      pTail = pNew;
   
   // Set the head.
   pHead = pNew;
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{
   erase(iterator(pTail));
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{
   erase(iterator(pHead));
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
{
   if (pHead)
      return pHead->data;
   throw("ERROR: unable to access data from an empty list");
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
   if (pTail)
      return pTail->data;
   throw("ERROR: unable to access data from an empty list");
}


/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
   // Catch empty lists
   if (empty())
      return end();

   iterator itNext = end();
   
   // Repoint previous and after nodes
   if (it.p->pNext)
   {
      it.p->pNext->pPrev = it.p->pPrev;
      itNext = it.p->pNext;
   }
   else
      pTail = pTail->pPrev;
   
   if (it.p->pPrev)
      it.p->pPrev->pNext = it.p->pNext;
   else
      pHead = pHead->pNext;
   
   // Delete p
   delete it.p;
   numElements--;
   return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                 const T & data) 
{
   // If no existing add it as head and tail.
   if (empty())
   {
      pHead = pTail = new Node(data);
      numElements++;
      return begin();
   }
   
   // Add at the end
   if (it == end())
   {
      Node* pNew = new Node(data);
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   
   // Else Hook it up
   else
   {
      // Create node and point it to existing nodes.
      Node* pNew = new Node(data);
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;
      
      // Hook up the existing nodes to pNew if they exist, if not update head/tail.
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
   // If no existing add it as head and tail.
   if (empty())
   {
      pHead = pTail = new Node(std::move(data));
      numElements++;
      return begin();
   }
   
   // Add at the end
   if (it == end())
   {
      Node* pNew = new Node(std::move(data));
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   
   // Else Hook it up
   else
   {
      // Create node and point it to existing nodes.
      Node* pNew = new Node(std::move(data));
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;
      
      // Hook up the existing nodes to pNew if they exist, if not update head/tail.
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;
      numElements++;
      return iterator(pNew);
   }}

/**********************************************
 * LIST :: SWAP
 * Swap one list with another
 *     INPUT  : a list to be swapped
 *     OUTPUT :
 *     COST   : O(1) 
 *********************************************/
template <typename T, typename A>
void list <T, A> ::swap(list <T, A>& rhs)
{
   std::swap(pHead, rhs.pHead);
   std::swap(pTail, rhs.pTail);
   std::swap(numElements, rhs.numElements);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
   std::swap(lhs.pHead, rhs.pHead);
   std::swap(lhs.pTail, rhs.pTail);
   std::swap(lhs.numElements, rhs.numElements);
}

}; // namespace custom


