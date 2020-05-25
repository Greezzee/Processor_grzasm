#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../GLib/file_manager.h"
#include "../GLib/grzasm_structs.h"
#include "../GLib/error_msg.h"
#include "stack.h"

class Processor
{
public:
    void Run(const char* filename);
private:

    const size_t _memory_size = 4 * 1024 * 1024;

    Instruction* _instr;
    size_t _instr_count;
    size_t cur_instr;

    Stack<int64_t> _program_stack;
    Stack<size_t> _func_stack;

    char* _memory;
    int64_t _reg[5];

    bool _flag_bigger, _flag_less, _flag_equal;

    void Execute();

    void SetReg(Register r, int64_t val);
    int64_t GetReg(Register r);

    void PushCMD(Instruction* instr);
    void PopCMD(Instruction* instr);
    void TopCMD(Instruction* instr);
    void MovCMD(Instruction* instr);
    void AddCMD(Instruction* instr);
    void SubCMD(Instruction* instr);
    void MulCMD(Instruction* instr);
    void DivCMD(Instruction* instr);
    void ModCMD(Instruction* instr);
    void SqrtCMD(Instruction* instr);
    void ScanCMD(Instruction* instr);
    void ScanNumCMD(Instruction* instr);
    void PrintCMD(Instruction* instr);
    void PrintNumCMD(Instruction* instr);
    void JumpCMD(Instruction* instr);
    void Jump0CMD(Instruction* instr);
    void JumpNegCMD(Instruction* instr);
    void FuncCMD(Instruction* instr);
    void ReturnCMD(Instruction* instr);
    void GetCMD(Instruction* instr);
    void SetCMD(Instruction* instr);
    void GetByteCMD(Instruction* instr);
    void SetByteCMD(Instruction* instr);
    void AbsCMD(Instruction* instr);
    void PrintRamCMD(Instruction* instr);
    void PrintStackCMD(Instruction* instr);
    void CMP_CMD(Instruction* instr);
};

void Processor::Run(const char* filename)
{
    _instr_count = FileManager::FileSize(filename) / sizeof(Instruction);
    _instr = (Instruction*)FileManager::ReadTextBinary(filename);
    //for (int i = 0; i < _instr_count; i++)
     //   printf("[%d: %d %d; %d %d]\n", _instr[i].cmd, _instr[i].arg1_flag, _instr[i].arg1, _instr[i].arg2_flag, _instr[i].arg2);
    _memory = (char*)calloc(_memory_size, sizeof(char));
    Execute();

    free(_instr);
    free(_memory);
}

void Processor::Execute()
{
    for (int i = 0; i < 5; i++)
        _reg[i] = 0;
    cur_instr = 0;
    _flag_equal = false;
    _flag_less = false;
    _flag_bigger = false;
    while(_instr[cur_instr].cmd != BEGIN) cur_instr++;
    while(_instr[cur_instr].cmd != END) {
        if (cur_instr >= _instr_count)
            Error::RuntimeErr("END not reachable");
        Instruction* instr = &_instr[cur_instr];

        unsigned char cmd = (unsigned char)_instr[cur_instr].cmd;
        bool exec = false;

        if ((cmd & 128) == 0 && (cmd & 64) == 0 && (cmd & 32) == 0)
            exec = true;

        if ((cmd & 128) != 0) {
            cmd -= 128;
            if (_flag_bigger)
                exec = true;
        }

        if ((cmd & 64) != 0) {
            cmd -= 64;
            if (_flag_less)
                exec = true;
        }

        if ((cmd & 32) != 0) {
            cmd -= 32;
            if (_flag_equal)
                exec = true;
        }
        if (exec)
        switch ((Command)cmd)
        {
        case STAY:
            break;
        case PUSH:
            PushCMD(instr);
            break;
        case POP:
            PopCMD(instr);
            break;
        case TOP:
            TopCMD(instr);
            break;
        case MOV:
            MovCMD(instr);
            break;
        case ADD:
            AddCMD(instr);
            break;
        case SUB:
            SubCMD(instr);
            break;
        case MUL:
            MulCMD(instr);
            break;
        case DIV:
            DivCMD(instr);
            break;
        case MOD:
            ModCMD(instr);
            break;
        case SQRT:
            SqrtCMD(instr);
            break;
        case SCAN:
            ScanCMD(instr);
            break;
        case SCAN_NUM:
            ScanNumCMD(instr);
            break;
        case PRINT:
            PrintCMD(instr);
            break;
        case PRINT_NUM:
            PrintNumCMD(instr);
            break;
        case SET_MARK:
            break;
        case JUMP:
            JumpCMD(instr);
            break;
        case SET_FUNC:
            break;
        case FUNC:
            FuncCMD(instr);
            break;
        case RETURN:
            ReturnCMD(instr);
            break;
        case BEGIN:
            break;
        case END:
            return;
            break;
        case GET:
            GetCMD(instr);
            break;
        case SET:
            SetCMD(instr);
            break;
        case GET_BYTE:
            GetByteCMD(instr);
            break;
        case SET_BYTE:
            SetByteCMD(instr);
            break;
        case ABS:
            AbsCMD(instr);
            break;
        case PRINT_RAM:
            PrintRamCMD(instr);
            break;
        case PRINT_STACK:
            PrintStackCMD(instr);
            break;
        case CMP:
            CMP_CMD(instr);
            break;
        default:
            Error::RuntimeErr("Unknown command");
            break;
        }
        cur_instr++;
    }

}

