# Designing Data Structures Course

This repository contains comprehensive custom implementations of Standard Template Library (STL) data structures and algorithms. This advanced curriculum represents the pinnacle of computer science education, focusing on sophisticated design patterns, template programming, and enterprise-level software architecture.

## Course Overview

The Design Patterns course emphasizes advanced software engineering concepts including:
- **Template Metaprogramming**: Generic programming with type safety and performance optimization
- **Memory Management**: Custom allocator integration and RAII principles
- **Algorithm Complexity**: Optimized implementations with formal complexity analysis
- **Design Pattern Implementation**: Iterator, Container, and Adapter patterns
- **Enterprise Architecture**: Scalable, maintainable, and performant code structures

## Repository Structure

```
Design-Patterns/
├── array.h                    # Fixed-size array container with STL compliance
├── bnode.h                    # Binary tree node with comprehensive tree operations
├── bst.h                      # Red-Black Binary Search Tree implementation
├── deque.h                    # Double-ended queue with block-based memory management
├── hash.h                     # Unordered set with separate chaining hash table
├── list.h                     # Doubly-linked list with full STL interface
├── map.h                      # Associative container built on BST foundation
├── node.h                     # Linked list node with extensive utility functions
├── priority_queue.h           # Heap-based priority queue implementation
├── set.h                      # Ordered set container using red-black tree
├── stack.h                    # LIFO container with adapter pattern
├── vector.h                   # Dynamic array with automatic memory management
└── README.md                  # This file
```

## Data Structure Implementations

### Core Container Types

#### Array (`array.h`)
**Fixed-size container with compile-time bounds checking**

```cpp
template <typename T, int N>
struct array
```

**Key Features:**
- **Compile-time Size**: Template parameter for compile-time optimization
- **STL Compliance**: Full iterator support and standard interface
- **Memory Efficiency**: Stack-allocated with no dynamic allocation overhead
- **Bounds Checking**: Safe `at()` method with exception handling

**Implementation Highlights:**
- Zero-overhead abstraction over C-style arrays
- Iterator implementation supporting STL algorithms
- Exception-safe bounds checking with `std::out_of_range`

---

#### Vector (`vector.h`)
**Dynamic array with automatic memory management**

```cpp
template <typename T, typename A = std::allocator<T>>
class vector
```

**Advanced Features:**
- **Custom Allocator Support**: Template parameter for memory management strategy
- **Exception Safety**: Strong exception guarantee for all operations
- **Performance Optimization**: Exponential growth strategy with move semantics
- **Memory Management**: RAII with proper construction/destruction cycles

**Technical Implementation:**
```cpp
void reserve(size_t newCapacity);           // Capacity management
void resize(size_t newElements);            // Size adjustment with construction
void shrink_to_fit();                       // Memory optimization
```

**Complexity Guarantees:**
- **Access**: O(1) random access
- **Insertion**: O(1) amortized at end, O(n) elsewhere
- **Memory**: Optimal space utilization with exponential growth

---

#### Deque (`deque.h`)
**Double-ended queue with block-based memory architecture**

```cpp
template <typename T, typename A = std::allocator<T>>
class deque
```

**Sophisticated Architecture:**
- **Block-Based Storage**: Segmented memory for efficient front/back operations
- **Index Calculation**: Complex addressing with `iaFromID()`, `ibFromID()`, `icFromID()`
- **Dynamic Block Allocation**: On-demand memory allocation per block
- **Circular Buffer Logic**: Efficient wraparound indexing

**Performance Characteristics:**
- **Front/Back Operations**: O(1) push/pop at both ends
- **Random Access**: O(1) with complex index translation
- **Memory Efficiency**: Minimal wasted space with block allocation

---

### Tree-Based Structures

#### Binary Search Tree (`bst.h`)
**Red-Black tree implementation with self-balancing**

```cpp
template <typename T>
class BST
```

**Advanced Tree Features:**
- **Red-Black Balancing**: Automated tree balancing for O(log n) guarantees
- **Template Metaprogramming**: Generic implementation with type deduction
- **Iterator Implementation**: In-order traversal with STL compliance
- **Self-Balancing Logic**: Complex rotation and recoloring algorithms

