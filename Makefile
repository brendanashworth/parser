CC ?= gcc
CC_FLAGS ?= -g

test:
	$(CC) $(CC_FLAGS) src/parser.c -I. test/test-parser-basic.c -o test/test-parser-basic.o
	test/test-parser-basic.o
	$(CC) $(CC_FLAGS) src/parser.c -I. test/test-parser-error.c -o test/test-parser-error.o
	test/test-parser-error.o

bench:
	$(CC) $(CC_FLAGS) -I. -Ideps  src/parser.c benchmark/simple.c -o benchmark/simple.o
	benchmark/simple.o

.PHONY: test bench