%{
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

/*
 * Set up a simplified stack to track indentation level as described in the
 * Python docs.  Put 0 on top of the stack.
 *
 * https://docs.python.org/3/reference/lexical_analysis.html#indentation
 */
#define MAX_INDENT_LEVELS 128
static int _indent_stack[MAX_INDENT_LEVELS] = { 0 };
static int _indent_stack_top = 0;
void indent_stack_push(int);
void indent_stack_pop();
int indent_stack_top();
int indent_stack_isempty();

/*
 * Initialize a parser state to be sent to the parser on each push parse call,
 * along with a lexeme value and location value to be passed to the push parse
 * calls.
 */
yypstate* pstate = NULL;
YYSTYPE yylval;
YYLTYPE yylloc;

/*
 * This macro invokes the push parser for a new token.  Make sure to allocate
 * space for lexeme and copy the lexeme string into it when calling
 * PUSH_TOKEN(), if lexeme is not NULL.  The first line of the macro makes
 * sure that pstate is initialized, since we can't call yypstate_new() here.
 */
#define PUSH_TOKEN(category, lexeme) do {                           \
    pstate = pstate ? pstate : yypstate_new();                      \
    if (lexeme != NULL) {                                           \
        int len = strlen(lexeme);                                   \
        yylval.str = malloc((len + 1) * sizeof(char));                  \
        strncpy(yylval.str, lexeme, len + 1);                           \
    }                                                               \
    yylloc.first_line = yylloc.last_line = yylineno;                \
    int status = yypush_parse(pstate, category, &yylval, &yylloc);  \
    if (status != YYPUSH_MORE) {                                    \
        yypstate_delete(pstate);                                    \
        return status;                                              \
    }                                                               \
} while (0)
%}

%option noyywrap
%option yylineno

%%

^[ \t]*\r?\n  /* Skip blank lines */

^[ \t]*#.*\r?\n  /* Skip whole-line comments. */

#.*$  /* Skip comments on the same line as a statement. */

^[ \t]+ {
    /*
     * Handle indentation as described in Python docs linked above. Note that
     * this rule's pattern treats leading spaces and leading tabs equivalently,
     * which could cause some unexpected behavior (compared to normal Python
     * indentation behavior) if they're combined in a single line.  For the
     * purposes of this project, that's OK.
     */
    if (indent_stack_top() < yyleng) {
        /*
         * If the current indentation level is greater than the previous indentation
         * level (stored at the top of the stack), then emit an INDENT and push the
         * new indentation level onto the stack.
         */
        indent_stack_push(yyleng);
        PUSH_TOKEN(INDENT, NULL);
    } else {
        /*
         * If the current indentation level is less than or equal to the previous
         * indentation level, pop indentation levels off the stack until the top is
         * equal to the current indentation level.  Emit a DEDENT for each element
         * popped from the stack.
         */
        while (!indent_stack_isempty() && indent_stack_top() != yyleng) {
            indent_stack_pop();
            PUSH_TOKEN(DEDENT, NULL);
        }

        /*
         * If we popped everything off the stack, that means the current
         * indentation level didn't match any on the stack, which is an indentation
         * error.
         */
        if (indent_stack_isempty()) {
            fprintf(stderr, "Error: Incorrect indentation on line %d\n", yylineno);
            return 1;
        }
    }
}

^[^ \t\r\n]+ {
    /*
     * If we find a line that's not indented (i.e. a line that begins with
     * non-whitespace characters), pop all indentation levels off the stack,
     * and emit a DEDENT for each one.  Then, call REJECT, so the next rule
     * matching this token (i.e. the one at the beginning of the line) is also
     * applied.
     */
    while (indent_stack_top() != 0) {
        indent_stack_pop();
        PUSH_TOKEN(DEDENT, NULL);
    }
    REJECT;
}

\r?\n {
    /*
     * This rule will apply only to endlines that come after a statement.
     * Endlines associated with empty lines and comments are handled above.
     * This rule handles both Unix-style and Windows-style line endings.
     */
    PUSH_TOKEN(NEWLINE, NULL);
}

