#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>
#include "Error.h"

// It must be a macro, not a constant, because it must be known at compile-time (it is used for array initialization in another module)
#define BONES_COUNT 2

// a helper to release D3D interface if it is not NULL
inline void release_interface(IUnknown* iface)
{
    if( iface != NULL )
        iface->Release();
}

// a helper to call delete[] on pointer to an array(!) if it is not NULL
template<class Type> void delete_array(Type **dynamic_array)
{
    _ASSERT(dynamic_array != NULL);
    if( *dynamic_array != NULL)
    {
        delete[] *dynamic_array;
        *dynamic_array = NULL;
    }
}

// a helper to call delete on pointer if it is not NULL
template<class Type> void delete_pointer(Type **pointer)
{
    _ASSERT(NULL != pointer);
    if( NULL != *pointer)
    {
        delete *pointer;
        *pointer = NULL;
    }
}

// a helper to find out a size of an array defined with `array[]={...}' without doing `sizeof(array)/sizeof(array[0])'
template<size_t SIZE, class T> inline size_t array_size(T (&array)[SIZE])
{
    UNREFERENCED_PARAMETER(array);
    return SIZE;
}