**Balancing Algorithm:**
```cpp
void balance();                             // Red-black tree balancing
bool verifyRedBlack(int depth) const;       // Validation for debugging
```

**Complexity Analysis:**
- **Search/Insert/Delete**: O(log n) guaranteed through balancing
- **Space**: O(n) with minimal overhead per node

---

#### Set (`set.h`)
**Ordered container built on red-black tree foundation**

```cpp
template <typename T>
class set
```

**Design Pattern Implementation:**
- **Adapter Pattern**: Clean interface over BST complexity
- **Iterator Pattern**: STL-compliant iteration through sorted elements
- **Template Specialization**: Optimized for unique element storage

---

#### Map (`map.h`)
**Associative container with key-value pairs**

```cpp
template <class K, class V>
class map
```

**Key-Value Architecture:**
- **Pair-Based Storage**: Integration with `custom::pair<K,V>`
- **BST Foundation**: Leverages red-black tree for ordered keys
- **Subscript Operator**: Automatic insertion semantics for missing keys
- **Exception Safety**: `at()` method with bounds checking

---

### List Structures

#### Doubly-Linked List (`list.h`)
**Full-featured linked list with bidirectional iteration**

```cpp
template <typename T, typename A = std::allocator<T>>
class list
```

**Advanced List Features:**
- **Node Management**: Sophisticated pointer manipulation with sentinel handling
- **Memory Safety**: RAII with proper constructor/destructor chains
- **Iterator Invalidation**: Careful iterator management during modifications
- **Custom Allocator**: Template support for specialized memory management

**Implementation Complexity:**
```cpp
iterator insert(iterator it, const T& data);    // O(1) insertion
iterator erase(const iterator& it);             // O(1) removal with cleanup
```

#### Node Utilities (`node.h`, `bnode.h`)
**Foundation classes with extensive utility functions**

**Node Operations:**
- **Copy/Assign**: Deep copy algorithms with cycle detection
- **Memory Management**: Safe allocation/deallocation patterns
- **Tree Operations**: Binary tree manipulation and traversal

---

### Specialized Containers

#### Hash Table (`hash.h`)
**Unordered set with separate chaining**

```cpp
template <typename T, typename Hash = std::hash<T>>
class unordered_set
```

**Hash Table Architecture:**
- **Separate Chaining**: Vector of lists for collision resolution
- **Load Factor Management**: Automatic rehashing for performance
- **Custom Hash Functions**: Template parameter for hash strategy
- **Iterator Implementation**: Complex traversal across buckets

**Performance Optimization:**
```cpp
void rehash(size_t numBuckets);                 // Dynamic restructuring
float load_factor() const noexcept;            // Performance monitoring
```

---

#### Priority Queue (`priority_queue.h`)
**Heap-based priority container**

```cpp
template<class T, class Container = vector<T>, class Compare = std::less<T>>
class priority_queue
```

**Heap Implementation:**
- **Binary Heap**: Array-based heap with parent/child calculations
- **Percolation Algorithms**: Up/down heap maintenance
- **Custom Comparator**: Template support for priority determination
- **Container Adapter**: Built on underlying container abstraction

**Heap Operations:**
```cpp
bool percolateDown(size_t indexHeap);          // Heap property maintenance
void heapify();                                // Initial heap construction
```

---

#### Stack (`stack.h`)
**Container adapter with LIFO semantics**

```cpp
template<class T, class Container = vector<T>>
class stack
```

**Adapter Pattern Implementation:**
- **Container Abstraction**: Works with any STL-compatible container
- **Minimal Interface**: Clean LIFO operations only
- **Template Flexibility**: Configurable underlying storage

## Advanced Programming Concepts

### Template Metaprogramming
All implementations demonstrate sophisticated template techniques:

**Type Deduction and SFINAE:**
```cpp
template <class Iterator>
vector(Iterator first, Iterator last);         // Iterator-based construction

template <typename T, typename A = std::allocator<T>>
class vector;                                   // Default template parameters
```

