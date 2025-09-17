/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include "vector.h"

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T, class Container = custom::vector<T>>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   //
   // Construct
   //
   
   stack()                       {                             }
   stack(const stack <T> &  rhs) { *this = rhs;                }
   stack(      stack <T> && rhs) { *this = std::move(rhs);     }
   stack(const Container &  rhs) { container = rhs;            }
   stack(      Container && rhs) { container = std::move(rhs); }
   ~stack()                      {                             }
   
   //
   // Assign
   //
   stack <T> & operator = (const stack <T> & rhs)
   {
      container = rhs.container;
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }
   void swap(stack <T>& rhs)
   {
      container.swap(rhs.container);
   }

   //
   // Access
   //
   
   T & top()       
   { 
      return container.back();
   }
   const T & top() const 
   { 
      return container.back();
   }

   //
   // Insert
   //
   
   void push(const T &  t) 
   {  
      container.push_back(t);
   }
   void push(      T && t) 
   {  
      container.push_back(std::move(t));
   }

   //
   // Remove
   //
   
   void pop() 
   {  
      container.pop_back();
   }

   //
   // Status
   //
   
   size_t size () const { return container.size() ; }
   bool   empty() const { return container.empty(); }
   
private:
   
   Container container;  // underlying container (probably a vector)
};



} // custom namespace


