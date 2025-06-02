/*
 * You must implement all of your LLVM IR generation functions in this file
 * for the comilation process defined in the project's Makefile to work
 * correctly.  Feel free to add any new definitions to ast.h or _ast_internal.h
 * if you need to.
 */

#include <stdio.h>
#include <stdlib.h>
#include <llvm-c/Core.h>

#include "_ast_internal.h"
#include "../lib/hash.h"
#include "../parser.h"

// Global LLVM state
static LLVMContextRef context;
static LLVMModuleRef module;
static LLVMBuilderRef builder;
static LLVMValueRef target_function;
static LLVMBasicBlockRef break_target = NULL;

extern struct hash* symbols;

static LLVMValueRef gen_expr(struct ast_node* node);
static void gen_stmt(struct ast_node* node);

// Generate LLVM IR for expressions
static LLVMValueRef gen_expr(struct ast_node* node) {
    if (node->type == ID_EXPR)
        return LLVMBuildLoad2(builder, LLVMFloatTypeInContext(context), (LLVMValueRef)hash_get(symbols, node->node_data.id_expr->id), "");
    
    if (node->type == FLOAT_EXPR)
        return LLVMConstReal(LLVMFloatTypeInContext(context), node->node_data.float_expr->val);
    
    if (node->type == INT_EXPR)
        return LLVMConstReal(LLVMFloatTypeInContext(context), node->node_data.int_expr->val);
    
    if (node->type == BOOL_EXPR)
        return LLVMConstReal(LLVMFloatTypeInContext(context), node->node_data.bool_expr->val);
    
    // Binary operations
    if (node->type == BINOP_EXPR) {
        LLVMValueRef l = gen_expr(node->node_data.binop_expr->lhs);
        LLVMValueRef r = gen_expr(node->node_data.binop_expr->rhs);
        
        int op = node->node_data.binop_expr->op;
        if (op == PLUS) return LLVMBuildFAdd(builder, l, r, "addtmp");
        if (op == MINUS) return LLVMBuildFSub(builder, l, r, "subtmp");
        if (op == TIMES) return LLVMBuildFMul(builder, l, r, "multmp");
        if (op == DIVIDEDBY) return LLVMBuildFDiv(builder, l, r, "divtmp");
        
        // Comparison operations
        LLVMRealPredicate pred[] = {[EQ]=LLVMRealUEQ, [NEQ]=LLVMRealUNE, [GT]=LLVMRealUGT, [GTE]=LLVMRealUGE, [LT]=LLVMRealULT, [LTE]=LLVMRealULE};
        
        const char* names[] = {[EQ]="eqtmp", [NEQ]="neqtmp", [GT]="gttmp", [GTE]="gtetmp", [LT]="lttmp", [LTE]="ltetmp"};
        
        LLVMValueRef cmp = LLVMBuildFCmp(builder, pred[op], l, r, names[op]);
        return LLVMBuildUIToFP(builder, cmp, LLVMFloatTypeInContext(context), "booltmp");
    }
    return NULL;
}

