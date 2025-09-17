/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Tyler and Braedon
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements     ; }
   
private:
   
   

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
   
   void deleteBinaryTree(BNode * pNode);
   void copyBinaryTree(BNode *& pDest, const BNode * pSrc);
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      pParent = pLeft = pRight = nullptr;
      isRed = true;
   }
   BNode(const T &  t):data(t)
   {
      pParent = pLeft = pRight = nullptr;
      isRed = true;
   }
   BNode(T && t):data(std::move(t))
   {
      pParent = pLeft = pRight = nullptr;
      isRed = true;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);


   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pRight == pNode; }
   bool isLeftChild( BNode * pNode) const { return pLeft  == pNode; }

   // balance the tree
   void balance();
   
#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};


/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)
   {
      pNode = p;
   }
   iterator(const iterator & rhs)         
   {
      pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator next = this;
      next;
      this++;
      return *next;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      iterator next = this;
      next;
      this--;
      return *next;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};



/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs):BST()
{
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs):BST()
{
   *this = std::move(rhs);
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il):BST()
{
   *this = il;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   copyBinaryTree(root, rhs.root);
   numElements = rhs.numElements;
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (const T & item: il)
      insert(item);
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   std::swap(root, rhs.root);
   std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   // Save the pParent so when the node p become a nullptr we know where to place the new node
   BNode * pParent = root;
   BNode * p = root;
   
   // Initialize a variable to deturmine where to place the new node and eliminating and extra use of <
   bool putLeft = true;
   
   // Loop until it finds where to place the new node
   while (p != nullptr)
   {
      pParent = p;
      
      // If keepUnique prevent duplicates from being inserted
      if (keepUnique)
      {
         // Return the location of the dduplicate
         if (t == p->data)
            return std::pair<iterator, bool>(iterator(p), false);
      }
      
      // If t is less than data go ledt
      if (t < p->data)
      {
         putLeft = true;
         p = p->pLeft;
      }
      
      // Otherwise go right
      else
      {
         putLeft = false;
         p = p->pRight;
      }
   }
   
   // Allocate the new node
   BNode * pNew = new BNode(t);
   
   // Place in the tree if parent exists
   if (pParent != nullptr)
   {
      // Decide which side
      if (putLeft)
      {
         pNew->pParent = pParent;
         pParent->pLeft = pNew;
      }
      else
      {
         pNew->pParent = pParent;
         pParent->pRight = pNew;
      }
   }
   
   // If no parent then the new node is the root
   else
      root = pNew;
   
   // Increment the elements and balance the tree
   numElements++;
   pNew->balance();
   
   // Find the new root node
   while(root->pParent != nullptr)
      root=root->pParent;
   
   // Return the pair
   std::pair<iterator, bool> pairReturn(iterator(pNew), true);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   // Save the pParent so when the node p become a nullptr we know where to place the new node
   BNode * pParent = root;
   BNode * p = root;
   
   // Initialize a variable to deturmine where to place the new node and eliminating and extra use of <
   bool putLeft = true;
   
   // Loop until it finds where to place the new node
   while (p != nullptr)
   {
      pParent = p;
      
      // If keepUnique prevent duplicates from being inserted
      if (keepUnique)
      {
         // Return the location of the dduplicate
         if (t == p->data)
            return std::pair<iterator, bool>(iterator(p), false);
      }
      
      // If t is less than data go ledt
      if (t < p->data)
      {
         putLeft = true;
         p = p->pLeft;
      }
      
      // Otherwise go right
      else
      {
         putLeft = false;
         p = p->pRight;
      }
   }
   
   // Allocate the new node
   BNode * pNew = new BNode(std::move(t));
   
   // Place in the tree if parent exists
   if (pParent != nullptr)
   {
      // Decide which side
      if (putLeft)
      {
         pNew->pParent = pParent;
         pParent->pLeft = pNew;
      }
      else
      {
         pNew->pParent = pParent;
         pParent->pRight = pNew;
      }
   }
   
   // If no parent then the new node is the root
   else
      root = pNew;
   
   // Increment the elements and balance the tree
   numElements++;
   pNew->balance();
   
   // Find the new root node
   while(root->pParent != nullptr)
      root=root->pParent;
   
   // Return the pair
   std::pair<iterator, bool> pairReturn(iterator(pNew), true);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator & it)
{
   if (it == end())
      return end();
   
   iterator next = it;  // Start with the current iterator
   ++next;              // Move to the next iterator before deleting the current one

   // Case 1: Node has no children
   if (it.pNode->pRight == nullptr && it.pNode->pLeft == nullptr)
   {
      if (it.pNode->pParent != nullptr)
      {
         if (it.pNode->pParent->pRight == it.pNode)
            it.pNode->pParent->pRight = nullptr;
         else
            it.pNode->pParent->pLeft = nullptr;
      }
      delete it.pNode;
   }
   
   // Case 2: Node has one child
   else if (it.pNode->pRight == nullptr && it.pNode->pLeft != nullptr)
   {
      it.pNode->pLeft->pParent = it.pNode->pParent;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pRight == it.pNode)
         it.pNode->pParent->pRight = it.pNode->pLeft;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = it.pNode->pLeft;
      delete it.pNode;
   }
   else if (it.pNode->pLeft == nullptr && it.pNode->pRight != nullptr)
   {
      it.pNode->pRight->pParent = it.pNode->pParent;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pRight == it.pNode)
         it.pNode->pParent->pRight = it.pNode->pRight;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = it.pNode->pRight;
      delete it.pNode;
   }
   
   // Case 3: Node has 2 children
   else {
      BNode* successor = next.pNode;
      BNode* oldNode = it.pNode;
      
      if (oldNode->pParent == nullptr)
      {
         root = successor;
         successor->pParent = nullptr;
      }
      else if (oldNode->pParent->pLeft == oldNode)
         oldNode->pParent->addLeft(successor);
      else
         oldNode->pParent->addRight(successor);
      
      BNode* oldRight = oldNode->pRight;
      oldNode->pRight->addLeft(successor->pRight);
      successor->addLeft(oldNode->pLeft);
      
      if (oldRight != successor)
         successor->addRight(oldNode->pRight);
      
      delete oldNode;
   }
   
   numElements--;
   return next;
}