void Processor::CMP_CMD(Instruction* instr)
{
    int64_t num1 = GetReg((Register)instr->arg1);
    int64_t num2 = 0;
    if (instr->arg2_flag == REG)
        num2 = GetReg((Register)instr->arg2);
    if (instr->arg2_flag == NUM)
        num2 = instr->arg2;

    if (num1 == num2) {
        _flag_equal = true;
        _flag_bigger = false;
        _flag_less = false;
    }
    if (num1 > num2) {
        _flag_equal = false;
        _flag_bigger = true;
        _flag_less = false;
    }
    if (num1 < num2) {
        _flag_equal = false;
        _flag_bigger = false;
        _flag_less = true;
    }
}

void Processor::SetReg(Register r, int64_t val)
{
    switch (r)
    {
    case rax:
        _reg[1] = val;
        break;
    case rbx:
        _reg[2] = val;
        break;
    case rcx:
        _reg[3] = val;
        break;
    case rdx:
        _reg[4] = val;
        break;
    case eax:
        ((int32_t*)_reg)[2] = (int32_t)val;
        break; 
    case ebx:
        ((int32_t*)_reg)[4] = (int32_t)val;
        break; 
    case ecx:
        ((int32_t*)_reg)[6] = (int32_t)val;
        break; 
    case edx:
        ((int32_t*)_reg)[8] = (int32_t)val;
        break; 
    case ax:
        ((int16_t*)_reg)[4] = (int16_t)val;
        break; 
    case bx:
        ((int16_t*)_reg)[8] = (int16_t)val;
        break; 
    case cx:
        ((int16_t*)_reg)[12] = (int16_t)val;
        break; 
    case dx:
        ((int16_t*)_reg)[16] = (int16_t)val;
        break; 
    case al:
        ((char*)_reg)[8] = (char)val;
        break; 
    case bl:
        ((char*)_reg)[16] = (char)val;
        break; 
    case cl:
        ((char*)_reg)[24] = (char)val;
        break; 
    case dl:
        ((char*)_reg)[32] = (char)val;
        break;
    case ah:
        ((char*)_reg)[9] = (char)val;
        break; 
    case bh:
        ((char*)_reg)[17] = (char)val;
        break; 
    case ch:
        ((char*)_reg)[25] = (char)val;
        break; 
    case dh:
        ((char*)_reg)[33] = (char)val;
        break;
    case esp:
        cur_instr =(size_t)val;
    default:
        break;
    }
    //for (int i = sizeof(int64_t); i < 5 * sizeof(int64_t); i++) {
    //   if (i % sizeof(int64_t) == 0)
    //        printf("\n");
    //    printf("%d ", *((char*)_reg + i));
    //}
    //printf("\n");
}

int64_t Processor::GetReg(Register r)
{
    switch (r)
    {
    case rax:
        return _reg[1];
        break;
    case rbx:
        return _reg[2];
        break;
    case rcx:
        return _reg[3];
        break;
    case rdx:
        return _reg[4];
        break;
    case eax:
        return ((int32_t*)_reg)[2];
        break; 
    case ebx:
        return ((int32_t*)_reg)[4];
        break; 
    case ecx:
        return ((int32_t*)_reg)[6];
        break; 
    case edx:
        return ((int32_t*)_reg)[8];
        break; 
    case ax:
        return ((int16_t*)_reg)[4];
        break; 
    case bx:
        return ((int16_t*)_reg)[8];
        break; 
    case cx:
        return ((int16_t*)_reg)[12];
        break; 
    case dx:
        return ((int16_t*)_reg)[16];
        break; 
    case al:
        return ((char*)_reg)[8];
        break; 
    case bl:
        return ((char*)_reg)[16];
        break; 
    case cl:
        return ((char*)_reg)[24];
        break; 
    case dl:
        return ((char*)_reg)[32];
        break;
    case ah:
        return ((char*)_reg)[9];
        break; 
    case bh:
        return ((char*)_reg)[17];
        break; 
    case ch:
        return ((char*)_reg)[25];
        break; 
    case dh:
        return ((char*)_reg)[33];
        break;
    case esp:
        return cur_instr;
    default:
        return 0;
        break;
    }
}

