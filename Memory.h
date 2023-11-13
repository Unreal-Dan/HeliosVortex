#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>
#include <stdlib.h>

#include "HeliosConfig.h"

void *operator new  (size_t size);
void *operator new[](size_t size);
void  operator delete  (void *ptr);
void  operator delete[](void *ptr);
void *operator new  (size_t size, void *ptr) noexcept;
void *operator new[](size_t size, void *ptr) noexcept;
void  operator delete  (void *ptr, size_t size) noexcept;
void  operator delete[](void *ptr, size_t size) noexcept;
//void *operator new  (size_t size, std::align_val_t al);
//void *operator new[](size_t size, std::align_val_t al);
//void  operator delete  (void *ptr, std::align_val_t al) noexcept;
//void  operator delete[](void *ptr, std::align_val_t al) noexcept;
//void  operator delete  (void *ptr, size_t size, std::align_val_t al) noexcept;
//void  operator delete[](void *ptr, size_t size, std::align_val_t al) noexcept;

#endif
