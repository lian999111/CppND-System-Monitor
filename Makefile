.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

build-gcc-7:
	mkdir -p build
	cd build && \
	cmake -D CMAKE_C_COMPILER=gcc-7 -D CMAKE_CXX_COMPILER=g++-7 .. && \
	make

.PHONY: debug
debug-gcc-7:
	mkdir -p build
	cd build && \
	cmake -D CMAKE_C_COMPILER=gcc-7 -D CMAKE_CXX_COMPILER=g++-7 -DCMAKE_BUILD_TYPE=debug .. && \
	make
	
.PHONY: clean
clean:
	rm -rf build