// Generate LLVM IR for statements
static void gen_stmt(struct ast_node* node) {
    // Variable assignment
    if (node->type == ASSIGN_STMT) {
        char* var = node->node_data.assign_stmt->lhs;
        LLVMValueRef alloca = (LLVMValueRef)hash_get(symbols, var);
        if (!alloca) {
            alloca = LLVMBuildAlloca(builder, LLVMFloatTypeInContext(context), var);
            hash_insert(symbols, var, alloca);
        }
        LLVMBuildStore(builder, gen_expr(node->node_data.assign_stmt->rhs), alloca);
        return;
    }
    
    // Conditional statements
    if (node->type == IF_STMT) {
        LLVMValueRef cond = LLVMBuildFCmp(builder, LLVMRealONE, gen_expr(node->node_data.if_stmt->condition), LLVMConstReal(LLVMFloatTypeInContext(context), 0.0), "ifcond");
        
        // Create basic blocks for control flow
        LLVMBasicBlockRef if_bb = LLVMAppendBasicBlockInContext(context, target_function, "ifBlock");
        LLVMBasicBlockRef else_bb = node->node_data.if_stmt->else_block ? LLVMAppendBasicBlockInContext(context, target_function, "elseBlock") : NULL;
        LLVMBasicBlockRef cont_bb = LLVMAppendBasicBlockInContext(context, target_function, "ifContinueBlock");
        
        // Branch based on condition
        LLVMBuildCondBr(builder, cond, if_bb, else_bb ?: cont_bb);
        
        // Generate if block
        LLVMPositionBuilderAtEnd(builder, if_bb);
        gen_stmt(node->node_data.if_stmt->if_block);
        LLVMBuildBr(builder, cont_bb);
        
        // Generate else block if present
        if (else_bb) {
            LLVMPositionBuilderAtEnd(builder, else_bb);
            gen_stmt(node->node_data.if_stmt->else_block);
            LLVMBuildBr(builder, cont_bb);
        }
        
        // Continue execution after if/else
        LLVMPositionBuilderAtEnd(builder, cont_bb);
        return;
    }
    
    // While loops
    if (node->type == WHILE_STMT) {
        // Create basic blocks for loop structure
        LLVMBasicBlockRef cond_bb = LLVMAppendBasicBlockInContext(context, target_function, "whileCondBlock");
        LLVMBasicBlockRef body_bb = LLVMAppendBasicBlockInContext(context, target_function, "whileBlock");
        LLVMBasicBlockRef cont_bb = LLVMAppendBasicBlockInContext(context, target_function, "whileContinueBlock");
        
        // Save and set break target for nested breaks
        LLVMBasicBlockRef old_break = break_target;
        break_target = cont_bb;
        
        // Jump to condition check
        LLVMBuildBr(builder, cond_bb);
        LLVMPositionBuilderAtEnd(builder, cond_bb);
        
        // Evaluate condition and branch
        LLVMValueRef cond = LLVMBuildFCmp(builder, LLVMRealONE, gen_expr(node->node_data.while_stmt->condition), LLVMConstReal(LLVMFloatTypeInContext(context), 0.0), "whilecond");
        LLVMBuildCondBr(builder, cond, body_bb, cont_bb);
        
        // Generate loop body and jump back to condition
        LLVMPositionBuilderAtEnd(builder, body_bb);
        gen_stmt(node->node_data.while_stmt->block);
        LLVMBuildBr(builder, cond_bb);
        
        // Restore previous break target and continue execution
        break_target = old_break;
        LLVMPositionBuilderAtEnd(builder, cont_bb);
        return;
    }
    
    // Break statements
    if (node->type == BREAK_STMT) {
        LLVMBuildBr(builder, break_target);
        return;
    }
    
    // Statement blocks
    if (node->type == BLOCK) {
        for (int i = 0; i < node->node_data.block->n_stmts; i++)
            gen_stmt(node->node_data.block->stmts[i]);
        return;
    }
}

// Main entry point
char* generate_llvm_ir(struct ast_node* root) {
    // Initialize LLVM context and module
    context = LLVMContextCreate();
    module = LLVMModuleCreateWithNameInContext("Python compiler", context);
    builder = LLVMCreateBuilderInContext(context);
    
    // Create target function with float return type
    LLVMTypeRef float_type = LLVMFloatTypeInContext(context);
    target_function = LLVMAddFunction(module, "target", LLVMFunctionType(float_type, NULL, 0, 0));
    
    // Generate function body from AST
    LLVMPositionBuilderAtEnd(builder, LLVMAppendBasicBlockInContext(context, target_function, "entry"));
    gen_stmt(root);
    
    // Return value handling
    LLVMValueRef ret_var = (LLVMValueRef)hash_get(symbols, "return_value");
    LLVMBuildRet(builder, ret_var ? LLVMBuildLoad2(builder, float_type, ret_var, "") : LLVMConstReal(float_type, 0.0));
    
    // Convert to string and cleanup
    char* ir = LLVMPrintModuleToString(module);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
    return ir;
}

// Generate object file from LLVM IR
void generate_object_code(const char* llvm_ir, const char* output_file) {
    FILE* f = fopen("temp.ll", "w");
    fprintf(f, "%s", llvm_ir);
    fclose(f);
    
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "llc -filetype=obj -o=%s temp.ll", output_file);
    system(cmd);
    remove("temp.ll");
}
