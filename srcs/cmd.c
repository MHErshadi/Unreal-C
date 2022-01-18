#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "macro.h"
#include "unreal.h"

#define CMD_MAX_INP 256

int cmd_handler(int argc, char **argv);
void disp_err(int err_code);
void terminate(int sig);

const char *cmderr_str[1] =
{
    "NULL"
};

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Unreal Compiler version %s, verified as Unreal version %s\n", COMPILER_VER, UN_VER);
        printf("Unreal Core verified as %s version %s\n", CORE_HAN, CORE_VER);
        printf("Unreal Term verified as %s version %s\n\n", TERM_HAN, TERM_VER);

        char *command;
        alloc(command, char, CMD_MAX_INP);

        while (1)
        {
            printf(":: ");
            fgets(command, CMD_MAX_INP, stdin);

            //disp_err(cmd_handler(1, *command));

            time_t s = clock();
            for (; isspace((char)*command); command++);
            if (!*command)
                continue;
            command[strlen(command) - 1] = '\0';

            lres_t lres = gen_toks(command, DEF_FN, '\0');
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

            cres_t cres = compile(NOD(pres));
            if (HERR(cres))
            {
                disp_runtime_err(ERR(cres));
                continue;
            }
            time_t e = clock();

            printf("%s\n", cres.incs);
            printf("%s\n", cres.deps);
            printf("%s\n", cres.defs);
            printf("%s\n", cres.funcs_head);
            printf("%s\n", cres.main);
            disp_funcs(cres.funcs);
            printf("\ntime: %ld\n", e - s);

            fre(cres.incs);
            fre(cres.deps);
            fre(cres.defs);
            fre(cres.main);
            fre(cres.funcs_head);
            free_funcs(cres.funcs);
        }

        fre(command);
        return 0;
    }

    disp_err(cmd_handler(argc - 1, argv));
}

int cmd_handler(int argc, char **argv)
{
    
}

void disp_err(int err_code)
{
    if (!err_code)
        return;
}

void terminate(int sig)
{
    puts("\nterminating operation");
    exit(1);
}
