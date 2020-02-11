#ifndef INSTRUCTION_MANAGER_H
#define INSTRUCTION_MANAGER_H
#include "../GLib/grzasm_structs.h"
#include <stdlib.h>
#include <stdio.h>
const static size_t InstructionCount = 75;
const static char _possible_command[InstructionCount][3] = {
        {STAY, NONE, NONE},

        {PUSH, REG, NONE},
        {PUSH, NUM, NONE},

        {POP, NONE, NONE},
        {POP, REG, NONE},

        {TOP, REG, NONE},

        {MOV, REG, REG},
        {MOV, REG, NUM},

        {ADD, NONE, NONE},
        {ADD, REG, REG},
        {ADD, REG, NUM},
        {ADD, REG, NONE},
        {ADD, NUM, NONE},

        {SUB, NONE, NONE},
        {SUB, REG, REG},
        {SUB, REG, NUM},
        {SUB, REG, NONE},
        {SUB, NUM, NONE},

        {MUL, NONE, NONE},
        {MUL, REG, REG},
        {MUL, REG, NUM},
        {MUL, REG, NONE},
        {MUL, NUM, NONE},

        {DIV, NONE, NONE},
        {DIV, REG, REG},
        {DIV, REG, NUM},
        {DIV, REG, NONE},
        {DIV, NUM, NONE},

        {MOD, NONE, NONE},
        {MOD, REG, REG},
        {MOD, REG, NUM},
        {MOD, REG, NONE},
        {MOD, NUM, NONE},

        {SQRT, NONE, NONE},
        {SQRT, REG, NONE},

        {ABS, NONE, NONE},
        {ABS, REG, NONE},

        {SCAN, NONE, NONE},
        {SCAN, REG, NONE},

        {SCAN_NUM, NONE, NONE},
        {SCAN_NUM, REG, NONE},

        {PRINT, NONE, NONE},
        {PRINT, NUM, NONE},
        {PRINT, REG, NONE},

        {PRINT_NUM, NONE, NONE},
        {PRINT_NUM, NUM, NONE},
        {PRINT_NUM, REG, NONE},

        {SET_MARK, MARK, NONE},

        {JUMP, MARK, NONE},

        {JUMP_0, MARK, NONE},
        {JUMP_0, NUM, MARK},
        {JUMP_0, REG, MARK},

        {JUMP_NEG, MARK, NONE},
        {JUMP_NEG, NUM, MARK},
        {JUMP_NEG, REG, MARK},

        {SET_FUNC, MARK, NONE},

        {FUNC, MARK, NONE},

        {RETURN, NONE, NONE},

        {BEGIN, NONE, NONE},

        {END, NONE, NONE},

        {GET, ADR, REG},
        {GET, RADR, REG},

        {SET, ADR, NUM},
        {SET, ADR, REG},
        {SET, RADR, REG},
        {SET, RADR, NUM},

        {GET_BYTE, ADR, REG},
        {GET_BYTE, RADR, REG},

        {SET_BYTE, ADR, NUM},
        {SET_BYTE, ADR, REG},
        {SET_BYTE, RADR, REG},
        {SET_BYTE, RADR, NUM},

        {PRINT_RAM, ADR, ADR},

        {PRINT_STACK, NONE, NONE},
    };

class InstructionManager
{
public:
    static bool IsPossibleInstruction(Flag lex_flahs[3], Command cmd);
private:
};

bool InstructionManager::IsPossibleInstruction(Flag lex_flag[3], Command cmd)
{
    if (lex_flag[0] == F_ERR)
        return false;
    
    for (int i = 0; i < InstructionCount; i++) {
        if (cmd == _possible_command[i][0] &&
            lex_flag[1] == _possible_command[i][1] &&
            lex_flag[2] == _possible_command[i][2])
                return true;
    }
    return false;
}

#endif