#ifndef SYNTAX_MANAGER_H
#define SYNTAX_MANAGER_H

#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include "../GLib/grzasm_structs.h"

class SyntaxManager
{
public:
    static Flag GetFlag(const char* str);
    static int64_t GetVal(const char* str);
private:
    static int64_t GetCMD(const char* str);
    static int64_t GetREG(const char* str);
    static int64_t GetNUM(const char* str);
};

Flag SyntaxManager::GetFlag(const char* str)
{
    if (str == nullptr || strlen(str) == 0)
        return F_ERR;

    if (strcmp("stay", str) == 0)      return CMD;
    if (strcmp("push", str) == 0)      return CMD;
    if (strcmp("pop", str) == 0)       return CMD;
    if (strcmp("top", str) == 0)       return CMD;
    if (strcmp("mov", str) == 0)       return CMD;
    if (strcmp("add", str) == 0)       return CMD;
    if (strcmp("sub", str) == 0)       return CMD;
    if (strcmp("mul", str) == 0)       return CMD;
    if (strcmp("div", str) == 0)       return CMD;
    if (strcmp("mod", str) == 0)       return CMD;
    if (strcmp("sqrt", str) == 0)      return CMD;
    if (strcmp("scan", str) == 0)      return CMD;
    if (strcmp("scan_num", str) == 0)  return CMD;
    if (strcmp("print", str) == 0)     return CMD;
    if (strcmp("print_num", str) == 0) return CMD;
    if (strcmp("set_mark", str) == 0)  return CMD;
    if (strcmp("jump", str) == 0)      return CMD;
    if (strcmp("set_func", str) == 0)  return CMD;
    if (strcmp("func", str) == 0)      return CMD;
    if (strcmp("return", str) == 0)    return CMD;
    if (strcmp("begin", str) == 0)     return CMD;
    if (strcmp("end", str) == 0)       return CMD;
    if (strcmp("get", str) == 0)       return CMD;
    if (strcmp("set", str) == 0)       return CMD;
    if (strcmp("get_byte", str) == 0)  return CMD;
    if (strcmp("set_byte", str) == 0)  return CMD;
    if (strcmp("abs", str) == 0)       return CMD;
    if (strcmp("print_r", str) == 0)   return CMD;
    if (strcmp("print_s", str) == 0)   return CMD;
    if (strcmp("cmp", str) == 0)       return CMD;
    if (strcmp("jb", str) == 0)        return CMD;
    if (strcmp("jl", str) == 0)        return CMD;
    if (strcmp("je", str) == 0)        return CMD;
    if (strcmp("jbe", str) == 0)       return CMD;
    if (strcmp("jle", str) == 0)       return CMD;
    if (strcmp("jne", str) == 0)       return CMD;

    if (strcmp("rax", str) == 0)        return REG;
    if (strcmp("rbx", str) == 0)        return REG;
    if (strcmp("rcx", str) == 0)        return REG;
    if (strcmp("rdx", str) == 0)        return REG;
    if (strcmp("eax", str) == 0)        return REG;
    if (strcmp("ebx", str) == 0)        return REG;
    if (strcmp("ecx", str) == 0)        return REG;
    if (strcmp("edx", str) == 0)        return REG;
    if (strcmp("ax", str) == 0)         return REG;
    if (strcmp("bx", str) == 0)         return REG;
    if (strcmp("cx", str) == 0)         return REG;
    if (strcmp("dx", str) == 0)         return REG;
    if (strcmp("ah", str) == 0)         return REG;
    if (strcmp("bh", str) == 0)         return REG;
    if (strcmp("ch", str) == 0)         return REG;
    if (strcmp("dh", str) == 0)         return REG;
    if (strcmp("al", str) == 0)         return REG;
    if (strcmp("bl", str) == 0)         return REG;
    if (strcmp("cl", str) == 0)         return REG;
    if (strcmp("dl", str) == 0)         return REG;
    if (strcmp("esp", str) == 0)        return REG;

    if (isdigit(str[0]) || (str[0] == '-' 
        && isdigit(str[1])))           return NUM;
    if (strlen(str) >= 3 && str[0] == '\'' && 
        str[strlen(str) - 1] == '\'')  return NUM;

    if (str[0] == ':')                 return MARK;

    if (str[0] == '*' && GetFlag(str + 1) == NUM)  return ADR;
    if (str[0] == '*' && GetFlag(str + 1) == REG)  return RADR;

    return F_ERR;
}

