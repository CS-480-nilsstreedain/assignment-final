/*
 * This file contains implementations of functions for generating a GraphViz
 * specification to visualize an AST.  Some of the functions defined in this
 * file are part of the public interface of the AST.  However, there are also
 * several internal functions defined here.  Internal functions are marked
 * `static`, and their names begin with an underscore.
 */

#include <stdlib.h>

#include "_ast_internal.h"
#include "../lib/strutils.h"
#include "../parser.h"

/*
 * This is a forward declaration of this function, which will be defined later.
 * We need to call the function before we define it.
 */
static char* _ast_node_graphviz(struct ast_node* node, char* name);

/*
 * Generates a GraphViz string representing a single leaf node in an AST.
 *
 * @param name The GraphViz ID of the node.  This must be unique among all nodes
 *   in the graph.
 * @param label The main label to be printed inside the generated node.
 * @param sublabel A sublabel to be printed under the main label.  This may be
 *   NULL, in which case a sublabel is not included.
 *
 * @return Returns a string of GraphViz representing the specified node.  The
 *   string will take one of the two following forms, depending on whether or
 *   not a sublabel is specified:
 *
 *       {name} [shape=box,label="{label}\n{sublabel}"];
 *
 *       {name} [shape=box,label="{label}"];
 */
static char* _graphviz_leaf_node(char* name, char* label, char* sublabel) {
    if (sublabel) {
        return concat_strings(7,
            "\t", name, " [shape=box,label=\"", label, "\\n", sublabel, "\"];\n"
        );
    } else {
        return concat_strings(5,
            "\t", name, " [shape=box,label=\"", label, "\"];\n"
        );
    }
}

/*
 * Generates a GraphViz string representing a single internal node in an AST.
 *
 * @param name The GraphViz ID of the node.  This must be unique among all nodes
 *   in the graph.
 * @param label The main label to be printed inside the generated node.
 * @param sublabel A sublabel to be printed under the main label.  This may be
 *   NULL, in which case a sublabel is not included.
 *
 * @return Returns a string of GraphViz representing the specified node.  The
 *   string will take one of the two following forms, depending on whether or
 *   not a sublabel is specified:
 *
 *       {name} [label="{label}\n{sublabel}"];
 *
 *       {name} [label="{label}"];
 */
static char* _graphviz_internal_node(char* name, char* label, char* sublabel) {
    if (sublabel) {
        return concat_strings(7,
            "\t", name, " [label=\"", label, "\\n", sublabel, "\"];\n"
        );
    } else {
        return concat_strings(5,
            "\t", name, " [label=\"", label, "\"];\n"
        );
    }
}

/*
 * Generates a GraphViz string representing a single edge in an AST.
 *
 * @param tailname The GraphViz ID of the node at the tail of the edge.
 * @param headname The GraphViz ID of the node at the head of the edge.
 * @param label A label to be printed at the tail of the edge .  This may be
 *   NULL, in which case a label is not included.
 *
 * @return Returns a string of GraphViz representing the specified edge.  The
 *   string will take one of the two following forms, depending on whether or
 *   not a label is specified:
 *
 *       {tailname} -> {headname} [taillabel="{label}"];
 *
 *       {tailname} -> {headname};
 */
