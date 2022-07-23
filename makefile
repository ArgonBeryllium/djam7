CXX=clang++
LIBS=-lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf\
	 -I $(LIBCUMT) -I $(CUMT_DIR)/src/
CXXFLAGS=--std=c++17 -g

BIN_DIR=bin
TEST_SRC=$(wildcard src/*.cpp)
TEST_BIN=$(BIN_DIR)/test

CXX_WIN=x86_64-w64-mingw32-g++
TEST_BIN_WIN=$(BIN_DIR)/test.exe

CUMT_DIR=./cumt
LIBCUMT=$(CUMT_DIR)/bin/lib/libcumt.a
LIBCUMT_SRC=$(wildcard $(CUMT_DIR)/src/cumt*.cpp)

test: $(TEST_BIN) $(LIBCUMT_SRC)

$(TEST_BIN): $(TEST_SRC) $(LIBCUMT) src/*.h
	$(CXX) $(CXXFLAGS) $(LIBS) $(TEST_SRC) $(LIBCUMT) -o $(TEST_BIN)

# TODO fix this later
$(TEST_BIN_WIN): $(TEST_SRC) $(LIBCUMT) src/*.h
	$(CXX_WIN) $(CXXFLAGS) $(LIBS) $(TEST_SRC) $(LIBCUMT) -o $(TEST_BIN_WIN)

$(LIBCUMT): $(LIBCUMT_SRC)
	make -C $(CUMT_DIR) lib_static
