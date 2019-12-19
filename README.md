# Experiments in another C++ wrapper for libxml2

The biggest issue I've seen with the use of libxml2 in a large project is memory management. It isn't always clear to programmers when a libxml2 pointer should be freed directly and when it will be freed implicitly when freeing an owning structure.

With modern C++, smart pointers not only make ownership clear, they also handle freeing resources automatically.

So, the primary goal here is to create an API where all pointers that should be freed by the application programmer are returned via std::unique\_ptr.

## Guiding principles (subject to change)

* Any pointers returned that should be freed by the caller will be returned as a std::unique\_ptr with the proper deleter.
* When it makes sense, provide functions that return collections or operate on collections.
* Functions that take callback parameters should be provided to capture common patterns.
* This library intends to follow the standard library naming conventions, so the names from libxml2 will be converted from camelCase to snake\_case.
* The "xml" prefix on identifiers will be dropped in favor of a namespace.
* Other elements of a function's name should be dropped when overloading makes them unnecessary
* 
