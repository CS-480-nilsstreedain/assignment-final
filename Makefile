#
# Figure out what the name of the llvm-config executable is (assuming a limited
# number of options for the common environments in which we'll be building this
# code for this course).
#
LLVM_CONFIG := $(shell which llvm-config-7.0-64)
ifndef LLVM_CONFIG
	LLVM_CONFIG := $(shell which llvm-config-13)
endif
ifndef LLVM_CONFIG
	LLVM_CONFIG := llvm-config
endif

CC=gcc --std=c99
CXX=g++

all: compile

compile: main.o parser.o scanner.o ast_create.o ast_graphviz.o ast_llvm.o hash.o strutils.o
	$(CXX) main.o parser.o scanner.o ast_create.o ast_graphviz.o ast_llvm.o hash.o strutils.o	\
		$(shell $(LLVM_CONFIG) --cppflags --ldflags --libs --system-libs all)	\
		 -o compile

scanner.c: scanner.l
	flex -o scanner.c scanner.l

parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

main.o: main.c
	$(CC) main.c -c -o main.o

scanner.o: scanner.c
	$(CC) scanner.c -c -o scanner.o

parser.o: parser.c
	$(CC) parser.c -c -o parser.o

ast_llvm.o: ast/ast_llvm.c ast/ast.h ast/_ast_internal.h
	$(CC) $(shell $(LLVM_CONFIG) --cflags) ast/ast_llvm.c -c -o ast_llvm.o

ast_create.o: ast/ast_create.c ast/ast.h ast/_ast_internal.h
	$(CC) ast/ast_create.c -c -o ast_create.o

ast_graphviz.o: ast/ast_graphviz.c ast/ast.h ast/_ast_internal.h parser.h
	$(CC) ast/ast_graphviz.c -c -o ast_graphviz.o

hash.o: lib/hash.c lib/hash.h
	$(CC) lib/hash.c -c -o hash.o

strutils.o: lib/strutils.c lib/strutils.h
	$(CC) lib/strutils.c -c -o strutils.o

clean:
	rm -f compile scanner.c parser.c parser.h *.o
