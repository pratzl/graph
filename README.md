# Graph Library Proposal for the C++ Standard

This library is in the alpha stage that may include significant changes to the interface. It is not recommended for general use.

## Build & Run Requirements

### Prerequesites

1. A C++ compiler that supports Concepts and has the \<concepts\> header (gcc10, MSVC latest)
1. CMake 3.16 or later
1. Python3
1. Conan package manager (Python)

### Cloning & Building

```C++
git clone https://github.com/pratzl/graph.git
cd graph
mkdir build
cd build
cmake ../???
make
```

If you're using vscode in linux you may need to manually add gcc to the CMake Kits. Enter "CMake: Edit User-Local CMake Kits" in the Command Pallette (Ctrl+Shift+P) vscode to bring up the file.

The following libraries will automatically be installed by Conan

1. Catch2 unit test framework
2. range-v3 library
3. fmt library
4. spdlog (unused)

Other Useful Tools

1. clang-format
2. clang-tidy

## Orientation

1. ./cmake contains files used for building
1. ./include/graph/include holds the graph implementation
1. ./test/ contains unit tests.
        test_* are test files
        data_* hold data to be used for tests

## Naming Conventions
| Pattern           | Example              | Req'd? | Description                                                                          |
| :---------------- | :------------------- | :----: | :----------------------------------------------------------------------------------- |
| *_type            | edge_type            |   Y    | The full type stored in the graph                                                    |
| *_value_type      | edge_value_type      |   Y    | The edge type defined by the graph, without the edge_key                             |
| *_user_value_type | edge_user_value_type |   Y    | The user-defined type from the the template parameter (e.g EV)                       |
|                   |                      |        |                                                                                      |
| *_size_type       | vertex_size_type     |   Y    | The size_type defined by the underlying container, or size_t if there isn't one.     |
| *_index_type      | vertex_index_type    |   n    | The type used for referring to a vertex when it's kept in a random_access container. |
| *_key_type        | vertex_key_type      |   Y    | The key used for finding a vertex.                                                   |


## ToDo

- [ ] concepts
  - [ ] vertex_c --> vertex? (name conflict with vertex(g,uv))
  - [ ] add type traits? (needed when concepts exist?)
  - [ ] Add edge_set concept
  - [ ] Add vertex_set concept
  - [ ] algorithms: refine concept requirements
  - [ ] [paper] sparse/dense not defined;
  - [ ] compare to BGL concepts [Lumsdaine]
- [ ] Algorithms
  - [x] Common
    - [x] Change contiguous --> random_access for requirements
    - [ ] Test with constexpr
  - [ ] Strongly-connected components: impl, test, update paper
    - [x] implement
    - [ ] test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
  - [ ] Connected components: impl, test, update paper
    - [x] implement
    - [ ] test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
  - [ ] Bi-connected components: impl, test, update paper [Baran]
    - [ ] implement & test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
  - [ ] Articulation Points: impl, test, update paper [Baran]
    - [ ] implement
    - [ ] test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
  - [ ] shortest paths
    - [ ] Review template arguments
- [ ] Iterators
  - [ ] DFS
    - [ ] validate const iterator
  - [ ] BFS
    - [ ] validate const iterator
  - [ ] Topological Sort
    - [ ] implement
    - [ ] test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
- [ ] API
  - [x] Common
    - [x] Replace type...const& --> const_type&
    - [x] Add ssize defs & implementations for vertices, edge, outward_edges & inward_edges
    - [x] Add free functions for size, ssize, begin, end for each graph data structure (assume vertices)
    - [x] Remove degree functions (alias of size not needed)
  - [ ] Uniform API
    - [x] vertices_begin/end --> vertices_begin/end
    - [x] edges_begin/end --> edges_begin/end? replace begin/end(g,u)?
    - [ ] add vertices(g,u) -> vertex_range_t<G> & matching outward_vertices(g,u), inward_vertices(g,u)
