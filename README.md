Proposal for C++ standard graph library

This library is in the alpha stage and is not recommended for general use.

Build & Run Requirements
1. MSVC 2019 16.5 with following build settings
	/std:c++latest
	/permissive-
	/experimental:preprocessor (compliant pre-processor release in 16.6)
	precompiled headers (enbled; optional)
2. range-v3 library (assumed in D:\lib\range-v3)
3. Build tool: CMake
4. Test framework: Catch2
5. Conan?

C++20 requirements
1. Concepts (language and <concepts> header)
2. Ranges (range-v3)
3. [future] Modules

ORIENTATION
1.	./test/ contains unit tests. 
	test_* are test files
	data_* hold data to be used for tests
2.	graph/include holds the graph implementation
	./graph_fwd.hpp is the primary header that contains prototypes in the paper
	./compressed_adj_array.hpp is the directed graph interface
	./undirected_adj_list.hpp is the undirected graph interface
	./algorithm/ holds algorithm implementation
	./range/ holds range definitions
	./detail/ hold the implementation details for the graphs

NOTES (OTHER)
Range requirement for container
https://stackoverflow.com/questions/35542744/how-to-create-a-custom-container-for-range-v3

TODO
1.	Multi-platform compiler/build support
	a.	Use CMake for build in Windows
	b.	Use range-v3 macros for platform-specific concept support
	c.	Build with gcc9 & gcc10 in linux
	d.	Build with clang in linux
	e.	MSVC: support gcc & clang builds in WSL
	f.	use Catch2 for unit tests
2.	Tools
	a.	Add clang-tidy to VSCode 
3.	bgl17
	a.	[in process] Add simple adapator to show dfs_vertex_range iteratation with bgl17 vov graph
4.	Paper / Design
	a.	Strongly-connected components: impl, test, update paper
		i	implement & test
		ii	update paper
		iii	add example to paper
	b.	Connected components: impl, test, update paper
		i	implement & test
		ii	update paper
		iii	add example to paper
	c.	Bi-connected components: impl, test, update paper
		i	implement & test
		ii	update paper
		iii	add example to paper
	d.	Articulation Points: impl, test, update paper
		i	implement & test
		ii	update paper
		iii	add example to paper
	e.	Simple test with arrray<T> for constexpr
	f.	define graph module (C++20)
	g.	compare with other libraries (Lemon, ...)
	h.	algorithms: refine concept requirements
		i	support map for vertices? for edges?
		ii	allow non-integer vertex_key?
	i.	how extensive should graph concepts be, esp when an algo may not need everything?
	j.	add data structures
		i	directed data structure
		ii	unordered data structure
	k.	concepts
		i	Add has_edge_set concept?
		ii	Add has_vertex_set concept?

Thanks to
Jason Turner for the cpp_starter_project