void Processor::PushCMD(Instruction* instr)
{
    if (instr->arg1_flag == REG)
        _program_stack.Push(GetReg((Register)instr->arg1));
    if (instr->arg1_flag == NUM)
        _program_stack.Push(instr->arg1);
}
void Processor::PopCMD(Instruction* instr)
{
    if (instr->arg1_flag == REG)
        SetReg((Register)instr->arg1, _program_stack.Pop());
    if (instr->arg1_flag == NONE)
        _program_stack.Pop();
}
void Processor::TopCMD(Instruction* instr)
{
    SetReg((Register)instr->arg1, _program_stack.Top());
}
void Processor::MovCMD(Instruction* instr)
{
    if (instr->arg2_flag == REG)
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg2));
    if (instr->arg2_flag == NUM)
        SetReg((Register)instr->arg1, instr->arg2);
}
void Processor::AddCMD(Instruction* instr)
{
    int64_t a = 0, b = 0;
    if (instr->arg1_flag == NONE && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = _program_stack.Pop();
        _program_stack.Push(a + b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = GetReg((Register)instr->arg1);
        _program_stack.Push(a + b);
    }
    else if (instr->arg1_flag == NUM && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = instr->arg1;
        _program_stack.Push(a + b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == REG) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) + GetReg((Register)instr->arg2));
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NUM) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) + instr->arg2);
    }
}
void Processor::SubCMD(Instruction* instr)
{
    int64_t a = 0, b = 0;
    if (instr->arg1_flag == NONE && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = _program_stack.Pop();
        _program_stack.Push(b - a);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = GetReg((Register)instr->arg1);
        _program_stack.Push(a - b);
    }
    else if (instr->arg1_flag == NUM && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = instr->arg1;
        _program_stack.Push(a - b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == REG) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) - GetReg((Register)instr->arg2));
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NUM) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) - instr->arg2);
    }
}
void Processor::MulCMD(Instruction* instr)
{
    int64_t a = 0, b = 0;
    if (instr->arg1_flag == NONE && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = _program_stack.Pop();
        _program_stack.Push(a * b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = GetReg((Register)instr->arg1);
        _program_stack.Push(a * b);
    }
    else if (instr->arg1_flag == NUM && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = instr->arg1;
        _program_stack.Push(a * b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == REG) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) * GetReg((Register)instr->arg2));
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NUM) {
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) * instr->arg2);
    }
}
void Processor::DivCMD(Instruction* instr)
{
    int64_t a = 0, b = 0;
    if (instr->arg1_flag == NONE && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = _program_stack.Pop();
        if (a == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(b / a);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = GetReg((Register)instr->arg1);
        if (b == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(a / b);
    }
    else if (instr->arg1_flag == NUM && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = instr->arg1;
        if (b == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(a / b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == REG) {
        if (GetReg((Register)instr->arg2) == 0) Error::RuntimeErr("Division by 0");
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) / GetReg((Register)instr->arg2));
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NUM) {
        if (instr->arg2 == 0) Error::RuntimeErr("Division by 0");
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) / instr->arg2);
    }
}
void Processor::ModCMD(Instruction* instr)
{
    int64_t a = 0, b = 0;
    if (instr->arg1_flag == NONE && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = _program_stack.Pop();
        if (a == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(b % a);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = GetReg((Register)instr->arg1);
        if (b == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(a % b);
    }
    else if (instr->arg1_flag == NUM && instr->arg2_flag == NONE) {
        a = _program_stack.Pop();
        b = instr->arg1;
        if (b == 0) Error::RuntimeErr("Division by 0");
        _program_stack.Push(a % b);
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == REG) {
        if (GetReg((Register)instr->arg2) == 0) Error::RuntimeErr("Division by 0");
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) % GetReg((Register)instr->arg2));
    }
    else if (instr->arg1_flag == REG && instr->arg2_flag == NUM) {
        if (instr->arg2 == 0) Error::RuntimeErr("Division by 0");
        SetReg((Register)instr->arg1, GetReg((Register)instr->arg1) % instr->arg2);
    }
}
void Processor::SqrtCMD(Instruction* instr)
{
    if (instr->arg1_flag == NONE) {
        int a = _program_stack.Pop();
        if (a < 0) Error::RuntimeErr("Sqrt of negative num");
        _program_stack.Push(sqrt(a));
    }
    if (instr->arg1_flag == REG) {
        int a = GetReg((Register)instr->arg1);
        if (a < 0) Error::RuntimeErr("Sqrt of negative num");
        SetReg((Register)instr->arg1, sqrt(a));
    }
}

