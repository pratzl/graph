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

## ToDo

- [ ] concepts
  - [ ] vertex_c --> vertex? (name conflict with vertex(g,uv))
  - [ ] add type traits? (needed when concepts exist?)
  - [ ] Add edge_set concept
  - [ ] Add vertex_set concept
  - [ ] algorithms: refine concept requirements
  - [ ] [paper] sparse/dense not defined
- [ ] Algorithms
  - [ ] Common
    - [x] Change contiguous --> random_access for requirements
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
- [ ] API
  - [x] Common
  - [ ] Uniform API
    - [ ] vertex_begin/end --> vertices_begin/end
    - [ ] edge_begin/end --> edges_begin/end? replace begin/end(g,u)?
  - [x] Outgoing API
  - [x] Ingoing API
- [ ] Data structures
  - [ ] directed adjacency array
    - [x] implement with vector
    - [ ] extend to support constexpr array
    - [ ] implement with deque?
    - [ ] implement with map? (non-integer index)
    - [ ] use common data structure for directed graph?
  - [x] unordered adjacency list
    - [x] implement with vector
    - [ ] extend to support constexpr array
    - [ ] implement with deque?
    - [ ] implement with map? (non-integer index)
- [ ] C++20
  - [ ] review use of sentinal
  - [ ] define graph module (C++20)
  - [x] use spaceship operator (n/a b/c <, >, <=, >= isn't needed)
  - [ ] investigate use of coroutines (stack space limits?)
- [ ] Unit Tests
  - [ ] Replace EXPECT_EQ(), etc. --> REQUIRE()
- [ ] Other libraries
  - [ ] bgl17
    - [ ] [in process] Add simple adaptor to show dfs_vertex_range iteratation with bgl17 vov graph
    - [ ] Add simple adaptor for compressed graph
    - [ ] Move adaptor code to ./adaptor
  - [ ] boost
    - [ ] [paper] comparison
  - [ ] lemon
    - [ ] [paper] comparison
- [ ] Compiler/build support
  - [ ] Build with clang in linux
    - [ ] Use range-v3 macros for platform-specific concept support
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
  - [ ] Tomaz Kaminski
    - [x] BFS & DFS range category --> input_iterator
    - [x] BFS & DFS iterator return types
    - [x] replace *reinterpret_cast<T*>(nullptr), with declval<T&>() 
  - [ ] Jens Mauer
    - [ ] graph_traits<>
    - [ ] functions are missing concepts
    - [x] noexcept shouldn't be on all functions
    - [ ] (final comment review)
  - [ ] Andrew Sutton
    - [ ] Move concepts to beginning of the paper
    - [ ] Drop _c suffix on concept names (vertex_c --> conflicts with vertex(g,uv) fnc)
    - [ ] Add -ward suffix for inward & outward (or -coming/-going?)
    - [ ] Tighten up definition of undirected
    - [ ] Add new concepts
      - [ ] Path
      - [ ] Cycle
      - [ ] add/delete vertices|edges
    - [ ] (final comment review)

## Thanks to

Bob Steagal for his [gcc-builder & clang-builder scripts](https://github.com/BobSteagall)

Jason Turner for his [cpp_starter_project](https://github.com/lefticus/cpp_starter_project)