<<EOF>> {
    /*
     * If we reach the end of the file, pop all indentation levels off the stack
     * and emit a DEDENT for each one.
     */
    while(indent_stack_top() != 0) {
        indent_stack_pop();
        PUSH_TOKEN(DEDENT, NULL);
    }
    int status = yypush_parse(pstate, 0, NULL, NULL);
    yypstate_delete(pstate);
    return status;
}

[ \t]  /* Ignore spaces that haven't been handled above. */

"and"       PUSH_TOKEN(AND, NULL);
"break"     PUSH_TOKEN(BREAK, NULL);
"def"       PUSH_TOKEN(DEF, NULL);
"elif"      PUSH_TOKEN(ELIF, NULL);
"else"      PUSH_TOKEN(ELSE, NULL);
"for"       PUSH_TOKEN(FOR, NULL);
"if"        PUSH_TOKEN(IF, NULL);
"not"       PUSH_TOKEN(NOT, NULL);
"or"        PUSH_TOKEN(OR, NULL);
"return"    PUSH_TOKEN(RETURN, NULL);
"while"     PUSH_TOKEN(WHILE, NULL);

"True"      PUSH_TOKEN(BOOLEAN, yytext);
"False"     PUSH_TOKEN(BOOLEAN, yytext);

[a-zA-Z_][a-zA-Z0-9_]* {
    /*
     * This rule handling identifiers must come after all the keyword rules
     * above, since each keyword would otherwise be treated as a valid
     * identifier.
     */
    PUSH_TOKEN(IDENTIFIER, yytext);
}

[0-9]*"."[0-9]+     PUSH_TOKEN(FLOAT, yytext);
[0-9]+              PUSH_TOKEN(INTEGER, yytext);

"="     PUSH_TOKEN(ASSIGN, NULL);
"+"     PUSH_TOKEN(PLUS, NULL);
"-"     PUSH_TOKEN(MINUS, NULL);
"*"     PUSH_TOKEN(TIMES, NULL);
"/"     PUSH_TOKEN(DIVIDEDBY, NULL);
"=="    PUSH_TOKEN(EQ, NULL);
"!="    PUSH_TOKEN(NEQ, NULL);
">"     PUSH_TOKEN(GT, NULL);
">="    PUSH_TOKEN(GTE, NULL);
"<"     PUSH_TOKEN(LT, NULL);
"<="    PUSH_TOKEN(LTE, NULL);
"("     PUSH_TOKEN(LPAREN, NULL);
")"     PUSH_TOKEN(RPAREN, NULL);
","     PUSH_TOKEN(COMMA, NULL);
":"     PUSH_TOKEN(COLON, NULL);

. {
    /*
     * If we recognize an invalid symbol, send the ASCII code for that symbol
     * into the parser as its category.  By definition, this will not match
     * any of the existing categories, since they are represented by integer
     * values greater than the largest ASCII code.
     */
    fprintf(stderr, "Unrecognized token on line %d: %s\n", yylineno, yytext);
    PUSH_TOKEN(yytext[0], NULL);
}

%%

/*
 * This function pushes another level to the indentation stack.
 */
void indent_stack_push(int l) {
    /*
     * Increment index of top and make sure it's still within the bounds of the
     * stack array.  If it isn't exit with an error.
     */
    _indent_stack_top++;
    if (_indent_stack_top >= MAX_INDENT_LEVELS) {
        fprintf(stderr, "ERROR: too many levels of indentation\n");
        exit(1);
    }
    _indent_stack[_indent_stack_top] = l;
}

/*
 * This function pops the top from the indent stack.
 */
void indent_stack_pop() {
    if (indent_stack_top >= 0) {
        _indent_stack_top--;
    }
}

/*
 * This function returns the top of the indent stack.  Returns -1 if the
 * indent stack is empty.
 */
int indent_stack_top() {
    return _indent_stack_top >= 0 ? _indent_stack[_indent_stack_top] : -1;
}

/*
 * This function returns 1 if the indent stack is empty or 0 otherwise.
 */
int indent_stack_isempty() {
    return _indent_stack_top < 0;
}
