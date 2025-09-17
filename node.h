/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
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
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   // Check for nullptr
   if (!pSource)
      return nullptr;
   
   // Set src and des
   auto pDestination = new Node<T>(pSource->data);
   auto * pSrc = pSource;
   auto * pDes = pDestination;
   
   // Copy
   for (auto *p = pSrc->pNext; p; p = p->pNext)
      pDes = insert(pDes, p->data, true);
   
   return pDestination;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   // Reuse existing nodes
   const Node<T> *  pSrc = pSource;
   Node<T> *  pDes = pDestination;
   Node<T> * pDesPrevious = nullptr;
   
   // Copy data for matching nodes between pSource and pDestination
   while (pSrc && pDes)
   {
      pDes->data = pSrc->data;
      pDesPrevious = pDes;
      pDes = pDes->pNext;
      pSrc = pSrc->pNext;
   }
   
   // If pSrc still has nodes, insert them into pDestination
   if (pSrc)
   {
      pDes = pDesPrevious;
      
      while (pSrc)
      {
         pDes = insert(pDes, pSrc->data, true);
         if (!pDestination)
            pDestination = pDes;
         pSrc = pSrc->pNext;
      }
      
      pDes = pDes->pNext;
   }
   
   // If pDes still has leftover nodes, clear them
   if (!pSrc && pDes)
   {
      bool setToNull = false;
      
      if (pDes->pPrev)
         pDes->pPrev->pNext = nullptr;
      else
         setToNull = true;
      
      clear(pDes);
      
      if (setToNull)
         pDestination = nullptr;
   }
      
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   // Return if node is nullptr
   if (!pRemove)
      return nullptr;
   
   // Set left and right pointers if nodes exist
   if (pRemove->pPrev)
      pRemove->pPrev->pNext = pRemove->pNext;
   if (pRemove->pNext)
      pRemove->pNext->pPrev = pRemove->pPrev;

   // Delete pRemove and return parent
   Node<T>* pReturn;
   if (pRemove->pPrev)
      pReturn = pRemove->pPrev;
   else
      pReturn = pRemove->pNext;
   delete pRemove;
   return pReturn;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   Node<T> *p = new Node<T>(t);
   
   // If list is null new node is the new list
   if (pCurrent == NULL)
      return p;
   
   // Insert after pCurrent
   if (after)
   {
      p->pPrev = pCurrent;
      
      // Check if new node is the new tail
      if (pCurrent->pNext)
      {
         p->pNext = pCurrent->pNext;
         p->pNext->pPrev = p;
      }
      pCurrent->pNext = p;
   }
   
   // Insert before pCurrent
   else
   {
      p->pNext = pCurrent;
      
      // Check if new node is the new head
      if (pCurrent->pPrev)
      {
         p->pPrev = pCurrent->pPrev;
         p->pPrev->pNext = p;
      }
      pCurrent->pPrev = p;
   }
   return p;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   size_t size = 0;
   for (auto p = pHead; p; p=p->pNext)
      size++;
   return size;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   for (auto p = pHead; p; p=p->pNext)
      out << *p;
   return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   while (pHead)
   {
      auto pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
   }
}