- [ ] Data structures
  - [ ] Common
    - [x] remove edge & vertex definitions in paper (distracting & not referenced)
    - [x] replace use of short graph name with long name
    - [x] remove type defs on graph classes in paper (only ctors should exist)
    - [ ] describe rationale for data structures used
  - [ ] directed adjacency vector
    - [x] implement with vector
    - [ ] implement with deque?
    - [ ] implement with map? (non-integer index)
    - [ ] use common data structure for directed graph?
    - [ ] vertex_size_type, vertex_index_type, vertex_key_type are similar. Do we need all of them?
    - [x] rename directed_adjacency_array -> directed_adjacency_vector
  - [x] unordered adjacency list
    - [x] implement with vector
    - [ ] extend to support constexpr array
    - [ ] implement with deque?
    - [ ] implement with map? (non-integer index)
  - [ ] support constexpr data structures
- [ ] C++20
  - [ ] review use of sentinal
  - [ ] define graph module
  - [x] use spaceship operator (n/a b/c <, >, <=, >= isn't needed)
  - [ ] operator!= not needed when operator== defined? (part of spaceship operator)
  - [ ] investigate use of coroutines (stack space limits?)
- [ ] Unit Tests
  - [ ] Replace EXPECT_EQ(), etc. --> REQUIRE()
- [ ] Other libraries
  - [ ] bgl17
    - [x] vov graph
      - [x] Create adaptor
      - [x] Add depth_first_search_vertex_range test
      - [ ] Add depth_first_search_edge_range test
      - [ ] Add breadth_first_search_vertex_range test
      - [ ] Add breadth_first_search_edge_range test
    - [ ] compressed graph
      - [ ] Create adaptor
      - [ ] Add depth_first_search_vertex_range test
      - [ ] Add depth_first_search_edge_range test
      - [ ] Add breadth_first_search_vertex_range test
      - [ ] Add breadth_first_search_edge_range test
    - [ ] [paper] comparison?
  - [ ] boost
    - [ ] [paper] comparison
  - [ ] lemon
    - [ ] [paper] comparison
- [ ] Compiler/build support
  - [ ] Build with clang in linux
    - [ ] Use range-v3 headers/macros for concept support
  - [ ] use Catch2 hierarchy feature for unit tests
  - [ ] github failing action(s) on push
  - [ ] validate doxygen output
  - [ ] validate sanitizer generation
  - [ ] remove unrelated libraries (imgui, ...)
- [ ] Other Tools
  - [ ] Add clang-tidy to VSCode
  - [ ] Can VS be configured for development & remote development?
- [ ] Documentation
  - [ ] Add full instructions for building in README.md (c.f. range-v3)
  - [ ] Generate doxygen
  - [ ] Move todo out of README.md
- [ ] Feedback
  - [x] Tomaz Kaminski
    - [x] BFS & DFS range category --> input_iterator
    - [x] BFS & DFS iterator return types
    - [x] replace *reinterpret_cast<T*>(nullptr), with declval<T&>() 
  - [ ] Jens Mauer
    - [x] graph_traits<>
    - [ ] functions are missing concepts
    - [x] noexcept shouldn't be on all functions
    - [ ] (final comment review)
  - [ ] Andrew Sutton
    - [x] Move concepts to beginning of the paper
    - [ ] Drop _c suffix on concept names (vertex_c --> conflicts with vertex(g,uv) fnc)
    - [x] Add -ward suffix for inward & outward
    - [ ] Tighten up definition of undirected
    - [ ] Add new concepts
      - [ ] Path
      - [ ] Cycle
      - [ ] add/delete vertices|edges
    - [ ] (final comment review)
  - [ ] Nikolaev, Andrey (Intel)
    - [ ] Consider adding undirected_adjacency_vector & directed_adjacency_list (perf vs. mutabilty)
    - [ ] Consider adding adjacency_matrix, particularly for subgraph isomorphism
    - [x] graph names should only include long names (not short & long names)
    - [x] use long names for consistency & clarity (graphs, algorithms, API)
    - [x] Remove types on graph data structures. Only constructors should appear on graph
    - [x] Remove definitions for edge & vertex classes (confusing) in favor of using graph_traits types
    - [x] Add noexcept to size() & ssize() functions? (consider legacy graphs that don't use noexcept)

## Thanks to

Bob Steagal for his [gcc-builder & clang-builder scripts](https://github.com/BobSteagall)

Jason Turner for his [cpp_starter_project](https://github.com/lefticus/cpp_starter_project)
