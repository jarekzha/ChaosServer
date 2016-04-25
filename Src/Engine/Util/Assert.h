/*
 * Assert.h
 *
 *  Created on: 2015年11月11日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <assert.h>


// some safe assert, declaring what to do if assert fails
#define ASSERT_DO(assert_statement, do_statement)   \
{                                                   \
    if (!(assert_statement))                        \
    {                                               \
        assert(!#assert_statement);                 \
        do_statement;                               \
    }                                               \
}

#define ASSERT_RETURN(assert_statement, return_value)   \
    ASSERT_DO(assert_statement, return return_value)

#define ASSERT_RETURN_VOID(assert_statement)            \
    ASSERT_RETURN(assert_statement, )

#define ASSERT_CONTINUE(assert_statement)               \
    ASSERT_DO(assert_statement, continue)

#define ASSERT_BREAK(assert_statement)                  \
    ASSERT_DO(assert_statement, break)

