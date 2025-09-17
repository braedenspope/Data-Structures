/***********************************************************************
 * Header:
 *    MAP
 * Summary:
 *    Our custom implementation of a std::map 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap;

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend class ::TestMap;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs); 
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() 
   {
   }
   map(const map &  rhs) : bst(rhs.bst)
   {
   }
   map(map && rhs) : bst(std::move(rhs.bst))
   {
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
      insert(first, last);
   }
   map(const std::initializer_list <Pairs>& il) 
   {
      *this = il;
   }
  ~map()         
   {
   }

   //
   // Assign
   //
   map & operator = (const map & rhs)
   {
      bst = rhs.bst;
      return *this;
   }
   map & operator = (map && rhs)
   {
      bst = std::move(rhs.bst);
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
      clear();

      //Insert each list item
      for (const Pairs& element : il)
         bst.insert(element, true);
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(bst.begin());
   }
   iterator end() 
   { 
      return iterator();    
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator find(const K & k)
   {
      return iterator(bst.find(k));
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      auto pair = bst.insert(std::move(rhs));
      return make_pair(iterator(pair.first), pair.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      auto pair = bst.insert(rhs);
      return make_pair(iterator(pair.first), pair.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      //Insert elements until the end is reached
      while (first != last)
      {
         bst.insert(*first, true);
         ++first;
      }
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
      //Insert each element
      for (const Pairs& element : il)
         bst.insert(element, true);
   }

   //
   // Remove
   //
   void clear() noexcept
   {
      bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap;
   template <class KK, class VV>
   friend class custom::map; 
public:
   //
   // Construct
   //
   iterator()
   {
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   { 
      it = rhs;
   }
   iterator(const iterator & rhs) 
   { 
      it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
      it++;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator next = *this;
      it++;
      return next;
   }
   iterator & operator -- ()
   {
      it--;
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      iterator next = *this;
      it--;
      return next;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
   custom::pair<K, V> pair(key);
   auto it = bst.find(pair);
   if (it.pNode != nullptr)
      return it.pNode->data.second;
   else
   {
      bst.insert(pair);
      return bst.find(pair).pNode->data.second;
   }
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
   custom::pair<K, V> pair(key);
   auto it = bst.find(pair);
   if (it.pNode != nullptr)
      return it.pNode->data.second;
   else
   {
      bst.insert(pair);
      return bst.find(pair).pNode->data.second;
   }
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   custom::pair<K, V> pair(key);
   auto it = bst.find(pair);
   if (it.pNode != nullptr)
      return it.pNode->data.second;
   throw std::out_of_range(std::string("invalid map<K, T> key"));
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   custom::pair<K, V> pair(key);
   auto it = bst.find(pair);
   if (it.pNode != nullptr)
      return it.pNode->data.second;
   throw std::out_of_range(std::string("invalid map<K, T> key"));
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
   std::swap(lhs.bst, rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   auto it = find(k);
   if (it == end())
      return 0;
   erase(it);
   return 1;
}
/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   if (first != end())
      while (first != last)
         first = erase(first);
   return last;
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return iterator(bst.erase(it.it));
}

}; //  namespace custom