static char* _graphviz_edge(char* tailname, char* headname, char* label) {
    if (label) {
        return concat_strings(7,
            "\t", tailname, " -> ", headname, " [taillabel=\"", label, "\"];\n"
        );
    } else {
        return concat_strings(5,
            "\t", tailname, " -> ", headname, ";\n"
        );
    }
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing an identifier expression.
 *
 * @param node The identifier expression node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _id_expr_node_graphviz(struct _id_expr_node* node, char* name) {
    return _graphviz_leaf_node(name, "IDENTIFIER", node->id);
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a float expression.
 *
 * @param node The float expression node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _float_expr_node_graphviz(
    struct _float_expr_node* node,
    char* name
) {
    char* val_str = float_to_str(node->val);
    char* gv = _graphviz_leaf_node(name, "FLOAT", val_str);
    free(val_str);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing an integer expression.
 *
 * @param node The integer expression node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _int_expr_node_graphviz(struct _int_expr_node* node, char* name) {
    char* val_str = int_to_str(node->val);
    char* gv = _graphviz_leaf_node(name, "INTEGER", val_str);
    free(val_str);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a boolean expression.
 *
 * @param node The boolean expression node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _bool_expr_node_graphviz(
    struct _bool_expr_node* node,
    char* name
) {
    char* val_str = int_to_str(node->val);
    char* gv = _graphviz_leaf_node(name, "BOOLEAN", val_str);
    free(val_str);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a binary operation expression.
 *
 * @param node The binary operation expression node for which to generate
 *   GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _binop_expr_node_graphviz(
    struct _binop_expr_node* node,
    char* name
) {
    /*
     * Figure out what string to use to represent the binary operation.
     */
    char* op_str;
    switch (node->op) {
        case PLUS:
            op_str = "PLUS";
            break;
        case MINUS:
            op_str = "MINUS";
            break;
        case TIMES:
            op_str = "TIMES";
            break;
        case DIVIDEDBY:
            op_str = "DIVIDEDBY";
            break;
        case EQ:
            op_str = "EQ";
            break;
        case NEQ:
            op_str = "NEQ";
            break;
        case GT:
            op_str = "GT";
            break;
        case GTE:
            op_str = "GTE";
            break;
        case LT:
            op_str = "LT";
            break;
        case LTE:
            op_str = "LTE";
            break;
        case AND:
            op_str = "AND";
            break;
        case OR:
            op_str = "OR";
            break;
    }
    char* node_gv = _graphviz_internal_node(name, op_str, NULL);

    char* lhs_name = concat_strings(2, name, "_lhs");
    char* lhs_edge_gv = _graphviz_edge(name, lhs_name, NULL);
    char* lhs_subtree_gv = _ast_node_graphviz(node->lhs, lhs_name);

    char* rhs_name = concat_strings(2, name, "_rhs");
    char* rhs_edge_gv = _graphviz_edge(name, rhs_name, NULL);
    char* rhs_subtree_gv = _ast_node_graphviz(node->rhs, rhs_name);

    char* gv = concat_strings(5, node_gv, lhs_edge_gv, lhs_subtree_gv,
        rhs_edge_gv, rhs_subtree_gv);

    free(node_gv);
    free(lhs_name);
    free(lhs_edge_gv);
    free(lhs_subtree_gv);
    free(rhs_name);
    free(rhs_edge_gv);
    free(rhs_subtree_gv);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing an assignment statement.
 *
 * @param node The assignment statement node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _assign_stmt_node_graphviz(
    struct _assign_stmt_node* node,
    char* name
) {
    char* node_gv = _graphviz_internal_node(name, "ASSIGNMENT", node->lhs);
    char* rhs_name = concat_strings(2, name, "_rhs");
    char* rhs_edge_gv = _graphviz_edge(name, rhs_name, NULL);
    char* rhs_subtree_gv = _ast_node_graphviz(node->rhs, rhs_name);

    char* gv = concat_strings(3, node_gv, rhs_edge_gv, rhs_subtree_gv);

    free(node_gv);
    free(rhs_name);
    free(rhs_edge_gv);
    free(rhs_subtree_gv);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a block of statements.
 *
 * @param node The block node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _block_node_graphviz(struct _block_node* node, char* name) {
    char* gv = _graphviz_internal_node(name, "BLOCK", NULL);

    for (int i = 0; i < node->n_stmts; i++) {
        char* stmt_name = concat_strings(3, name, "_", int_to_str(i));
        char* stmt_edge_gv = _graphviz_edge(name, stmt_name, NULL);
        char* stmt_subtree_gv = _ast_node_graphviz(node->stmts[i], stmt_name);

        char* old_gv = gv;
        gv = concat_strings(3, old_gv, stmt_edge_gv, stmt_subtree_gv);

        free(old_gv);
        free(stmt_name);
        free(stmt_edge_gv);
        free(stmt_subtree_gv);
    }
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing an if statement.
 *
 * @param node The if statement node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _if_stmt_node_graphviz(struct _if_stmt_node* node, char* name) {
    char* node_gv = _graphviz_internal_node(name, "IF", NULL);

    char* cond_name = concat_strings(2, name, "_cond");
    char* cond_edge_gv = _graphviz_edge(name, cond_name, "cond");
    char* cond_subtree_gv = _ast_node_graphviz(node->condition, cond_name);

    char* if_block_name = concat_strings(2, name, "_block");
    char* if_block_edge_gv = _graphviz_edge(name, if_block_name, "if");
    char* if_block_subtree_gv = _ast_node_graphviz(node->if_block, if_block_name);

    char* gv = concat_strings(5, node_gv, cond_edge_gv, cond_subtree_gv,
        if_block_edge_gv, if_block_subtree_gv);

    if (node->else_block) {
        char* else_name = concat_strings(2, name, "_else");
        char* else_edge_gv = _graphviz_edge(name, else_name, "else");
        char* else_subtree_gv = _ast_node_graphviz(node->else_block, else_name);
        char* old_gv = gv;
        gv = concat_strings(3, old_gv, else_edge_gv, else_subtree_gv);
        free(else_name);
        free(else_edge_gv);
        free(else_subtree_gv);
        free(old_gv);
    }

    free(node_gv);
    free(cond_name);
    free(cond_edge_gv);
    free(cond_subtree_gv);
    free(if_block_name);
    free(if_block_edge_gv);
    free(if_block_subtree_gv);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a while statement.
 *
 * @param node The while statement node for which to generate GraphViz.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _while_stmt_node_graphviz(
    struct _while_stmt_node* node,
    char* name
) {
    char* node_gv = _graphviz_internal_node(name, "WHILE", NULL);

    char* cond_name = concat_strings(2, name, "_cond");
    char* cond_edge_gv = _graphviz_edge(name, cond_name, "cond");
    char* cond_subtree_gv = _ast_node_graphviz(node->condition, cond_name);

    char* block_name = concat_strings(2, name, "_block");
    char* block_edge_gv = _graphviz_edge(name, block_name, NULL);
    char* block_subtree_gv = _ast_node_graphviz(node->block, block_name);

    char* gv = concat_strings(5, node_gv, cond_edge_gv, cond_subtree_gv,
        block_edge_gv, block_subtree_gv);

    free(node_gv);
    free(cond_name);
    free(cond_edge_gv);
    free(cond_subtree_gv);
    free(block_name);
    free(block_edge_gv);
    free(block_subtree_gv);
    return gv;
}

/*
 * Generates and returns the GraphViz specification for an AST node
 * representing a break statement.
 *
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for a break statement node.
 */
static char* _break_stmt_node_graphviz(char* name) {
    return _graphviz_internal_node(name, "BREAK", NULL);
}

/*
 * This function generates the GraphViz specification for the AST node and all
 * nodes in its subtree.  The returned string includes specifications for all
 * nodes and edges in the subtree under the specified node (including the node
 * itself), but it must eventually be wrapped in a GraphViz `digraph` to be
 * valid.
 *
 * @param node A node in an AST.  The GraphViz specification for this node and
 *   its entire subtree is generated.
 * @param name The name to use for this node in the generated GraphViz
 *   specification.
 *
 * @return Returns a string containing the complete GraphViz specification
 *   for `node` and its entire subtree.
 */
static char* _ast_node_graphviz(struct ast_node* node, char* name) {
    if (!node) {
        /*
         * Return a free-able empty string if node is NULL.
         */
        return concat_strings(1, "");
    }

    /*
     * Determine what type of node this is and then generate the appropriate
     * kind of GraphViz for it.
     */
    switch (node->type) {
        case ID_EXPR:
            return _id_expr_node_graphviz(node->node_data.id_expr, name);
        case FLOAT_EXPR:
            return _float_expr_node_graphviz(node->node_data.float_expr, name);
        case INT_EXPR:
            return _int_expr_node_graphviz(node->node_data.int_expr, name);
        case BOOL_EXPR:
            return _bool_expr_node_graphviz(node->node_data.bool_expr, name);
        case BINOP_EXPR:
            return _binop_expr_node_graphviz(node->node_data.binop_expr, name);
        case ASSIGN_STMT:
            return _assign_stmt_node_graphviz(node->node_data.assign_stmt, name);
        case IF_STMT:
            return _if_stmt_node_graphviz(node->node_data.if_stmt, name);
        case BLOCK:
            return _block_node_graphviz(node->node_data.block, name);
        case WHILE_STMT:
            return _while_stmt_node_graphviz(node->node_data.while_stmt, name);
        case BREAK_STMT:
            return _break_stmt_node_graphviz(name);
        default:
            return concat_strings(1, "");
    }
}

/*
 * This function generates a GraphViz digraph specification for the AST
 * represented by a given root node.
 *
 * @param root The root node of an AST for which to generate a digraph.
 *
 * @return Returns a string containing the complete GraphViz digraph
 *   specification for `root` and its entire subtree.
 */
char* generate_graphviz(struct ast_node* root) {
    char* tree_spec = _ast_node_graphviz(root, "n0");
    char* full_spec = concat_strings(3, "digraph AST {\n", tree_spec, "}\n");
    free(tree_spec);
    return full_spec;
}
