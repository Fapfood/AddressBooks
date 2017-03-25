CC = gcc
CFLAGS = -std=gnu11                         #-Wall -pedantic


_LIB_OBJ = contact.o listBook.o treeBook.o
LIB_OBJ = $(patsubst %,src/obj/%,$(_LIB_OBJ))


all: static

static: $(LIB_OBJ)
	@mkdir -p lib
	ar rcs lib/libaddressbook.a $^

shared: _CFLAGS = -fPIC
shared: $(LIB_OBJ)
	@mkdir -p lib
	$(CC) -shared -o lib/libaddressbook.so $^

src/obj/%.o: src/%.c src/%.h
	@mkdir -p src/obj
	$(CC) -c -o $@ $< $(CFLAGS) $(_CFLAGS) $(DEBUG)

testy: test.c static
	@mkdir -p bin
	$(CC) test.c -o bin/testy lib/libaddressbook.a $(OPTIMIZATION_FLAG) $(DEBUG) $(CFLAGS)

test-static: static
	@mkdir -p bin
	$(CC) test.c -o bin/test-static lib/libaddressbook.a $(OPTIMIZATION_FLAG) $(DEBUG)

test-shared: shared
	@mkdir -p bin
	$(CC) test.c -o bin/test-shared -L./lib -laddressbook -Wl,-rpath,./lib $(OPTIMIZATION_FLAG) $(DEBUG)

test-dynamic: shared
	@mkdir -p bin
	$(CC) testDyn.c -o bin/test-shared -ldl $(OPTIMIZATION_FLAG)

measure: test-shared
	./bin/test-shared > results.txt

run: test-shared
	./bin/test-shared

zad3-O0: OPTIMIZATION_FLAG = -O0
zad3-O0: test-shared

zad3-O1: OPTIMIZATION_FLAG = -O1
zad3-O1: test-shared

zad3-O2: OPTIMIZATION_FLAG = -O2
zad3-O2: test-shared

zad3-O3: OPTIMIZATION_FLAG = -O3
zad3-O3: test-shared

zad3-Os: OPTIMIZATION_FLAG = -Os
zad3-Os: test-shared

measure-optimization: test-shared
	@make zad3-O0
	@echo "----------------------------- o0: -----------------------" > results.txt
	@./bin/test-shared >> results.txt
	@make zad3-O1
	@echo "----------------------------- o1: -----------------------" >> results.txt
	@./bin/test-shared >> results.txt
	@make zad3-O2
	@echo "----------------------------- o2: -----------------------" >> results.txt
	@./bin/test-shared >> results.txt
	@make zad3-O3
	@echo "----------------------------- o3: -----------------------" >> results.txt
	@./bin/test-shared >> results.txt
	@make zad3-Os
	@echo "----------------------------- os: -----------------------" >> results.txt
	@./bin/test-shared >> results.txt

debug: DEBUG = -g -O0
debug: test-static

clean:
	@find \( -name "*.o" -o\
	  		 -name "*.a" -o\
	 		 -name "*.so" -o\
			 -name "results.txt" -o\
			 -name "test-static" -o\
			 -name "test-shared"\
		  \)\
		  -exec rm -f {} \;
	@rm -rf bin
	@rm -rf lib
	@rm -rf src/obj

.PHONY: all shared static clean