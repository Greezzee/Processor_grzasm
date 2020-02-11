#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error_msg.h"
#include "grzasm_structs.h"

class FileManager
{
public:
    static char* ReadText(const char* in_file_name);
    static char* ReadTextBinary(const char* in_file_name);
    static size_t FileSize(FILE* file);
    static size_t FileSize(const char* filename);
    static void WriteBinary(const char* file_name, void* data, size_t size, size_t count);
private:
    static FILE* _input;

    static char* _text;
    static size_t _file_size;
};

FILE* FileManager::_input;

char* FileManager::_text;
size_t FileManager::_file_size;

char* FileManager::ReadText(const char in_file_name[])
{
    _input = fopen(in_file_name, "r");
    if (_input == nullptr)
        Error::FileErr("File not found");
    _file_size = FileSize(_input);
    _text = (char*)calloc(_file_size + 1, sizeof(char));
    fread(_text, 1, _file_size, _input);
    fclose(_input);
    return _text;
}

char* FileManager::ReadTextBinary(const char in_file_name[])
{
    _input = fopen(in_file_name, "rb");
    if (_input == nullptr)
        Error::FileErr("File not found");
    _file_size = FileSize(_input);
    _text = (char*)calloc(_file_size, sizeof(char));
    fread(_text, 1, _file_size, _input);
    fclose(_input);
    return _text;
}

void FileManager::WriteBinary(const char* file_name, void* data, size_t size, size_t count)
{
    FILE* out_file = fopen(file_name, "wb");
    fwrite(data, size, count, out_file);
    fclose(out_file);
}

size_t FileManager::FileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    size_t out = ftell(file);
    fseek(file, 0, 0);
    return out;
}

size_t FileManager::FileSize(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == nullptr)
        Error::FileErr("File not found");
    fseek(file, 0, SEEK_END);
    size_t out = ftell(file);
    fseek(file, 0, 0);
    fclose(file);
    return out;
}

#endif