#include "utils.h"
#include <string.h>
#include "interface.h"

// ==========================
// Bump Allocator
// ==========================
BumpAllocator MakeAllocator(size_t size)
{
    BumpAllocator alloc = {};
    alloc.memory = (char*)malloc(size);
    LOG_ASSERT(alloc.memory != nullptr, "Failed to allocate BumpAllocator memory");
    alloc.capacity = size;
    alloc.used = 0;
    return alloc;
}

char* BumpAlloc(BumpAllocator* bumpAllocator, size_t size)
{
    LOG_ASSERT(bumpAllocator, "BumpAlloc: allocator is null");

    if (bumpAllocator->used + size > bumpAllocator->capacity)
        return nullptr;

    char* ptr = bumpAllocator->memory + bumpAllocator->used;
    bumpAllocator->used += size;
    return ptr;
}

// ==========================
// File I/O
// ==========================
long long get_timestemp(const char* file)
{
    LOG_ASSERT(file, "get_timestemp: file is null");

    struct stat file_stat = {};
    if (stat(file, &file_stat) != 0)
        return 0;

    return file_stat.st_mtime;
}

bool file_exists(const char* filepath)
{
    LOG_ASSERT(filepath, "file_exists: filepath is null");

    FILE* file = fopen(filepath, "rb");
    if (!file)
        return false;

    fclose(file);
    return true;
}

long get_file_size(const char* filepath)
{
    LOG_ASSERT(filepath, "get_file_size: filepath is null");

    FILE* file = fopen(filepath, "rb");
    if (!file)
        return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return size;
}

char* read_file(const char* filepath, int* size, char* buffer)
{
    LOG_ASSERT(filepath, "read_file: filepath is null");
    LOG_ASSERT(size, "read_file: size is null");
    LOG_ASSERT(buffer, "read_file: buffer is null");

    *size = 0;

    FILE* file = fopen(filepath, "rb");
    if (!file)
        return nullptr;

    fseek(file, 0, SEEK_END);
    *size = (int)ftell(file);
    fseek(file, 0, SEEK_SET);

    memset(buffer, 0, *size + 1);
    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

char* read_file(const char* filepath, int* size, BumpAllocator* bumpAllocator)
{
    LOG_ASSERT(bumpAllocator, "read_file: allocator is null");

    long fileSize = get_file_size(filepath);
    if (fileSize <= 0)
        return nullptr;

    char* buffer = BumpAlloc(bumpAllocator, fileSize + 1);
    if (!buffer)
        return nullptr;

    return read_file(filepath, size, buffer);
}

void write_file(const char* filepath, const char* buffer, int size)
{
    LOG_ASSERT(filepath, "write_file: filepath is null");
    LOG_ASSERT(buffer, "write_file: buffer is null");

    FILE* file = fopen(filepath, "wb");
    if (!file)
    {
        LOG_ERROR("Failed opening file: %s", filepath);
        return;
    }

    fwrite(buffer, 1, size, file);
    fclose(file);
}

bool copy_file(const char* filename, const char* outputName, char* buffer)
{
    LOG_ASSERT(filename, "copy_file: filename is null");
    LOG_ASSERT(outputName, "copy_file: outputName is null");
    LOG_ASSERT(buffer, "copy_file: buffer is null");

    int fileSize = 0;
    char* data = read_file(filename, &fileSize, buffer);
    if (!data || fileSize <= 0)
        return false;

    FILE* outputFile = fopen(outputName, "wb");
    if (!outputFile)
        return false;

    fwrite(data, 1, fileSize, outputFile);
    fclose(outputFile);
    return true;
}

bool copy_file(const char* filename, const char* outputName, BumpAllocator* bumpAllocator)
{
    LOG_ASSERT(bumpAllocator, "copy_file: allocator is null");

    long fileSize = get_file_size(filename);
    if (fileSize <= 0)
        return false;

    char* buffer = BumpAlloc(bumpAllocator, fileSize + 1);
    if (!buffer)
        return false;

    return copy_file(filename, outputName, buffer);
}
