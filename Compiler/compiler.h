#ifndef COMPILER_H
#define COMPILER_H

#include "../GLib/grzasm_structs.h"
#include "../GLib/file_manager.h"

#include "lexem_converter.h"
#include "text_manager.h"
class Compiler
{
public:

    void Compile(const char* in_file_name, const char* out_file_name);

private:
    Lexem* _readed_text;
    size_t _lex_count;

    Instruction* _instructs;
    size_t _instr_count;
};




void Compiler::Compile(const char* in_file_name, const char* out_file_name)
{
    _lex_count = TextManager::ConvertToLexems(FileManager::ReadText(in_file_name), &_readed_text);
    //printf("\n\n\n\n");
    _instr_count = LexemConverter::MakeInstructs(_readed_text, _lex_count, &_instructs);
    //for (int i = 0; i < _instr_count; i++)
     //   printf("[%d: %d %d; %d %d]\n", _instructs[i].cmd, _instructs[i].arg1_flag, _instructs[i].arg1, _instructs[i].arg2_flag, _instructs[i].arg2);
    if (out_file_name == nullptr)
        FileManager::WriteBinary("out.grun", _instructs, sizeof(Instruction), _instr_count);
    else
        FileManager::WriteBinary(out_file_name, _instructs, sizeof(Instruction), _instr_count);
    free(_readed_text);
    free(_instructs);

}
#endif