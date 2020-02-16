#ifndef INSTRUCTION_MANAGER_H
#define INSTRUCTION_MANAGER_H
#include "../GLib/grzasm_structs.h"
#include <stdlib.h>
#include <stdio.h>
const static size_t InstructionCount = 75;
const static char _possible_command[InstructionCount][3] = {
        {STAY, NONE, NONE}, //do nothing

        {PUSH, REG, NONE}, //push register's data to stack
        {PUSH, NUM, NONE}, //push number to stack

        {POP, NONE, NONE}, //pop element from stack
        {POP, REG, NONE},  //pop element from stack to register

        {TOP, REG, NONE},  //get top element of stack to register (don't remove it)

        {MOV, REG, REG}, //reg1 = reg2
        {MOV, REG, NUM}, //reg1 = num

        {ADD, NONE, NONE},//pop 2 elements from stack and push their sum
        {ADD, REG, REG},  //reg1 += reg2
        {ADD, REG, NUM},  //reg1 += num
        {ADD, REG, NONE}, //add reg value to top element of stack
        {ADD, NUM, NONE}, //add num value to top element of stack

        {SUB, NONE, NONE},//as ADD, but subscribe
        {SUB, REG, REG},
        {SUB, REG, NUM},
        {SUB, REG, NONE},
        {SUB, NUM, NONE},

        {MUL, NONE, NONE},//as ADD, but multiply
        {MUL, REG, REG},
        {MUL, REG, NUM},
        {MUL, REG, NONE},
        {MUL, NUM, NONE},

        {DIV, NONE, NONE},//as ADD, but divide
        {DIV, REG, REG},
        {DIV, REG, NUM},
        {DIV, REG, NONE},
        {DIV, NUM, NONE},

        {MOD, NONE, NONE},//as ADD
        {MOD, REG, REG},
        {MOD, REG, NUM},
        {MOD, REG, NONE},
        {MOD, NUM, NONE},

        {SQRT, NONE, NONE}, //pop 1 element from stack and push this sqrt
        {SQRT, REG, NONE},  //reg = sqrt(reg);

        {ABS, NONE, NONE}, //pop 1 element from stack and push this abs
        {ABS, REG, NONE},  //reg = abs(reg)

        {SCAN, NONE, NONE},//scan SYMBOL CODE from console to stack
        {SCAN, REG, NONE}, //to register

        {SCAN_NUM, NONE, NONE}, //scan NUMBER from console to stack
        {SCAN_NUM, REG, NONE},  //to register

        {PRINT, NONE, NONE}, //Pop element from stack and print it to console AS SYMBOL
        {PRINT, NUM, NONE},  //Print number value AS SYMBOL
        {PRINT, REG, NONE},  //Print register value AS SYMBOL

        {PRINT_NUM, NONE, NONE}, //as PRINT, but print NUMBER, not symbol
        {PRINT_NUM, NUM, NONE},
        {PRINT_NUM, REG, NONE},

        {SET_MARK, MARK, NONE}, //Create mark you can jump to

        {JUMP, MARK, NONE}, //Jump to mark

        {JUMP_0, MARK, NONE}, //Pop element from stack. If it is 0, jump to mark
        {JUMP_0, NUM, MARK}, //If number is 0, jump to mark
        {JUMP_0, REG, MARK}, //If reg value is 0, jump to mark

        {JUMP_NEG, MARK, NONE}, //Pop element from stack. If it is < 0, jump to mark
        {JUMP_NEG, NUM, MARK}, //If number is < 0, jump to mark
        {JUMP_NEG, REG, MARK}, //If reg value is < 0, jump to mark

        {SET_FUNC, MARK, NONE}, //Create function mark (synonym to SET_MARK)

        {FUNC, MARK, NONE}, //Jump to mark, but push to function-stack current position in code

        {RETURN, NONE, NONE}, //Pop from function-stack and jump to this command-position

        {BEGIN, NONE, NONE}, //Program start from this command (Required for any program)

        {END, NONE, NONE}, //If program reach this command, it end (Required for any program)

        {GET, ADR, REG}, //Get number of 8 bytes from adress (number) in RAM to register
        {GET, RADR, REG},//Get number of 8 bytes from adress (register value) in RAM to register

        {SET, ADR, NUM}, //Set 8 bytes of RAM in adress (number) to this number
        {SET, ADR, REG}, //Set 8 bytes of RAM in adress (number) to this register value
        {SET, RADR, REG},//Set 8 bytes of RAM in adress (register) to this register value
        {SET, RADR, NUM},//Set 8 bytes of RAM in adress (register) to this number

        {GET_BYTE, ADR, REG}, //As GET, but get only 1 byte
        {GET_BYTE, RADR, REG},

        {SET_BYTE, ADR, NUM}, //As SET, but set only 1 byte
        {SET_BYTE, ADR, REG},
        {SET_BYTE, RADR, REG},
        {SET_BYTE, RADR, NUM},

        {PRINT_RAM, ADR, ADR}, //Print number in RAM from adr1 to adr2 in line (for debug only)

        {PRINT_STACK, NONE, NONE}, //Print all numbers in stack in line (Debug only)
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