int64_t SyntaxManager::GetVal(const char* str)
{
    switch (GetFlag(str))
    {
    case F_ERR:
        return 0;
        break;
    case CMD:
        return GetCMD(str);
        break;
    case REG:
        return GetREG(str);
        break;
    case NUM:
        return GetNUM(str);
        break;
    case ADR:
        return GetNUM(str + 1);
        break;
    case RADR:
        return GetREG(str + 1);
        break;
    default:
        return 0;
        break;
    }
}

int64_t SyntaxManager::GetCMD(const char* str)
{
    if (strcmp("stay", str) == 0)      return STAY;
    if (strcmp("push", str) == 0)      return PUSH;
    if (strcmp("pop", str) == 0)       return POP;
    if (strcmp("top", str) == 0)       return TOP;
    if (strcmp("mov", str) == 0)       return MOV;
    if (strcmp("add", str) == 0)       return ADD;
    if (strcmp("sub", str) == 0)       return SUB;
    if (strcmp("mul", str) == 0)       return MUL;
    if (strcmp("div", str) == 0)       return DIV;
    if (strcmp("mod", str) == 0)       return MOD;
    if (strcmp("sqrt", str) == 0)      return SQRT;
    if (strcmp("scan", str) == 0)      return SCAN;
    if (strcmp("scan_num", str) == 0)  return SCAN_NUM;
    if (strcmp("print", str) == 0)     return PRINT;
    if (strcmp("print_num", str) == 0) return PRINT_NUM;
    if (strcmp("set_mark", str) == 0)  return SET_MARK;
    if (strcmp("jump", str) == 0)      return JUMP;
    if (strcmp("set_func", str) == 0)  return SET_FUNC;
    if (strcmp("func", str) == 0)      return FUNC;
    if (strcmp("return", str) == 0)    return RETURN;
    if (strcmp("begin", str) == 0)     return BEGIN;
    if (strcmp("end", str) == 0)       return END;
    if (strcmp("get", str) == 0)       return GET;
    if (strcmp("set", str) == 0)       return SET;
    if (strcmp("abs", str) == 0)       return ABS;
    if (strcmp("get_byte", str) == 0)  return GET_BYTE;
    if (strcmp("set_byte", str) == 0)  return SET_BYTE;
    if (strcmp("print_r", str) == 0)   return PRINT_RAM;
    if (strcmp("print_s", str) == 0)   return PRINT_STACK;
    if (strcmp("cmp", str) == 0)       return CMP;
    if (strcmp("jb", str) == 0)        return JB;
    if (strcmp("jl", str) == 0)        return JL;
    if (strcmp("je", str) == 0)        return JE;
    if (strcmp("jbe", str) == 0)       return JBE;
    if (strcmp("jle", str) == 0)       return JLE;
    if (strcmp("jne", str) == 0)       return JNE;

    return C_ERR;
}
int64_t SyntaxManager::GetREG(const char* str)
{
    if (strcmp("rax", str) == 0)        return rax;
    if (strcmp("rbx", str) == 0)        return rbx;
    if (strcmp("rcx", str) == 0)        return rcx;
    if (strcmp("rdx", str) == 0)        return rdx;
    if (strcmp("eax", str) == 0)        return eax;
    if (strcmp("ebx", str) == 0)        return ebx;
    if (strcmp("ecx", str) == 0)        return ecx;
    if (strcmp("edx", str) == 0)        return edx;
    if (strcmp("ax", str) == 0)         return ax;
    if (strcmp("bx", str) == 0)         return bx;
    if (strcmp("cx", str) == 0)         return cx;
    if (strcmp("dx", str) == 0)         return dx;
    if (strcmp("ah", str) == 0)         return ah;
    if (strcmp("bh", str) == 0)         return bh;
    if (strcmp("ch", str) == 0)         return ch;
    if (strcmp("dh", str) == 0)         return dh;
    if (strcmp("al", str) == 0)         return al;
    if (strcmp("bl", str) == 0)         return bl;
    if (strcmp("cl", str) == 0)         return cl;
    if (strcmp("dl", str) == 0)         return dl;
    if (strcmp("esp", str) == 0)        return esp;
    return R_ERR;
}
int64_t SyntaxManager::GetNUM(const char* str)
{
    if (str[0] == '\'') {
        if (str[1] == '\\') 
            switch (str[2]) {
                case 's':
                    return ';';
                case 'n':
                    return '\n';
                case 't':
                    return '\t';
                case '\\':
                    return '\\';
                case '_':
                    return ' ';
            }
        return (int64_t)str[1];
    }
    return atoi(str);
}

#endif