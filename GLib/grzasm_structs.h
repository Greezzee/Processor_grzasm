#ifndef GRZASM_STRUCTS_H
#define GRZASM_STRUCTS_H

#include <stdint.h>

enum Flag: char
{
    F_ERR,
    NONE,
    DELIM,

    CMD,
    REG,
    NUM,
    MARK,

    ADR,
    RADR
};

enum Command: char
{
    C_ERR,

    STAY,
    PUSH,
    POP,
    TOP,
    MOV,

    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    SQRT,
    ABS,

    SCAN,
    SCAN_NUM,
    PRINT,
    PRINT_NUM,

    SET_MARK,
    JUMP,
    JUMP_0,
    JUMP_NEG,

    SET_FUNC,
    FUNC,
    RETURN,

    BEGIN,
    END,

    GET,
    SET,

    GET_BYTE,
    SET_BYTE,

    PRINT_RAM,
    PRINT_STACK,
};

enum Register: char
{
    R_ERR, rax, rbx, rcx, rdx, eax, ebx, ecx, edx, ax, bx, cx, dx,
    ah, bh, ch, dh, al, bl, cl, dl, esp
};

struct Lexem
{
    Flag flag;
    int64_t val;
};

const Lexem EmptyLexem = { NONE, 0 };

struct Instruction
{
    Flag cmd_flag;
    Command cmd;
    Flag arg1_flag, arg2_flag;
    int64_t arg1, arg2;
};

#endif