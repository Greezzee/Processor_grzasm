#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <stdlib.h>
#include <string.h>

#include "syntax_manager.h"
#include "../GLib/error_msg.h"
#include "../GLib/grzasm_structs.h"

class TextManager
{
public:
    static size_t ConvertToLexems(char* text, Lexem** _readed_text);
    static size_t GetCharCount(const char* text, char c);
    static size_t SplitToStrings(char* text, char*** out);
private:
    static char** _strings;
    static size_t _str_count;

    static Lexem** _lexems_in_string;

    static char** _name_table;
    static size_t _names_count;

    static size_t ReadLexemsInString(char* str, Lexem** out);
    static void DeleteComments(char* str);
    static size_t GetMarkID(char* mark);
};

char** TextManager::_strings;
size_t TextManager::_str_count;

Lexem** TextManager::_lexems_in_string;

char** TextManager::_name_table;
size_t TextManager::_names_count;

size_t TextManager::ConvertToLexems(char* text, Lexem** readed_text)
{
    DeleteComments(text);
    _str_count = SplitToStrings(text, &_strings);
    size_t lexems_count = 0;
    _names_count = 0;
    _name_table = nullptr;
    size_t* lexems_in_str_count = (size_t*)calloc(_str_count, sizeof(size_t));
    _lexems_in_string = (Lexem**)calloc(_str_count, sizeof(Lexem*));
    for (int i = 0; i < _str_count; i++) {
        lexems_in_str_count[i] = ReadLexemsInString(_strings[i], _lexems_in_string + i);
        lexems_count += lexems_in_str_count[i];
    }
    *readed_text = (Lexem*)calloc(lexems_count, sizeof(Lexem));
    int cur_str = 0, cur_lex = 0;
    for (int i = 0; i < lexems_count; i++)
    {
        (*readed_text)[i] = _lexems_in_string[cur_str][cur_lex];
        cur_lex++;
        if (cur_lex >= lexems_in_str_count[cur_str]) {
            cur_lex = 0;
            cur_str++;
        }
    }
    
    //for (int i = 0; i < lexems_count; i++)
    //    printf("[%d %d]\n", (*readed_text)[i].flag, (*readed_text)[i].val);

    free(text);
    free(_strings);
    for (int i = 0; i < _str_count; i++) 
        free(_lexems_in_string[i]);
    free(lexems_in_str_count);
    free(_lexems_in_string);
    if (_names_count != 0)
        free(_name_table);
    return lexems_count;
}

void TextManager::DeleteComments(char* str)
{
    bool is_comment = false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '#') 
            is_comment = true;
        if (str[i] == '\n') {
            is_comment = false;
        }
        if (is_comment)
            str[i] = ' ';
    }   
}

size_t TextManager::GetMarkID(char* mark)
{
    for (int i = 0; i < _names_count; i++)
        if (strcmp(mark, _name_table[i]) == 0)
            return i;
    _names_count++;
    _name_table = (char**)realloc(_name_table, _names_count * sizeof(char*));
    _name_table[_names_count - 1] = mark;
    return _names_count - 1;
}

size_t TextManager::ReadLexemsInString(char* str, Lexem** out)
{
    *out = (Lexem*)calloc(4, sizeof(Lexem));
    char* buf = strtok(str, " \t\n");
    size_t i = 0;
    do {
        (*out)[i].flag = SyntaxManager::GetFlag(buf);
        if ((*out)[i].flag == MARK)
            (*out)[i].val = (int64_t)GetMarkID(buf);
        else
            (*out)[i].val  = SyntaxManager::GetVal(buf);
        if ((*out)[i].flag == F_ERR)
            Error::SyntaxErr(buf);
        i++;

    } while((buf = strtok(nullptr, " \t\n")) != nullptr);

    (*out)[i].flag = Flag::DELIM;
    (*out)[i].val = 0;
    i++;
    return i;
}

size_t TextManager::GetCharCount(const char* text, char c)
{
    size_t count = 0;
    for (int i = 0; text[i] != '\0'; i++)
        if (text[i] == c)
            count++;
    return count;
}

size_t TextManager::SplitToStrings(char* text, char*** out)
{
    *out = (char**)calloc(GetCharCount(text, '\n') + 1, sizeof(char*));

    char* buf = strtok(text, ";");
    size_t i = 0;
    do {
        (*out)[i] = buf;
        i++;
    } while((buf = strtok(nullptr, ";")) != nullptr);
    return i;
}

#endif