/*
 * This file contains the public interface of the AST implementation used by
 * the parser defined here.  It contains a structure representing an AST node
 * along with functions for creating different kinds of AST nodes and a
 * function for freeing all the memory belonging to an AST node.
 */

#ifndef __AST_H
#define __AST_H

/**
 * This structure is used to represent a node in an AST.  It is generic, and
 * there are more specialized structures defined in ast.c to represent
 * specific types of constructs within the AST implementation.
 */
struct ast_node;

/**
 * Frees all memory belonging to an AST node, including all nodes in its
 * subtree.
 */
void ast_node_free(struct ast_node* node);

/**
 * Allocate, initialize, and return a new identifier expression AST node.
 *
 * @param id The text of the identifier represented by the new node.  The
 *   returned node should be considered to have taken ownership of this
 *   string.  It will be freed by ast_node_free().
 */
struct ast_node* id_expr_node_create(char* id);

/**
 * Allocate, initialize, and return a new float expression AST node.
 *
 * @param val The value of the float represented by the new node.
 */
struct ast_node* float_expr_node_create(float val);

/**
 * Allocate, initialize, and return a new integer expression AST node.
 *
 * @param val The value of the integer represented by the new node.
 */
struct ast_node* int_expr_node_create(int val);

/**
 * Allocate, initialize, and return a new boolean expression AST node.
 *
 * @param val The value of the boolean represented by the new node.
 */
struct ast_node* bool_expr_node_create(int val);

/**
 * Allocate, initialize, and return a new binary operation expression AST node.
 *
 * @param op An integer value representing the type of operation being
 *   performed in this expression.  This value should come from parser.h
 *   (e.g. PLUS, MINUS, GTE, etc.).
 * @param lhs The AST node representing the left-hand side of the new node.
 *   The returned node should be considered to have taken ownership of this
 *   node.  It will be freed by ast_node_free().  It is also freed by this
 *   function if `rhs` is NULL and NULL is returned here.
 * @param rhs The AST node representing the right-hand side of the new node.
 *   The returned node should be considered to have taken ownership of this
 *   node.  It will be freed by ast_node_free().  It is also freed by this
 *   function if `lhs` is NULL and NULL is returned here.
 *
 * @return If `lsh` or `rhs` is NULL, this function returns NULL.  Otherwise,
 *   it returns an AST node representing the binary operation expression.
 */
struct ast_node* binop_expr_node_create(
    int op,
    struct ast_node* lhs,
    struct ast_node* rhs
);

/**
 * Allocate, initialize, and return a new assignment statement AST node.
 *
 * @param lhs The text of the identifier on the left-hand side of the
 *   assignment statement represented by the new node.  The returned node
 *   should be considered to have taken ownership of this string.  It will be
 *   freed by ast_node_free().  It is also freed by this function if `rhs` is
 *   NULL and NULL is returned here.
 * @param rhs The AST node representing the right-hand side of the new node.
 *   The returned node should be considered to have taken ownership of this
 *   node.  It will be freed by ast_node_free().
 *
 * @return If `rhs` is NULL, this function returns NULL.  Otherwise, it returns
 *   an AST node representing the assignment statement.
 */
struct ast_node* assign_stmt_node_create(char* lhs, struct ast_node* rhs);

/**
 * Allocate, initialize, and return a new if statement AST node.
 *
 * @param condition The AST node representing the conditional expression
 *   attached to the if block of the new node.  The returned node should be
 *   considered to have taken ownership of this node.  It will be freed by
 *   ast_node_free().
 * @param if_block The AST node representing the block of statements attached
 *   to the if branch of the new node.  The returned node should be considered
 *   to have taken ownership of this node.  It will be freed by ast_node_free().
 *   It is also freed by this function if `condition` is NULL and NULL is
 *   returned here.
 * @param else_block The AST node representing the block of statements attached
 *   to the else branch of the new node.  The returned node should be considered
 *   to have taken ownership of this node.  It will be freed by ast_node_free().
 *   It is also freed by this function if `condition` is NULL and NULL is
 *   returned here.
 *
 * @return If `condition` is NULL, this function returns NULL.  Otherwise, it
 *   returns an AST node representing the if statement.
 */
struct ast_node* if_stmt_node_create(
    struct ast_node* condition,
    struct ast_node* if_block,
    struct ast_node* else_block
);

/**
 * Allocate, initialize, and return a new block AST node.
 *
 * @param first_stmt The AST node representing the first statement within the
 *   new node.  The returned node should be considered to have taken ownership
 *   of this node.  It will be freed by ast_node_free().  If this argument is
 *   NULL, it is ignored.
 */
struct ast_node* block_node_create(struct ast_node* first_stmt);

/**
 * Adds a single new statement to the end of the list of statements contained
 * within a block.
 *
 * @param block The AST node representing an existing block to which to add a
 *   statement.
 * @param stmt The AST node representing the statement to be added to the list
 *   of statements represented by `block`.  The node `block` should be
 *   considered to have taken ownership of this node.  It will be freed by
 *   ast_node_free().  If this argument is NULL, it is ignored.
 */
void block_node_append_stmt(struct ast_node* block, struct ast_node* stmt);

/**
 * Allocate, initialize, and return a new while statement AST node.
 *
 * @param condition The AST node representing the conditional expression
 *   attached to the new node.  The returned node should be considered to have
 *   taken ownership of this node.  It will be freed by ast_node_free().
 * @param block The AST node representing the block of statements attached
 *   to the new node.  The returned node should be considered to have taken
 *   ownership of this node.  It will be freed by ast_node_free().  It is also
 *   freed by this function if `condition` is NULL and NULL is returned here.
 *
 * @return If `condition` is NULL, this function returns NULL.  Otherwise, it
 *   returns an AST node representing the if statement.
 */
struct ast_node* while_stmt_node_create(
    struct ast_node* condition,
    struct ast_node* block
);

/**
 * Allocate, initialize, and return a new break statement AST node.
 */
struct ast_node* break_stmt_node_create();

/**
 * This function generates a GraphViz digraph specification for the AST
 * represented by a given root node.
 *
 * @param root The root node of an AST for which to generate a digraph.
 *
 * @return Returns a string containing the complete GraphViz digraph
 *   specification for `root` and its entire subtree.
 */
char* generate_graphviz(struct ast_node* n);

#endif
