/*
 * This file contains implementations of functions for creating and freeing
 * AST nodes.  Many of the functions defined in this file are part of the
 * public interface of the AST.  However, there are also some internal
 * functions defined here.  Internal functions are marked `static`, and their
 * names begin with an underscore.
 */

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "_ast_internal.h"

/*
 * Allocate, initialize, and return a new identifier expression AST node.
 *
 * @param id The text of the identifier represented by the new node.  The
 *   returned node should be considered to have taken ownership of this
 *   string.  It will be freed by ast_node_free().
 */
struct ast_node* id_expr_node_create(char* id) {
    struct _id_expr_node* id_expr_node = malloc(sizeof(struct _id_expr_node));
    id_expr_node->id = id;
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = ID_EXPR;
    node->node_data.id_expr = id_expr_node;
    return node;
}

/*
 * Allocate, initialize, and return a new float expression AST node.
 *
 * @param val The value of the float represented by the new node.
 */
struct ast_node* float_expr_node_create(float val) {
    struct _float_expr_node* float_expr_node =
        malloc(sizeof(struct _float_expr_node));
    float_expr_node->val = val;
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = FLOAT_EXPR;
    node->node_data.float_expr = float_expr_node;
    return node;
}

/*
 * Allocate, initialize, and return a new integer expression AST node.
 *
 * @param val The value of the integer represented by the new node.
 */
struct ast_node* int_expr_node_create(int val) {
    struct _int_expr_node* int_expr_node =
        malloc(sizeof(struct _int_expr_node));
    int_expr_node->val = val;
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = INT_EXPR;
    node->node_data.int_expr = int_expr_node;
    return node;
}

/*
 * Allocate, initialize, and return a new boolean expression AST node.
 *
 * @param val The value of the boolean represented by the new node.
 */
struct ast_node* bool_expr_node_create(int val) {
    struct _bool_expr_node* bool_expr_node =
        malloc(sizeof(struct _bool_expr_node));
    bool_expr_node->val = val;
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = BOOL_EXPR;
    node->node_data.bool_expr = bool_expr_node;
    return node;
}

/*
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
) {
    if (!lhs || !rhs) {
        ast_node_free(lhs);
        ast_node_free(rhs);
        return NULL;
    } else {
        struct _binop_expr_node* binop_expr_node =
            malloc(sizeof(struct _binop_expr_node));
        binop_expr_node->op = op;
        binop_expr_node->lhs = lhs;
        binop_expr_node->rhs = rhs;
        struct ast_node* node = malloc(sizeof(struct ast_node));
        node->type = BINOP_EXPR;
        node->node_data.binop_expr = binop_expr_node;
        return node;
    }

}

/*
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
struct ast_node* assign_stmt_node_create(char* lhs, struct ast_node* rhs) {
    if (!rhs) {
        free(lhs);
        return NULL;
    } else {
        struct _assign_stmt_node* assign_stmt_node =
            malloc(sizeof(struct _assign_stmt_node));
        assign_stmt_node->lhs = lhs;
        assign_stmt_node->rhs = rhs;
        struct ast_node* node = malloc(sizeof(struct ast_node));
        node->type = ASSIGN_STMT;
        node->node_data.assign_stmt = assign_stmt_node;
        return node;
    }
}

/*
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
) {
    if (!condition) {
        ast_node_free(if_block);
        ast_node_free(else_block);
        return NULL;
    } else {
        struct _if_stmt_node* if_stmt_node =
            malloc(sizeof(struct _if_stmt_node));
        if_stmt_node->condition = condition;
        if_stmt_node->if_block = if_block;
        if_stmt_node->else_block = else_block;
        struct ast_node* node = malloc(sizeof(struct ast_node));
        node->type = IF_STMT;
        node->node_data.if_stmt = if_stmt_node;
        return node;
    }
}

/*
 * Allocate, initialize, and return a new block AST node.
 *
 * @param first_stmt The AST node representing the first statement within the
 *   new node.  The returned node should be considered to have taken ownership
 *   of this node.  It will be freed by ast_node_free().  If this argument is
 *   NULL, it is ignored.
 */
struct ast_node* block_node_create(struct ast_node* first_stmt) {
    struct _block_node* block_node = malloc(sizeof(struct _block_node));
    if (first_stmt) {
        block_node->stmts[0] = first_stmt;
        block_node->n_stmts = 1;
    } else {
        block_node->n_stmts = 0;
    }
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = BLOCK;
    node->node_data.block = block_node;
    return node;
}

