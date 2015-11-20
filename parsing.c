#include <stdio.h>
#include <stdlib.h>
#include "mpc/mpc.h"

/* If compiling on Windows, compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <histedit.h> 
#endif

int main(int argc, char** argv) {

    /* Create some parsers */
    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Operator  = mpc_new("operator");
    mpc_parser_t* Expr      = mpc_new("expr");
    mpc_parser_t* Zlisp     = mpc_new("zlisp");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
        "
        number      :   /-?[0-9]+/ ;                            \
        operator    :   '+' | '-' | '*' | '/' ;                 \
        expr        :   <number> | '(' <operator> <expr>+ ')';  \
        zlisp       :   /^/ <operator> <expr>+ /$/ ;            \
        ",
        Number, Operator, Expr, Zlisp);
    
    puts("zLisp version 0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1) {

        /* Read user input */
        char* input = readline("zlisp> ");

        /* Add input to history */
        add_history(input);

        /* Echo input back to user */
        printf("No you're a %s\n", input);

        /* Free retrieved input */
        free(input);
    }

    /* Undefine and delete our parsers */
    mpc_cleanup(4, Number, Operator, Expr, Zlisp);
    return 0;
}
