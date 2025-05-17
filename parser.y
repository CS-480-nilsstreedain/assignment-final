%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/hash.h"
#include "ast/ast.h"
#include "parser.h"

void yyerror(YYLTYPE* loc, const char* err);
int py_bool_to_int(char* py_bool);

/*
 * Here, `ast` is an AST node that will represent the root of the generated
 * AST for the source program.
 */
struct ast_node* ast = NULL;

/*
 * Here, symbols is a hash table used to keep track of all unique identifiers.
 * It will be used to figure out what variables to initialize in the final
 * generated C program.  This is just an `extern` reference to symbols, which
 * is actually defined in main.c.
 */
extern struct hash* symbols;

int have_err = 0;
%}

/*
 * Enable location tracking and verbose error messages.
 */
%locations
%define parse.error verbose


/*
 * All nonterminals in the grammar will be represented as AST nodes.  All
 * tokens coming from the scanner will be represented as strings.
 */
%union {
    struct ast_node* node;
    char* str;
}

/*
 * Because the lexer can generate more than one token at a time (i.e. DEDENT
 * tokens), we'll use a push parser.
 */
%define api.pure full
%define api.push-pull push

/*
 * These are all of the terminals in our grammar, i.e. the syntactic
 * categories that can be recognized by the lexer.  We assign the `str`
 * type from the %union declaration above to all of them, since we'll send
 * lexemes as strings from the scanner.
 */
%token <str> IDENTIFIER
%token <str> FLOAT INTEGER BOOLEAN
%token <str> INDENT DEDENT NEWLINE
%token <str> AND BREAK DEF ELIF ELSE FOR IF NOT OR RETURN WHILE
%token <str> ASSIGN PLUS MINUS TIMES DIVIDEDBY
%token <str> EQ NEQ GT GTE LT LTE
%token <str> LPAREN RPAREN COMMA COLON

/*
 * Here we're assigning types to the nonterminals.  All of them will be
 * represented as AST nodes.
 */
%type <node> expression primary_expression condition
%type <node> statement assign_statement if_statement while_statement break_statement
%type <node> statements block else_block

/*
 * Here, we're defining the precedence of the operators.  The ones that appear
 * later have higher precedence.  All of the operators are left-associative
 * except the "not" operator, which is right-associative.
 */
%left OR
%left AND
%right NOT
%left EQ NEQ GT GTE LT LTE
%left PLUS MINUS
%left TIMES DIVIDEDBY

/*
 * This is our goal/start symbol.
 */
%start program

%%

/*
 * Each of the CFG rules below generates the relevant AST node and returns
 * it as the semantic value of the rule's left-hand side.  Since each of the
 * various nodes becomes incorporated into its parent node in the AST, we
 * don't free the nodes.  The only things we free below are strings from the
 * scanner that we no longer need (i.e. floats, booleans, and integers).
 */


/*
 * This is the goal/start symbol.  Once all of the statements in the entire
 * source program are translated, this symbol receives the string containing
 * all of the translations and assigns it to the global `ast`, so it can be
 * used outside the parser.
 */
program
  : statements { ast = $1; }
  ;

/*
 * The `statements` symbol represents a set of contiguous statements.  It is
 * used to represent the entire program in the rule above and to represent a
 * block of statements in the `block` rule below.  The first production here
 * creates an AST node to represent a new set of statements, and the second
 * production simply appends each new statement into the set represented by
 * that node.
 */
statements
  : statement { $$ = block_node_create($1); }
  | statements statement {
        block_node_append_stmt($1, $2);
        $$ = $1;
    }
  ;

/*
 * This is a high-level symbol used to represent an individual statement.
 */
statement
  : assign_statement { $$ = $1; }
  | if_statement { $$ = $1; }
  | while_statement { $$ = $1; }
  | break_statement { $$ = $1; }
  | error NEWLINE {
        $$ = NULL;
        have_err = 1;
    }
  ;

/*
 * A primary expression is a "building block" of an expression.
 */
