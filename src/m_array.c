//
// Copyright(C) 2024 Roman Fomin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <string.h>

#include "m_array.h"

void M_ArrayDelete(void *v, size_t esize, int i, int n)
{
    if (v)
    {
        m_array_buffer_t *p = array_ptr(v);

        memmove((char *)v + i * esize, (char *)v + n * esize,
                (p->size - n - i) * esize);
        p->size -= n;
    }
}

void *M_ArrayResize(void *v, size_t esize, int n)
{
    int capacity = array_capacity(v);
    if (n > capacity)
    {
        int new_capacity = 16;
        while (new_capacity < n)
        {
            new_capacity <<= 1;
        }
        v = M_ArrayGrow(v, esize, new_capacity - capacity);
    }

    if (v)
    {
        m_array_buffer_t *p = array_ptr(v);

        int size = p->size;
        if (n > size)
        {
            memset((char *)v + size * esize, 0, (n - size) * esize);
        }

        p->size = n;
    }

    return v;
}

void *M_ArrayCopy(void *dst, void *src, size_t esize)
{
    int size = array_size(src);

    if (!size)
    {
        array_clear(dst);
    }
    else
    {
        dst = M_ArrayResize(dst, esize, size);
        if (dst)
        {
            memcpy(dst, src, size * esize);
        }
    }

    return dst;
}
