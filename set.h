/***********************************************************************
* Header:
*    Set
* Summary:
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Tyler and Braeden
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

   //   class TestSet;

   /************************************************
    * SET
    * A class that represents a Set
    ***********************************************/
   template <typename T>
   class set
   {
      friend class ::TestSet; // give unit tests access to the privates
   public:

      // 
      // Construct
      //
      set() : bst()
      {
      }
      set(const set& rhs) : bst(rhs.bst)
      {
      }
      set(set&& rhs) : bst(std::move(rhs.bst))
      {
      }
      set(const std::initializer_list <T>& il)
      {
         *this = il;
      }
      template <class Iterator>
      set(Iterator first, Iterator last)
      {
         insert(first, last);
      }
      ~set() { bst.clear(); }

      //
      // Assign
      //

      set& operator = (const set& rhs)
      {
         bst = rhs.bst;
         return *this;
      }
      set& operator = (set&& rhs)
      {
         bst = std::move(rhs.bst);
         return *this;
      }
      set& operator = (const std::initializer_list <T>& il)
      {
         clear();

         //Insert each list item
         for (const T& element : il)
            bst.insert(element, true);
         return *this;
      }
      void swap(set& rhs) noexcept
      {
         bst.swap(rhs.bst);
      }

      //
      // Iterator
      //

      class iterator;
      iterator begin() const noexcept
      {
         return iterator(bst.begin());
      }
      iterator end() const noexcept
      {
         return iterator(bst.end());
      }

      //
      // Access
      //
      iterator find(const T& t)
      {
         return bst.find(t);
      }

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

      //
      // Insert
      //
      std::pair<iterator, bool> insert(const T& t)
      {
         return bst.insert(t, true);
      }
      std::pair<iterator, bool> insert(T&& t)
      {
         return bst.insert(std::move(t), true);
      }
      void insert(const std::initializer_list <T>& il)
      {
         //Insert each element
         for (const T& element : il)
            bst.insert(element, true);
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


      //
      // Remove
      //
      void clear() noexcept
      {
         bst.clear();
      }
      iterator erase(iterator& it)
      {
         return bst.erase(it.it);
      }
      size_t erase(const T& t)
      {
         //Find the desired element
         auto it = bst.find(t);

         //If the element isn't found, return 0. Otherwise erase it and return 1
         if (it == bst.end())
            return 0;
         erase(it);
         return 1;
      }
      iterator erase(iterator& itBegin, iterator& itEnd)
      {
         //Insert elements until the end is reached
         while (itBegin != itEnd)
         {
            itBegin = erase(temp);
         }
         return itEnd;
      }

   private:

      custom::BST <T> bst;
   };


   /**************************************************
    * SET ITERATOR
    * An iterator through Set
    *************************************************/
   template <typename T>
   class set <T> ::iterator
   {
      friend class ::TestSet; // give unit tests access to the privates
      friend class custom::set<T>;
   public:
      // constructors, destructors, and assignment operator
      iterator()
      {
      }
      iterator(const typename custom::BST<T>::iterator& itRHS)
      {
         it = itRHS;
      }
      iterator(const iterator& rhs)
      {
         it = rhs.it;
      }
      iterator& operator = (const iterator& rhs)
      {
         it = rhs.it;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const iterator& rhs) const
      {
         return it != rhs.it;
      }
      bool operator == (const iterator& rhs) const
      {
         return it == rhs.it;
      }

      // dereference operator: by-reference so we can modify the Set
      const T& operator * () const
      {
         return *it;
      }

      // prefix increment
      iterator& operator ++ ()
      {
         ++it;
         return *this;
      }

      // postfix increment
      iterator operator++ (int postfix)
      {
         iterator temp(*this);
         ++it;
         return temp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         --it;
         return *this;
      }

      // postfix decrement
      iterator operator-- (int postfix)
      {
         iterator temp(*this);
         --it;
         return *this;
      }

   private:

      typename custom::BST<T>::iterator it;
   };



}; // namespace custom