primary_expression
  : IDENTIFIER {
        if (!hash_contains(symbols, $1)) {
            fprintf(stderr,
                "Error (line %d): unknown symbol '%s' used in expression.\n",
                @1.first_line, $1);
            have_err = 1;
            $$ = NULL;
        } else {
            $$ = id_expr_node_create($1);
        }
    }
  | FLOAT {
        $$ = float_expr_node_create(atof($1));
        free($1);
    }
  | INTEGER {
        $$ = int_expr_node_create(atoi($1));
        free($1);
    }
  | BOOLEAN {
        $$ = bool_expr_node_create(py_bool_to_int($1));
        free($1);
    }
  | LPAREN expression RPAREN { $$ = $2; }
  ;

/*
 * Symbol representing algebraic expressions.  For most forms of algebraic
 * expression, we generate an AST node that simply contains child nodes
 * representing of the operands along with a constant representing the operator
 * type.
 */
expression
  : primary_expression { $$ = $1; }
  | expression PLUS expression { $$ = binop_expr_node_create(PLUS, $1, $3); }
  | expression MINUS expression { $$ = binop_expr_node_create(MINUS, $1, $3); }
  | expression TIMES expression { $$ = binop_expr_node_create(TIMES, $1, $3); }
  | expression DIVIDEDBY expression { $$ = binop_expr_node_create(DIVIDEDBY, $1, $3); }
  | expression EQ expression { $$ = binop_expr_node_create(EQ, $1, $3); }
  | expression NEQ expression { $$ = binop_expr_node_create(NEQ, $1, $3); }
  | expression GT expression { $$ = binop_expr_node_create(GT, $1, $3); }
  | expression GTE expression { $$ = binop_expr_node_create(GTE, $1, $3); }
  | expression LT expression { $$ = binop_expr_node_create(LT, $1, $3); }
  | expression LTE expression { $$ = binop_expr_node_create(LTE, $1, $3); }
  ;

/*
 * This symbol represents an assignment statement.  For each assignment
 * statement, we first make sure to insert the LHS identifier into the global
 * symbol table, since it is potentially a new symbol.
 */
assign_statement
  : IDENTIFIER ASSIGN expression NEWLINE {
        hash_insert(symbols, $1, NULL);
        $$ = assign_stmt_node_create($1, $3);
    }
  ;

/*
 * A `block` represents the collection of statements associated with an if,
 * elif, else, or while statement.
 */
block
  : INDENT statements DEDENT { $$ = $2; }
  ;

/*
 * This symbol represents a boolean condition, used with an if, elif, or while.
 * The AST node for these conditions is represented like other binary
 * expressions.
 */
condition
  : expression { $$ = $1; }
  ;

/*
 * This symbol represents an entire if statement, including an optional else
 * block.
 */
if_statement
  : IF condition COLON NEWLINE block else_block {
        $$ = if_stmt_node_create($2, $5, $6);
    }
  ;

/*
 * This symbol represents an if statement's optional else block.
 */
else_block
  : %empty { $$ = NULL; }
  | ELSE COLON NEWLINE block { $$ = $4; }


/*
 * This symbol represents a while statement.  The C++ translation wraps the
 * while condition in parentheses.
 */
while_statement
  : WHILE condition COLON NEWLINE block { $$ = while_stmt_node_create($2, $5); }
  ;

/*
 * This symbol represents a break statement.  The C++ translation simply adds
 * a semicolon.
 */
break_statement
  : BREAK NEWLINE { $$ = break_stmt_node_create(); }
  ;

%%

/*
 * This is our simple error reporting function.  It prints the line number
 * and text of each error.
 */
void yyerror(YYLTYPE* loc, const char* err) {
    fprintf(stderr, "Error (line %d): %s\n", loc->first_line, err);
}


/*
 * This function translates a Python boolean value into the corresponding
 * integer value
 */
int py_bool_to_int(char* py_bool) {
    if (strcmp(py_bool, "True") == 0) {
        return 1;
    } else {
        return 0;
    }
}
