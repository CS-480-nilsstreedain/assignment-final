/*
 * This is the driver program for the compiler.  It runs the scanner/parser
 * combination by calling yylex(), and if an AST is successfully generated,
 * it generates GraphViz code for that AST and prints it to stdout.
 */

#include <stdio.h>
#include <stdlib.h>

#include "lib/hash.h"
#include "ast/ast.h"

/*
 * These symbols are needed in main() but are defined elsewhere.
 */
extern int yylex();
extern struct ast_node* ast;

/*
 * This symbol table is used by the parser to keep track of valid variable
 * names.  Once parsing is done, you could repurpose this symbol table to use
 * during LLVM code generation if you wanted to.
 */
struct hash* symbols;


int main(int argc, char const *argv[]) {
    symbols = hash_create();
    if (!yylex()) {
        if (ast) {
            char* llvm_ir = generate_llvm_ir(ast);
            printf("%s", llvm_ir);
            if (argc > 1)
                generate_object_code(llvm_ir, argv[1]);
            free(llvm_ir);
            ast_node_free(ast);
        }
    }
    hash_free(symbols);
    return 0;
}