/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   for (iterator it = begin(); it != end();)
   {
       it = erase(it); // Erase returns the next iterator
   }
   root = nullptr; // Set root to nullptr after clearing
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (root == nullptr)
      return iterator(nullptr);
   
   // Finds the leftmost node
   BNode* pNode  = root;
   while (pNode->pLeft != nullptr)
      pNode = pNode->pLeft;
   
   return iterator(pNode);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode * p = root;
   
   // Navigate tree iteratively
   while (p != nullptr)
   {
      if (p->data == t)
         return iterator(p);
      else if (t < p->data)
         p = p->pLeft;
      else
         p = p->pRight;
   }
   
   // If not found return end
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

template <typename T>
void  BST <T> :: deleteBinaryTree(BNode * pNode)
{
   if (pNode != nullptr)
   {
      deleteBinaryTree(pNode->pLeft);
      deleteBinaryTree(pNode->pRight);
      delete pNode;
      pNode = nullptr;
   }
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <typename T>
void BST<T>::copyBinaryTree(BNode *& pDest, const BNode * pSrc)
{
   if (pSrc == nullptr)
   {
      deleteBinaryTree(pDest);
      pDest = nullptr;
      return;
   }
   
   // Create new node if pDest is null
   if (pDest == nullptr)
   {
      pDest = new BNode(pSrc->data);
      pDest->isRed=pSrc->isRed;
   }
   else
   {
      // Update data if pDest already exists
      pDest->data = pSrc->data;
      pDest->isRed = pSrc->isRed;
   }

   // Recursive call for the right child
   copyBinaryTree(pDest->pRight, pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
   
   // Recursive call for the left child
   copyBinaryTree(pDest->pLeft, pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   pLeft = pNode;
   if (pNode != nullptr)
      pLeft->pParent=this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   pRight = pNode;
   if (pNode != nullptr)
      pRight->pParent=this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode * newNode = new BNode(t);
   pLeft = newNode;
   newNode->pParent = *this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BNode * newNode = new BNode(std::move(t));
   pLeft = newNode;
   newNode->pParent = *this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode * newNode = new BNode(t);
   pRight = newNode;
   newNode->pParent = *this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BNode * newNode = new BNode(std::move(t));
   pRight = newNode;
   newNode->pParent = *this;
}



#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (!pParent->isRed)
      return;
   
   // Find importants nodes and save their pointers so the aren't lost when doing balanceing
   BNode *pAunt = (pParent->pParent->pLeft == pParent) ? pParent->pParent->pRight : pParent->pParent->pLeft;
   BNode *pGranny = pParent->pParent;
   BNode *pSibling = (pParent->pLeft == this) ? pParent->pRight : pParent->pLeft;
   BNode *pHead = pGranny->pParent;
   
   // Case 3: if the aunt is red, then just recolor
   if (pParent->isRed && pGranny)
   {
      if (!pGranny->isRed && pAunt && pAunt->isRed)
      {
         pGranny->isRed = true;
         pParent->isRed = false;
         pAunt->isRed = false;
         return pGranny->balance();
      }
   }

   // Case 4: if the aunt is black or non-existant, then we need to rotate
   if (pAunt == nullptr || !pAunt->isRed)
   {
      // Case 4a: We are mom's left and mom is granny's left
      if ( this->pParent->isLeftChild(this) && pGranny->isLeftChild(pParent))
      {
         pParent->addRight(pGranny);
         pGranny->addLeft(pSibling);
         
         pGranny->isRed = true;
         pParent->isRed = false;
         
         pParent->pParent = pHead;
         
         if (pHead != nullptr)
         {
            if (pHead->isRightChild(pGranny))
               pHead->pRight = pParent;
            else
               pHead->pLeft = pParent;
         }
      }
      // case 4b: We are mom's right and mom is granny's right
      if (pParent->isRightChild(this) && pGranny->isRightChild(pParent))
      {
         pParent->addLeft(pGranny);
         pGranny->addRight(pSibling);
         
         pGranny->isRed = true;
         pParent->isRed = false;
         
         pParent->pParent = pHead;
         
         if (pHead != nullptr)
         {
            if (pHead->isRightChild(pGranny))
               pHead->pRight = pParent;
            else
               pHead->pLeft = pParent;
         }
         
      }
      // Case 4c: We are mom's right and mom is granny's left
      if (!pGranny->isRed && pParent->isRightChild(this) && pGranny->isLeftChild(pParent))
      {
         BNode* tempParent = pParent;
         pGranny->addLeft(this->pRight);
         pParent->addRight(this->pLeft);
         
         
         if (pGranny->pParent == nullptr)
            this->pParent = nullptr;
         else if (pGranny->pParent->isRightChild(pGranny))
            pGranny->pParent->pRight = this;
         else
            pGranny->pParent->pLeft = this;
         
         this->addRight(pGranny);
         this->addLeft(tempParent);

         pGranny->isRed = true;
         this->isRed = false;
         
         this->balance();
      }
      
      // case 4d: we are mom's left and mom is granny's right
      if (!pGranny->isRed && pParent->isLeftChild(this) && pGranny->isRightChild(pParent))
      {
         BNode* tempParent = pParent;
         pGranny->addRight(this->pLeft);
         pParent->addLeft(this->pRight);
         
         if (pGranny->pParent == nullptr)
            this->pParent = nullptr;
         else if (pGranny->pParent->isRightChild(pGranny))
            pGranny->pParent->pRight = this;
         else
            pGranny->pParent->pLeft = this;
         
         this->addLeft(pGranny);
         this->addRight(tempParent);
         
         pGranny->isRed = true;
         this->isRed = false;
         
         this->balance();
      }
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   // If nullptr return
   if (pNode == nullptr)
      return *this;
   
   // If goes right once then all the way to the left it can
   if (pNode->pRight)
   {
      pNode = pNode->pRight;
      while (pNode->pLeft)
         pNode = pNode->pLeft;
   }
   
   // If node is a left child then go up
   else if (pNode->pParent && pNode->pParent->isLeftChild(pNode))
   {
      pNode = pNode->pParent;
   }
   
   // Go up as long as pNode is its parents' right child
   else
   {
      while(pNode->pParent && pNode->pParent->isRightChild(pNode))
         pNode = pNode->pParent;
      pNode = pNode->pParent;
   }
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   // If nullptr return
   if (pNode == nullptr)
      return *this;
   
   // If goes left once then all the way to the right it can
   else if (pNode->pLeft)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight)
         pNode = pNode->pRight;
   }
   
   // If node is a right child then go up
   else if (pNode->pLeft == nullptr && pNode->pParent->isRightChild(pNode))
   {
      pNode = pNode->pParent;
   }
   
   // Go up as long as pNode is its parents' left child
   else if (pNode->pLeft == nullptr && pNode->pParent->isLeftChild(pNode))
   {
      while(pNode->pParent && pNode->pParent->isLeftChild(pNode))
         pNode = pNode->pParent;
      pNode = pNode->pParent;
   }
   return *this;
}
} // namespace custom


