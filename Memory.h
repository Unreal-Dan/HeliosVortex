#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>
#include <stdlib.h>

#include "HeliosConfig.h"

// need to provide these for C++ new/delete to work
void *operator new  (size_t size);
void *operator new[](size_t size);
void  operator delete  (void *ptr);
void  operator delete[](void *ptr);
void *operator new  (size_t size, void *ptr) noexcept;
void *operator new[](size_t size, void *ptr) noexcept;
void  operator delete  (void *ptr, size_t size) noexcept;
void  operator delete[](void *ptr, size_t size) noexcept;

#endif
