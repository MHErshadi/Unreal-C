#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "macro.h"
#include "unreal.h"

#define CMD_MAX_INP 1024

void terminate(int sig);

int main(int argc, char **argv)
{
    signal(SIGINT, terminate);

    if (argc == 1)
    {
        printf("Unreal version %s\n", UN_VER);
        printf("Unreal Core verified as %s version %s\n", CORE_HAN, CORE_VER);
        printf("Unreal Term verified as %s version %s\n\n", TERM_HAN, TERM_VER);

        char *code;
        alloc(code, char, CMD_MAX_INP + 1);

        struct __ctx ctx = set_ctx_def(DEF_CTX);

        while (1)
        {
            printf(">>> ");
            fgets(code, CMD_MAX_INP, stdin);

            time_t s = clock();

            for (; isspace((char)*code); code++);
            if (!*code)
                continue;
            code[strlen(code) - 1] = '\0';

            lres_t lres = gen_toks(code, DEF_FN, '\0');
            if (HERR(lres))
            {
                disp_ill_chr_err(ERR(lres));
                continue;
            }

            pres_t pres = parse(TOKS(lres));
            if (HERR(pres))
            {
                disp_inv_syn_err(ERR(pres));
                continue;
            }

            vres_t vres = visit(NOD(pres), &ctx, 1);
            if (HERR(vres))
            {
                disp_runtime_err(ERR(vres));
                continue;
            }

            time_t e = clock();
            disp_val(VAL(vres), '\n');
            printf("time: %ld\n", e - s);
            free_val(VAL(vres));
        }

        fre(code);
    }
    return 0;
}

void terminate(int sig)
{
    puts("\nterminating operation");
    exit(1);
}
