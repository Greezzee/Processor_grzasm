#ifndef LEXEM_CONVERTER_H
#define LEXEM_CONVERTER_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../GLib/grzasm_structs.h"
#include "../GLib/error_msg.h"
#include "instruction_manager.h"
class LexemConverter
{
public:
    static size_t MakeInstructs(Lexem* lexems, size_t lexems_count, Instruction** output_instructs);

private:

    static size_t GetInstrCount(Lexem* lexems, size_t lexems_count);
    static Instruction GetInstr(Lexem lexems[3]);
    static void SolveMarks(Instruction* instr, size_t instr_count);
};

size_t LexemConverter::MakeInstructs(Lexem* lexems, size_t lexems_count,  Instruction** output_instructs)
{
    size_t instr_count = GetInstrCount(lexems, lexems_count);

    *output_instructs = (Instruction*)calloc(instr_count, sizeof(Instruction));

    size_t cur_instr_lex = 0;
    size_t cur_instr = 0;
    Lexem instr_lexems[3];
    for (int i = 0; i < 3; i++)
        instr_lexems[i] = EmptyLexem;
    for (int i = 0; i < lexems_count; i++) {
        if (lexems[i].flag == DELIM) {
            (*output_instructs)[cur_instr] = GetInstr(instr_lexems);
            cur_instr_lex = 0;
            for (int i = 0; i < 3; i++)
                instr_lexems[i] = EmptyLexem;
            cur_instr++;
        }
        else {

            if (cur_instr_lex >= 3)
                Error::SyntaxErr("Too many args:\n Instruction can't has more than 2 args");

            instr_lexems[cur_instr_lex] = lexems[i];
            cur_instr_lex++;

        }
    }

    SolveMarks(*output_instructs, instr_count);

    return instr_count;
}

Instruction LexemConverter::GetInstr(Lexem lexems[3])
{
    Instruction out;
    Flag lex_flags[3];
    for (int i = 0; i < 3; i++)
        lex_flags[i] = lexems[i].flag;
    
    if (InstructionManager::IsPossibleInstruction(lex_flags, (Command)lexems[0].val)) {
        out.cmd       = (Command)lexems[0].val;
        out.cmd_flag  = CMD;
        out.arg1_flag = lexems[1].flag;
        out.arg2_flag = lexems[2].flag;
        out.arg1      = lexems[1].val;
        out.arg2      = lexems[2].val;
    }
    else
        Error::SyntaxErr("Uncorrect instruction");

    return out;
    
}

size_t LexemConverter::GetInstrCount(Lexem* lexems, size_t lexems_count)
{
    size_t out = 0;
    for (size_t i = 0; i < lexems_count; i++)
        if (lexems[i].flag == DELIM)
            out++;
    return out;
}

void LexemConverter::SolveMarks(Instruction* instr, size_t instr_count)
{
    size_t marks_count = 0;

    for (int i = 0; i < instr_count; i++) {
        if (instr[i].arg1_flag == MARK && instr[i].arg1 + 1 > marks_count) {
            marks_count = instr[i].arg1 + 1;
            //printf("[%d: %d %d; %d %d]\n", instr[i].cmd, instr[i].arg1_flag, instr[i].arg1, instr[i].arg2_flag, instr[i].arg2);
        }
        if (instr[i].arg2_flag == MARK && instr[i].arg2 + 1 > marks_count){
            marks_count = instr[i].arg2 + 1;
        }
    }
    //printf("Marks count: %d\n", marks_count);
    size_t* marks_map = (size_t*)calloc(marks_count, sizeof(size_t));

    for (int i = 0; i < instr_count; i++) {
        if (instr[i].cmd == SET_MARK || instr[i].cmd == SET_FUNC) {
            if (marks_map[instr[i].arg1] != 0)
                Error::SyntaxErr("Multiply init mark");
            marks_map[instr[i].arg1] = i + 1;
        }
    }
    
    for (int i = 0; i < marks_count; i++)
        if (marks_map[i] == 0)
            Error::SyntaxErr("Undefined mark");

    for (int i = 0; i < instr_count; i++) {
        if (instr[i].arg1_flag == MARK)
            instr[i].arg1 = marks_map[instr[i].arg1] - 1;
        if (instr[i].arg2_flag == MARK)
            instr[i].arg2 = marks_map[instr[i].arg2] - 1;
    }

    free(marks_map);
}

#endif