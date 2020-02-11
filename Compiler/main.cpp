#include "compiler.h"
#include "../GLib/error_msg.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{   
    Compiler comp;

    if (argc < 2)
        Error::FileErr("Enter input file name!");

    bool debig_mode = false;
    char* out_file = nullptr, *in_file = nullptr;

    in_file = argv[1];

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i >= argc)
                Error::FileErr("Enter output file name after -o");
            out_file = argv[i];
        }
    }
    comp.Compile(in_file, out_file);

    printf("Compiling success!\n");
    return 0;
}
