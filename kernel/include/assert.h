#ifndef KLIBC_ASSERT_H_INCLUDED
#define KLIBC_ASSERT_H_INCLUDED

#include <assert.h>


/////////////////
//  Functions  //
/////////////////

void _Assert (char *msg);

//////////////
//  Macros  //
//////////////

#define __STR(x) __VAL(x)
#define __VAL(x) #x

#undef assert
#ifdef NDEBUG
    #define assert(test) ((void)0)
#else
    #define assert(test) ((test) ? (void)0 : _Assert(__FILE__ ":" __STR(__LINE__) " " #test))
#endif

#endif