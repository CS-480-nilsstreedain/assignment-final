/*
 * This file contains shared definitions internal to the AST implementation.
 * This file is not intended to be included outside the AST implementation
 * itself.
 */

#ifndef __AST_INTERNAL_H
#define __AST_INTERNAL_H

/*
 * For simplicity, we limit the maximum number of children any node can have.
 * Note that this is *very* limiting, e.g. it limits the number of statements
 * within a block or program.  We'd definitely want to do something different
 * to handle bigger programs, e.g. store the children of a node in a dynamic
 * array/vector.
 */
#define AST_NODE_MAX_CHILDREN 16

/*
 * This enumeration provides a numerical code for each different type of AST
 * node.
 */
enum _ast_node_type {
    ID_EXPR,
    FLOAT_EXPR,
    INT_EXPR,
    BOOL_EXPR,
    BINOP_EXPR,
    ASSIGN_STMT,
    IF_STMT,
    WHILE_STMT,
    BREAK_STMT,
    BLOCK
};

/*****************************************************************************
 **
 ** AST node structures
 **
 *****************************************************************************/

/*
 * This is an AST node specifically representing an identifier expression.
 *
 * @var id The text of the identifier represented by this node.
 */
struct _id_expr_node {
    char* id;
};

/*
 * This is an AST node specifically representing a float expression.
 *
 * @var val The floating point value represented by this node.
 */
struct _float_expr_node {
    float val;
};

/*
 * This is an AST node specifically representing an integer expression.
 *
 * @var val The integer point value represented by this node.
 */
struct _int_expr_node {
    int val;
};

/*
 * This is an AST node specifically representing a boolean expression.
 *
 * @var val The boolean point value represented by this node.
 */
struct _bool_expr_node {
    int val;
};

/*
 * This is an AST node specifically representing a binary operation expression.
 *
 * @var op An integer value representing the type of operation being
 *   performed in this expression.  This value should come from parser.h
 *   (e.g. PLUS, MINUS, GTE, etc.).
 * @var lhs The AST node representing the left-hand side of this expression.
 * @var rhs The AST node representing the right-hand side of this expression.
 */
struct _binop_expr_node {
    int op;
    struct ast_node* lhs;
    struct ast_node* rhs;
};

/*
 * This is an AST node specifically representing an assignment statement.
 *
 * @var lhs The string representing the identifier on the left-hand side of
 *   this statement.
 * @var rhs The AST node representing the right-hand side of this statement.
 */
struct _assign_stmt_node {
    char* lhs;
    struct ast_node* rhs;
};

/*
 * This is an AST node specifically representing a block of statements.
 *
 * @var stmts An array containing the statements within this block.
 * @var n_stmts The number of statements in this block.
 */
struct _block_node {
    struct ast_node* stmts[AST_NODE_MAX_CHILDREN];
    int n_stmts;
};

/*
 * This is an AST node specifically representing an assignment statement.
 *
 * @var condition The conditional expression attached to the if block of this
 *   if statement.
 * @var if_block The block of statements representing the if branch of this
 *   if statement.
 * @var else_block The block of statements representing the else branch of this
 *   if statement.
 */
struct _if_stmt_node {
    struct ast_node* condition;
    struct ast_node* if_block;
    struct ast_node* else_block;
};

/*
 * This is an AST node specifically representing a while statement.
 *
 * @var condition The conditional expression attached to this while statement.
 * @var block The block of statements representing the body of this while
 *   statement.
 */
struct _while_stmt_node {
    struct ast_node* condition;
    struct ast_node* block;
};


/*
 * This structure is used to represent a node in an AST.  It is generic, and
 * there are more specialized structures defined in ast.c.
 *
 * @var type An integer value from `enum ast_node_type` above denoting the
 *   type of this AST node.
 * @var node The actual underlying AST node data for this node.  This is
 *   represented as a union so it can represent any specific type of node.
 */
struct ast_node {
    int type;
    union ast_nodes {
        struct _id_expr_node* id_expr;
        struct _float_expr_node* float_expr;
        struct _int_expr_node* int_expr;
        struct _bool_expr_node* bool_expr;
        struct _binop_expr_node* binop_expr;
        struct _assign_stmt_node* assign_stmt;
        struct _block_node* block;
        struct _if_stmt_node* if_stmt;
        struct _while_stmt_node* while_stmt;
    } node_data;
};


#endif