/*
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
void block_node_append_stmt(struct ast_node* block, struct ast_node* stmt) {
    struct _block_node* block_node = block->node_data.block;
    if (block_node->n_stmts >= AST_NODE_MAX_CHILDREN) {
        fprintf(stderr, "FATAL ERROR: too many statements added to block\n");
        exit(1);
    }
    if (stmt) {
        block_node->stmts[block_node->n_stmts] = stmt;
        block_node->n_stmts++;
    }
}

/*
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
) {
    if (condition) {
        struct _while_stmt_node* while_stmt_node =
            malloc(sizeof(struct _while_stmt_node));
        while_stmt_node->condition = condition;
        while_stmt_node->block = block;
        struct ast_node* node = malloc(sizeof(struct ast_node));
        node->type = WHILE_STMT;
        node->node_data.while_stmt = while_stmt_node;
        return node;
    } else {
        ast_node_free(block);
        return NULL;
    }
}

/*
 * Allocate, initialize, and return a new break statement AST node.
 */
struct ast_node* break_stmt_node_create() {
    struct ast_node* node = malloc(sizeof(struct ast_node));
    node->type = BREAK_STMT;
    return node;
}


/*****************************************************************************
 **
 ** AST node freeing functions
 **
 *****************************************************************************/

/*
 * Frees all memory belonging to an identifier expression AST node, including
 * the string representing the text of the identifier.
 */
static void _id_expr_node_free(struct _id_expr_node* node) {
    free(node->id);
    free(node);
}

/*
 * Frees all memory belonging to a float expression AST node.
 */
static void _float_expr_node_free(struct _float_expr_node* node) {
    free(node);
}

/*
 * Frees all memory belonging to a integer expression AST node.
 */
static void _int_expr_node_free(struct _int_expr_node* node) {
    free(node);
}

/*
 * Frees all memory belonging to a boolean expression AST node.
 */
static void _bool_expr_node_free(struct _bool_expr_node* node) {
    free(node);
}

/*
 * Frees all memory belonging to a binary operation expression AST node,
 * including its descendents.
 */
static void _binop_expr_node_free(struct _binop_expr_node* node) {
    ast_node_free(node->lhs);
    ast_node_free(node->rhs);
    free(node);
}

/*
 * Frees all memory belonging to an assignment statement AST node, including
 * its descendents.
 */
static void _assign_stmt_node_free(struct _assign_stmt_node* node) {
    free(node->lhs);
    ast_node_free(node->rhs);
    free(node);
}

/*
 * Frees all memory belonging to an if statement AST node, including its
 * descendents.
 */
static void _if_stmt_node_free(struct _if_stmt_node* node) {
    ast_node_free(node->condition);
    ast_node_free(node->if_block);
    if (node->else_block) {
        ast_node_free(node->else_block);
    }
    free(node);
}

/*
 * Frees all memory belonging to an AST node representing a block, including
 * its descendents.
 */
static void _block_node_free(struct _block_node* node) {
    for (int i = 0; i < node->n_stmts; i++) {
        ast_node_free(node->stmts[i]);
    }
    free(node);
}

/*
 * Frees all memory belonging to an while statement AST node, including its
 * descendents.
 */
static void _while_stmt_node_free(struct _while_stmt_node* node) {
    ast_node_free(node->condition);
    ast_node_free(node->block);
    free(node);
}

/*
 * Frees all memory belonging to an AST node, including all nodes in its
 * subtree.
 */
void ast_node_free(struct ast_node* node) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case ID_EXPR:
            _id_expr_node_free(node->node_data.id_expr);
            break;
        case FLOAT_EXPR:
            _float_expr_node_free(node->node_data.float_expr);
            break;
        case INT_EXPR:
            _int_expr_node_free(node->node_data.int_expr);
            break;
        case BOOL_EXPR:
            _bool_expr_node_free(node->node_data.bool_expr);
            break;
        case BINOP_EXPR:
            _binop_expr_node_free(node->node_data.binop_expr);
            break;
        case ASSIGN_STMT:
            _assign_stmt_node_free(node->node_data.assign_stmt);
            break;
        case IF_STMT:
            _if_stmt_node_free(node->node_data.if_stmt);
            break;
        case BLOCK:
            _block_node_free(node->node_data.block);
            break;
        case WHILE_STMT:
            _while_stmt_node_free(node->node_data.while_stmt);
            break;
        default:
            break;
    }
    free(node);
}