**Perfect Forwarding:**
```cpp
void push_back(T&& t);                         // Move semantics support
template <class... Args>
void emplace_back(Args&&... args);             // Perfect forwarding
```

### Memory Management Excellence

**Custom Allocator Integration:**
- All containers support custom allocators through template parameters
- RAII principles ensure exception-safe resource management
- Move semantics minimize unnecessary copying

**Exception Safety Guarantees:**
- **Basic Guarantee**: No memory leaks on exception
- **Strong Guarantee**: Operation succeeds or leaves unchanged
- **No-throw Guarantee**: Critical operations never throw

### Algorithm Complexity

**Formal Complexity Analysis:**
- **Vector**: O(1) amortized insertion, O(n) worst-case
- **BST**: O(log n) guaranteed through red-black balancing
- **Hash Table**: O(1) average case with O(n) worst-case
- **Heap**: O(log n) insertion/removal with O(1) access

### Design Pattern Implementation

**Iterator Pattern:**
- Consistent iterator interfaces across all containers
- Support for STL algorithms and range-based loops
- Proper iterator invalidation semantics

**Adapter Pattern:**
- Stack and priority_queue as container adapters
- Clean interface abstraction over complex implementations

**Template Method Pattern:**
- Consistent container interfaces with specialized implementations
- Virtual-like behavior through template specialization

## Professional Development Standards

### Code Quality
- **Template Programming**: Advanced generic programming techniques
- **Exception Safety**: Comprehensive error handling and resource management
- **Performance Optimization**: Algorithmic complexity optimization
- **Memory Efficiency**: Minimal overhead with optimal space utilization

### Software Engineering Excellence
- **STL Compliance**: Full compatibility with Standard Library interfaces
- **Documentation**: Comprehensive inline documentation and complexity analysis
- **Testing Framework**: Unit test compatibility and debugging support
- **Maintainability**: Clean, readable code with clear separation of concerns

### Enterprise-Level Architecture
- **Scalability**: Efficient algorithms suitable for large-scale applications
- **Modularity**: Independent components with minimal coupling
- **Extensibility**: Template-based design for easy customization
- **Performance**: Production-ready implementations with optimal complexity

## Technical Achievements

### Advanced C++ Features
1. **Template Metaprogramming**: Sophisticated compile-time programming
2. **Memory Management**: Custom allocator support and RAII implementation
3. **Move Semantics**: Efficient resource transfer with perfect forwarding
4. **Exception Safety**: Strong exception guarantees throughout

### Algorithm Implementation
1. **Red-Black Trees**: Self-balancing binary search trees with O(log n) guarantees
2. **Hash Tables**: Efficient separate chaining with dynamic rehashing
3. **Heap Algorithms**: Binary heap implementation with percolation
4. **Iterator Design**: STL-compliant iterators with proper invalidation semantics

### System Design
1. **Container Adapters**: Clean abstraction layers over complex implementations
2. **Generic Programming**: Type-safe templates with minimal runtime overhead
3. **Memory Optimization**: Efficient memory usage patterns and allocation strategies
4. **Performance Engineering**: Algorithmic optimization for enterprise applications

## Learning Outcomes

This Design Patterns course demonstrates mastery of:

**Advanced Programming Concepts:**
- Template metaprogramming and generic algorithm design
- Memory management with custom allocators and RAII
- Exception-safe programming with strong guarantees
- Performance optimization and complexity analysis

**Software Architecture:**
- Design pattern implementation and application
- Large-scale system design and modular architecture
- Container design and STL interface compliance
- Enterprise-level code organization and maintainability

**Professional Development Skills:**
- Code review and quality assurance practices
- Documentation standards and technical communication
- Performance benchmarking and optimization techniques
- Industry-standard development practices and conventions

The comprehensive implementation of these fundamental data structures represents the culmination of computer science education, demonstrating not just programming competency, but software engineering excellence suitable for enterprise-level development.

---

*This repository represents advanced computer science coursework demonstrating professional-level software engineering capabilities, template programming mastery, and enterprise-grade system design skills.*