void Processor::AbsCMD(Instruction* instr)
{
    if (instr->arg1_flag == NONE) {
        int a = _program_stack.Pop();
        _program_stack.Push(abs(a));
    }
    if (instr->arg1_flag == REG) {
        int a = GetReg((Register)instr->arg1);
        SetReg((Register)instr->arg1, abs(a));
    }
}

void Processor::ScanCMD(Instruction* instr)
{
    int64_t input = 0;
    int out = scanf("%c", &input);
    if (out != 1) Error::RuntimeErr("Incorrect input");
    if (instr->arg1_flag == NONE)
        _program_stack.Push(input);
    else if (instr->arg1_flag == REG)
        SetReg((Register)instr->arg1, input);
}

void Processor::ScanNumCMD(Instruction* instr)
{
    int64_t input = 0;
    int out = scanf("%Ld", &input);
    if (out != 1) Error::RuntimeErr("Incorrect input");
    if (instr->arg1_flag == NONE)
        _program_stack.Push(input);
    else if (instr->arg1_flag == REG)
        SetReg((Register)instr->arg1, input);
}
void Processor::PrintCMD(Instruction* instr)
{
    char toPrint = 0;
    if (instr->arg1_flag == NONE)
        toPrint = _program_stack.Pop();
    else if (instr->arg1_flag == NUM)
        toPrint = instr->arg1;
    else if (instr->arg1_flag == REG)
        toPrint = GetReg((Register)instr->arg1);
    printf("%c", toPrint);
}
void Processor::PrintNumCMD(Instruction* instr)
{
    int64_t toPrint = 0;
    if (instr->arg1_flag == NONE)
        toPrint = _program_stack.Pop();
    else if (instr->arg1_flag == NUM)
        toPrint = instr->arg1;
    else if (instr->arg1_flag == REG)
        toPrint = GetReg((Register)instr->arg1);
    printf("%Ld", toPrint);
}

void Processor::JumpCMD(Instruction* instr)
{
    cur_instr = instr->arg1;
}
void Processor::FuncCMD(Instruction* instr)
{
    _func_stack.Push(cur_instr);
    JumpCMD(instr);
}
void Processor::ReturnCMD(Instruction* instr)
{
    cur_instr = _func_stack.Pop();
}
void Processor::GetCMD(Instruction* instr)
{
    size_t adr = 0;
    if (instr->arg1_flag == ADR)
        adr = instr->arg1;
    else if (instr->arg1_flag == RADR)
        adr = GetReg((Register)instr->arg1);
    if (adr + sizeof(int64_t) >= _memory_size)
        Error::RuntimeErr("Out of memory");
    
    SetReg((Register)instr->arg2, *(int64_t*)(_memory + adr));
}
void Processor::SetCMD(Instruction* instr)
{
    size_t adr = 0;
    if (instr->arg1_flag == ADR)
        adr = instr->arg1;
    else if (instr->arg1_flag == RADR)
        adr = GetReg((Register)instr->arg1);
    if (adr + sizeof(int64_t) >= _memory_size)
        Error::RuntimeErr("Out of memory");
    int64_t num = 0;
    if (instr->arg2_flag == NUM)
        num = instr->arg2;
    else if (instr->arg2_flag == REG)
        num = GetReg((Register)instr->arg2);
    *(int64_t*)(_memory + adr) = num;
    //for (int i = 0; i < 20; i++)
     //   printf("%d ", _memory[i]);
    //printf("\n");
}

void Processor::GetByteCMD(Instruction* instr)
{
    size_t adr = 0;
    if (instr->arg1_flag == ADR)
        adr = instr->arg1;
    else if (instr->arg1_flag == RADR)
        adr = GetReg((Register)instr->arg1);
    if (adr + sizeof(int64_t) >= _memory_size)
        Error::RuntimeErr("Out of memory");
    
    SetReg((Register)instr->arg2, *(char*)(_memory + adr));
}
void Processor::SetByteCMD(Instruction* instr)
{
    size_t adr = 0;
    if (instr->arg1_flag == ADR)
        adr = instr->arg1;
    else if (instr->arg1_flag == RADR)
        adr = GetReg((Register)instr->arg1);
    if (adr + sizeof(int64_t) >= _memory_size)
        Error::RuntimeErr("Out of memory");
    int64_t num = 0;
    if (instr->arg2_flag == NUM)
        num = instr->arg2 % 256;
    else if (instr->arg2_flag == REG)
        num = GetReg((Register)instr->arg2) % 256;
    *(char*)(_memory + adr) = num;
    //for (int i = 0; i < 20; i++)
    //    printf("%d ", _memory[i]);
    //printf("\n");
}

void Processor::PrintRamCMD(Instruction* instr)
{
    for (int i = instr->arg1; i < instr->arg2 && i < _memory_size; i++)
        printf("%d ", _memory[i]);
}

void Processor::PrintStackCMD(Instruction* instr)
{
    _program_stack.PrintStack();
}

#endif