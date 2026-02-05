#include "utils.h"
#include <string.h>

// Bump Allocator
BumpAllocator MakeAllocator(size_t size)
{
    BumpAllocator ba = {};
    ba.memory = (char *)malloc(size);
    if (ba.memory)
    {
        ba.capacity = size;
        memset(ba.memory, 0, size);
    }
    else
    {
        LOG_ASSERT(false, "Failed to allocate Memory!");
    }
    return ba;
}

char *BumpAlloc(BumpAllocator *bumpAllocator, size_t size)
{
    char *result = nullptr;

    size_t allignedSize = (size + 7) & ~7;
    if (bumpAllocator->used + allignedSize <= bumpAllocator->capacity)
    {
        result = bumpAllocator->memory + bumpAllocator->used;
        bumpAllocator->used += allignedSize;
    }
    else
    {
        LOG_ASSERT(false, "BumpAllocator is full");
    }

    return result;
}

// File I/O
long long get_timestemp(char *file)
{
    struct stat file_stat = {};
    stat(file, &file_stat);
    return file_stat.st_mtime;
}

bool file_exists(char *filepath)
{
    LOG_ASSERT(filepath, "No filepath supplied!");

    auto file = fopen(filepath, "rb");
    if (!file)
        return false;
    fclose(file);
    return true;
}

long get_file_size(char *filepath)
{
    LOG_ASSERT(filepath, "No filepath supplied!");

    long fileSize = 0;
    auto file = fopen(filepath, "rb");
    if (!file)
    {
        LOG_ERROR("Failed opening File: %s", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return fileSize;
}

char *read_file(char *filepath, int *size, char *buffer)
{
    LOG_ASSERT(filepath, "No filepath supplied!");
    LOG_ASSERT(size, "No filepath supplied!");

    *size = 0;
    auto file = fopen(filepath, "rb");
    if (!file)
    {
        LOG_ERROR("Failed opening File: %s", filepath);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    memset(buffer, 0, *size + 1);
    fread(buffer, sizeof(char), *size, file);
    fclose(file);
    return buffer;
}

void write_file(char *filepath, char *buffer, int size)
{
    LOG_ASSERT(filepath, "No filepath supplied!");
    LOG_ASSERT(buffer, "No filepath supplied!");

    auto file = fopen(filepath, "wb");
    if (!file)
    {
        LOG_ERROR("Failed opening File: %s", filepath);
        return;
    }

    fwrite(buffer, sizeof(char), size, file);
    fclose(file);
}

char *read_file(char *filepath, int *size, BumpAllocator *bumpAllocator)
{
    char *file = nullptr;
    long fileSize = get_file_size(filepath);

    if (fileSize)
    {
        char *buffer = BumpAlloc(bumpAllocator, fileSize + 1);
        if (buffer) { // Ensure buffer was successfully allocated
            file = read_file(filepath, size, buffer);
        } else {
            LOG_ERROR("read_file (BumpAllocator): Failed to allocate buffer for file %s", filepath);
        }
    }
    return file;
}

bool copy_file(char *filename, char *outputName, char *buffer)
{
    int fileSize = 0;
    char *data = read_file(filename, &fileSize, buffer);

    auto outputFile = fopen(outputName, "wb");
    if (!outputFile)
    {
        LOG_ERROR("Failed opening File: %s", outputName);
        return false;
    }

    int result = fwrite(data, sizeof(char), fileSize, outputFile);
    if (!result)
    {
        LOG_ERROR("Failed opening File: %s", outputName);
        return false;
    }
    fclose(outputFile);
    return true;
}

bool copy_file(char *filename, char *outputName, BumpAllocator *bumpAllocator)
{
    char *file = nullptr;
    long fileSize = get_file_size(filename);

    if (fileSize)
    {
        char *buffer = BumpAlloc(bumpAllocator, fileSize + 1);
        return copy_file(filename, outputName, buffer);
    }
    return false;
}

