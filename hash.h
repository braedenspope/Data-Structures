/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Tyler and Braeden
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set():buckets(8)
   {
      maxLoadFactor = 1;
      numElements = 0;
   }
   unordered_set(size_t numBuckets)
   {
      numElements = 0;
      maxLoadFactor = 1;
      rehash(numBuckets);
   }
   unordered_set(const unordered_set&  rhs) 
   {
      *this = rhs;
   }
   unordered_set(unordered_set&& rhs) 
   {
      *this = std::move(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last)
   {
      numElements = 0;
      maxLoadFactor = 1;
      reserve(last - first);
      for (;last != first; insert(*(first++)));
   }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
      numElements = rhs.numElements;
      maxLoadFactor = rhs.maxLoadFactor;
      buckets = rhs.buckets;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      numElements = rhs.numElements;
      maxLoadFactor = rhs.maxLoadFactor;
      buckets = std::move(rhs.buckets);
      
      rhs.numElements = 0;
      rhs.maxLoadFactor = 1;
      rhs.buckets.resize(8);
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(buckets, rhs.buckets);
      std::swap(numElements, rhs.numElements);
      std::swap(maxLoadFactor, rhs.maxLoadFactor);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      for(auto it = buckets.begin(); it!=buckets.end();it++)
      {
         if (!(*it).empty())
            return iterator(buckets.end(), it, (*it).begin());
      }
      return end();
   }
   iterator end()
   {
      typename custom::list<T>::iterator it;
      return iterator(buckets.end(), buckets.end(), it);
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      return Hash()(t) % bucket_count();
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
      rehash(num / maxLoadFactor);
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      for (auto& bucket : buckets)
         bucket.clear();
      numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
      return numElements;
   }
   bool empty() const 
   { 
      return numElements == 0;
   }
   size_t bucket_count() const 
   { 
      return buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
      assert(i >= 0 && i < buckets.size());
      return buckets[i].size();
   }
   float load_factor() const noexcept 
   { 
      return numElements / buckets.size(); 
   }
   float max_load_factor() const noexcept 
   { 
      return maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

private:

   size_t min_buckets_required(size_t num) const
   {
      return (size_t)std::ceil(num / maxLoadFactor);
   }

   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator() 
   {
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList)
   {
      this->itVectorEnd = itVectorEnd;
      this->itVector = itVector;
      this->itList = itList;
   }
   iterator(const iterator& rhs) 
   { 
      *this = rhs;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      itVectorEnd = rhs.itVectorEnd;
      itList = rhs.itList;
      itVector = rhs.itVector;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
      return !(*this == rhs); 
   }
   bool operator == (const iterator& rhs) const 
   { 
      return (itVector == rhs.itVector
              && itList == rhs.itList
              && itVectorEnd == rhs.itVectorEnd);
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      auto next = *this;
      ++(*this);
      return next;
   }

private:
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator()  
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) 
   {
      this->itList = itList;
   }
   local_iterator(const local_iterator& rhs) 
   { 
      this->itList = rhs.itList;
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return itList != rhs.itList;
   }
   bool operator == (const local_iterator& rhs) const
   {
      return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      itList++;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      auto next = this;
      itList++;
      return next;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T,Hash,E,A>::erase(const T& t)
{
   // Find the element to erase
   auto itErase = find(t);
   
   // If it doesnt exist return
   if (itErase == end())
      return itErase;
   
   // Get the next to return
   auto itNext = itErase;
   itNext++;
   
   // Erase
   (*itErase.itVector).erase(itErase.itList);
   numElements--;
   return itNext;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   // Fund the hash index
   size_t index = bucket(t);
   
   // Check if element already exists in the unordered set
   auto itHash = find(t);
   if (itHash != end())
      return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, false);
   
   // Check if need to reserve more space
   if (min_buckets_required(numElements + 1) > bucket_count())
   {
      reserve(numElements * 2);
      index = bucket(t);
   }
   
   // Add element
   buckets[index].push_back(t);
   numElements++;
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(find(t), true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
   for (auto item : il)
      insert(item);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
   // If already more buckets, return
   if (numBuckets <= bucket_count())
      return;
   
   // Make new buckets
   custom::vector<custom::list<T,A>> bucketNew(numBuckets);
   
   // Rehash all elements into the new buckets
   for (auto it = begin(); it != end(); it++ )
   {
      // Compute the hash value and place the item in the new bucket
      size_t newIndex = Hash()(*it) % numBuckets;
      bucketNew[newIndex].push_back(std::move(*it));
   }
   
   buckets.swap(bucketNew);
      
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   // Get the index
   size_t iBucket = bucket(t);
   
   // Find the iterator for the bucket
   auto itBucket = typename custom::vector<custom::list<T>>::iterator(iBucket, buckets);
   
   // Check for item
   for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); it++)
   {
      if (*it == t)
         return iterator(buckets.end(), itBucket ,it);
   }
   
   // If none return end
   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // Is at the end
   if (itVector == itVectorEnd)
      return *this;
   
   // Advance the list iterator
   ++itList;
   
   // If the list iterator has not hit the end, return
   if (itList != (*itVector).end())
      return *this;
   
   // Advance the vector iterator
   ++itVector;
   
   // While vector iterator is empty or is the end, iterate.
   while (itVector != itVectorEnd && (*itVector).empty())
   {
      ++itVector;
   }
   
   if (itVector != itVectorEnd)
      itList = (*itVector).begin();
   
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
   lhs.swap(rhs);
}

}
