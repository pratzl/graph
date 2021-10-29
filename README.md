![Build with gcc-10](https://github.com/pratzl/graph/workflows/Build%20with%20gcc-10/badge.svg?branch=master)

# Graph Library Proposal for the C++ Standard

This library is in the alpha stage that may include significant changes to the interface. It is not recommended for general use.

## Build & Run Requirements

### Prerequesites

1. A C++ compiler that supports Concepts and has the \<concepts\> header (gcc10, MSVC latest)
2. CMake 3.16 or later
3. Python3
4. Conan package manager (Python: pip install conan)

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
| Pattern           | Example           | Req'd? | Description                                                                                         |
| :---------------- | :---------------- | :----: | :-------------------------------------------------------------------------------------------------- |
| *_type            | edge_type         |   Y    | The full type stored in the graph                                                                   |
| *_value_type      | edge_value_type   |   Y    | The edge type defined by the graph, without the edge_key                                            |
|                   |                   |        |                                                                                                     |
| *_key_type        | vertex_key_type   |   Y    | The key used for finding a vertex.                                                                  |
| *_size_type       | vertex_size_type  |   Y    | The unsigned size type defined by the underlying container.                                         |
| *_difference_type | vertex_size_type  |   Y    | The signed size type defined by the underlying container.                                           |
| *_index_type      | vertex_index_type |   n    | The type used for referring to a vertex when it's kept in a random_access container. Internal only. |


## ToDo

- [ ] concepts & type traits
  - [x] vertex_c --> graph_vertex
  - [x] add type traits? (needed when concepts exist?)
  - [x] algorithms: refine concept requirements
  - [x] [paper] sparse/dense not defined;
  - [x] compare to BGL concepts [Lumsdaine]: need to add concepts for adjacency, incidence, vertex_list, edge_list
  - [x] allow vertex_key_t<G> to be non-integer? [no; requires algo specialization, performance impact]
  - [x] Add create_edges to incremental edge concepts
  - [x] Add create_vertices to incremental vertex concepts
  - [ ] Remove types (only functions); return types not important?
- [ ] Algorithms
  - [x] Common
    - [x] Replace vertex_user_value_type with vertex_value_type for undirected_adjacency_list
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
    - [ ] Replace shortest_path with shortest_path2
    - [ ] Replace first/last with range in shortest_distance?
    - [ ] distance_fnc should take edge iterator (not reference)
    - [ ] dikstra impl requires incidence_graph & edge_list_graph
    - [ ] bellman_ford impl requires edge_list_graph only
- [ ] Ranges/Iterators
  - [ ] DFS
    - [ ] validate const iterator
    - [ ] update examples to reflect changes
  - [ ] BFS
    - [ ] validate const iterator
    - [ ] update examples to reflect changes
  - [ ] Topological Sort
    - [ ] implement
    - [ ] test
    - [ ] [paper] update description with missing info
    - [ ] [paper] add example
- [ ] API
  - [x] Common
    - [x] Replace type...const& --> const_type&
    - [x] Add ssize defs & implementations for vertices, edge, outward_edges & inward_edges
    - [x] Add free functions for size, ssize, begin, end, empty for each graph data structure (assume vertices)
    - [x] Remove degree functions (alias of size not needed)
    - [x] Consider addition of [[nodiscard]] (std only uses it on empty & allocate, neither used in graph.hpp)
    - [x] const_vertex_key_type --> vertex_key_type, const_edge_key_type --> edge_key_type
    - [x] use West const to mirror usage in std
    - [x] replace value(gve) with separate functions for graph_value(g), vertex_value(g,u), edge_value(g,uv)
    - [x] replace vertex reference with vertex_iterator for vertex_value
    - [x] replace edge reference with edge_iterators for edge_value (mult overloads for each edge iterator type)
    - [x] replace proposed arithmetic concept with arithmetic_v<T> (avoid barrier to acceptance)
    - [x] Should find_edge() return edge_range_t or vertex_edge_range_t? edge_range_t, find_vertex_edge() is used for vertex_edge_range_t
    - [x] Is there a reason user_value_type can't just be value_type? No: only key_type & value_type should be exposed.
    - [x] Remove all range-specific begin/end functions in favor of using begin(rng)/end(rng)
    - [ ] Define common implementations that each graph can optionally specialize
      - [x] degree()
      - [ ] create default implementations of uniform API based on outgoing functions for directed graph
    - [x] Replace vertex & edge references with iterators
    - [ ] [paper] Give overview of different ranges
      - [ ] relate the ranges to the concepts
      - [ ] show relationship between uniform & directed functions
    - [x] move from std::graph to std namespace
    - [ ] Replace range types with references in API or graph_traits?
    - [x] Return optional<T> instead of pair<T,bool>: create_vertex, create_edge, create_outward_edge, create_inward_edge
    - [x] Create concept-based functions to consolidate definitions (e.g. edge properties)
    - [x] Rename ssize_t to difference_t (and related) to match naming in std
  - [ ] Uniform API
    - [x] vertices_begin/end --> vertices_begin/end
    - [x] edges_begin/end --> edges_begin/end? replace begin/end(g,u)?
    - [x] add vertices(g,u) -> vertex_vertex_range_t<G> & matching outward_vertices(g,u), inward_vertices(g,u) [add impl]
    - [x] add vertices_size(g,u) [add impl]
    - [x] add vertices_ssize(g,u) [add impl]
    - [x] add contains_vertex(g,ukey), contains_edge(g, ukey, vkey), contains_edge(g, u, v) [add impl]
    - [x] vertex_key_type required to be integral
    - [x] add sentinel types for each range (completeness)
    - [x] add helper functions to assure graph is an alias of vertices(g)
  - [ ] Directed API
  - [ ] ordered_pair & unordered_pair
    - [x] initial implementation
    - [x] initial test impl
    - [ ] support conditionally explicit ctors
    - [ ] support tuple_size<ordered_pair<T1,T2>> & tuple_size<unordered_pair<T1,T2>>
    - [ ] support tuple_element<orderd_pair<T1,T2>> & tuple_element<unorderd_pair<T1,T2>>
    - [ ] support get(ordered_pair<T1,T2>) & get(unordered_pair<T1,T2>)
    - [ ] support piecewise_construct_t ctor
    - [x] [paper] Add ordered_pair & unordered_pair to paper
    - [x] Should they be in std:: or std::graph:: ? std (with graph)
  - [ ] What functions can be defined globally as a default (e.g. contains_vertex(g,ukey), degree(g,u))?
  - [ ] Extenders
    - [ ] adjacency_extender
    - [ ] edge_list_extender
  - [ ] Mappers
    - [ ] outward_to_uniform_incidence
    - [ ] outward_to_uniform_adjacency
    - [ ] inward_to_uniform_incidence
    - [ ] inward_to_uniform_adjacency
  - [ ] How to support a bitartite graph (multiple vector types)?
- [ ] Data structures
  - [ ] Common
    - [x] remove edge & vertex definitions in paper (distracting & not referenced)
    - [x] replace use of short graph name with long name
    - [x] remove type defs on graph classes in paper (only ctors should exist)
    - [ ] assure CSR can be handled with the current graph ctors
    - [x] IndexT --> KeyT, integral concept
    - [x] Separate _impl files into _impl for the class and _api for the API adaptors
    - [ ] Verify that value types can be a variant
    - [ ] Add random_access_contraints for user-defined containers
    - [ ] Add optional vertex count parameter to edges ctor to prevent req of scanning edges
  - [ ] directed adjacency vector
    - [x] implement with vector
    - [x] implement with deque? [easy when edgeset=deque<edge>, for vertex_set=deque<> vertex_key must be stored in vertex b/c vertex_key can't be calc'd with vertex*]
    - [x] implement with map? (non-integer index) [no: too big of impact to algorithms]
    - [x] vertex_size_type, vertex_index_type, vertex_key_type are similar. Do we need all of them? [key can be used in place of index]
    - [x] rename directed_adjacency_array -> directed_adjacency_vector
    - [ ] [wait for P1709 review by LEWG] Create paper D2240R0
    - [ ] create matching undirected adjacency vector with immutable edge values
    - [x] Add container parameters for vertices & edges
  - [ ] unordered adjacency list
    - [x] implement with vector
    - [ ] extend to support constexpr array
    - [x] implement with deque? [for vertex_set=deque<> vertex_key must be stored in vertex b/c vertex_key can't be calc'd with vertex*]
    - [x] implement with map? (non-integer index) [no: too big of impact to algorithms]
    - [ ] [wait for P1709 review by LEWG] Create paper D2239R0 
    - [x] Add container parameter for vertices
  - [ ] Create paper for D2241R0 graph data structure design directions
    - [ ] describe rationale for data structures used
    - [x] support Compressed Sparse Row (CSR)?
      - [x] no: can't store vertex props; vertex lookup log(n)
  - [ ] support constexpr data structures (vector, array)
  - [ ] create adaptors for directed graphs
    - [ ] map Outward directed functions to Vertex-Edge & Vertex-Vertex functions (when present)
    - [ ] map Inward directed functions to Vertex-Edge & Vertex-Vertex functions (when present)
- [ ] C++20
  - [x] review use of sentinal; removed: it can be retrieved from a range type if needed
  - [ ] define graph module
  - [x] use spaceship operator (n/a b/c <, >, <=, >= isn't needed)
  - [ ] operator!= not needed when operator== defined? (part of spaceship operator)
  - [ ] investigate use of coroutines for BFS & DFS (stack space limits?)
  - [ ] what is the status of make_subrange<I,S> for ranges?
- [ ] Unit Tests
  - [ ] Replace EXPECT_EQ(), etc. --> REQUIRE()
  - [x] Switch from range-v3 to compiler-defined ranges (except algorithms like sort)
  - [ ] Create templated tests based on directed/undirected types
  - [ ] Move German Routes to text file
- [ ] Other libraries
  - [ ] NWGraph
    - [ ] rename bgl17 to NWGraph
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
    - [ ] Add adaptor example
  - [ ] lemon
    - [ ] [paper] comparison
- [ ] Compiler/build support
  - [ ] Build with clang in linux (when <concepts> header available)
  - [ ] use Catch2 hierarchy feature for unit tests
  - [ ] github failing action(s) on push
  - [ ] validate doxygen output
  - [ ] validate sanitizer generation
  - [x] remove unrelated libraries (imgui, ...)
  - [x] use compiler-supplied concepts (msvc, gcc)
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
    - [x] functions are missing concepts
    - [x] noexcept shouldn't be on all functions
    - [ ] (final comment review)
  - [ ] Andrew Sutton
    - [x] Move concepts to beginning of the paper
    - [x] Drop _c suffix on concept names (vertex_c --> conflicts with vertex(g,uv) fnc)
    - [x] Add -ward suffix for inward & outward
    - [x] Tighten up definition of directed & undirected
    - [x] Consider creating ordered_pair and unordered_pair [as sub-structs of std::pair]
    - [ ] Add new concepts
      - [ ] Path
      - [ ] Cycle
      - [x] add/delete vertices|edges
    - [ ] (final comment review)
  - [ ] Andrey Nikolaev & Tatyana Bysheva (Intel)
    - [ ] Consider adding undirected_adjacency_vector & directed_adjacency_list (perf vs. mutabilty)
    - [ ] Consider adding adjacency_matrix, particularly for subgraph isomorphism
    - [x] graph names should only include long names (not short & long names)
    - [x] use long names for consistency & clarity (graphs, algorithms, API)
    - [x] Remove types on graph data structures. Only constructors should appear on graph
    - [x] Remove definitions for edge & vertex classes (confusing) in favor of using graph_traits types
    - [x] Add noexcept to size() & ssize() functions? (consider legacy graphs that don't use noexcept)
    - [ ] edges(g,u): what is behavior when u isn't in g?
      - [ ] Return empty range (preferred)
      - [ ] Undefined behavior if determining if u is in g is expensive
      - [ ] How does this apply to other functions in the API
    - [ ] how are invalid parameters handled in algorithms (cf API)? (undefined behavior, ...)
    - [ ] consider creating a subgraph_view<G>
    - [x] consider adding edge_key<G>, edge_key(g,uv) & edge_key(g,u,v)
    - [ ] Are const versions of algos needed? (If I pass a const graph g, are all dependent types const?)
    - [ ] Consider re-adding degree() (alias of size or ssize) : alias of ranges::size(rng)
  - [ ] Leanne/Jing Dong <jdleanne@gmail.com> 10/7/20
    - [ ] Recommend adding incidence matrix
  - [ ] SG19 Review 10/8/20
    - [x] Remove unneeded const types in graph_traits
    - [x] Use iterator_t<range> to define types in graph_traits when possible
    - [ ] Move graph data structures into separate proposals
    - [x] Other
      - [x] using map<K,V> for vertices is possible, but requires different algorithms to match
    - [ ] ToDo
      - [ ] concepts: incidence_list, vertex_list, edge_list, path, cycle, add/del vertex/edge
      - [ ] constexpr everywhere? (vector, array)
      - [ ] comparison to boost, Lemon, ...
    - [ ] Optional
      - [ ] co-routines
      - [ ] modules

## Thanks to

Bob Steagal for his [gcc-builder & clang-builder scripts](https://github.com/BobSteagall)

Jason Turner for his [cpp_starter_project](https://github.com/lefticus/cpp_starter_project)

René FerdinandRivera Morell for his [duck_invoke](https://github.com/bfgroup/duck_invoke), an implementation
of tag_invoke (P1895) that works for both gcc and msvc.
