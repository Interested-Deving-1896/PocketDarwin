#!/bin/bash
# XNU Additional Build Fixes
# Fixes for int8_t, int16_t, int32_t, int64_t and missing headers

set -e

echo "Applying additional XNU build fixes..."

# Fix 1: Update EXTERNAL_HEADERS/stdint.h to define signed integer types
echo "Fix 1: Adding signed integer type definitions to EXTERNAL_HEADERS/stdint.h"

if [ -f "EXTERNAL_HEADERS/stdint.h" ]; then
    # Create a backup
    cp EXTERNAL_HEADERS/stdint.h EXTERNAL_HEADERS/stdint.h.bak3
    
    # Add the signed integer types right after the unsigned ones
    # We'll insert after the uint64_t typedef
    
    cat > EXTERNAL_HEADERS/stdint.h << 'EOF'
/*
 * Copyright (c) 2004-2006 Apple Computer, Inc. All rights reserved.
 */

#ifndef _STDINT_H_
#define _STDINT_H_

#include <sys/types.h>
#include <machine/types.h>

/* Ensure we have the base unsigned types first */
#ifndef _UINT8_T
#define _UINT8_T
typedef u_int8_t              uint8_t;
#endif

#ifndef _UINT16_T
#define _UINT16_T
typedef u_int16_t            uint16_t;
#endif

#ifndef _UINT32_T
#define _UINT32_T
typedef u_int32_t            uint32_t;
#endif

#ifndef _UINT64_T
#define _UINT64_T
typedef u_int64_t            uint64_t;
#endif

/* Now define the signed types */
#ifndef _INT8_T
#define _INT8_T
typedef signed char          int8_t;
#endif

#ifndef _INT16_T
#define _INT16_T
typedef signed short         int16_t;
#endif

#ifndef _INT32_T
#define _INT32_T
typedef signed int           int32_t;
#endif

#ifndef _INT64_T
#define _INT64_T
typedef signed long long     int64_t;
#endif

/* Minimum-width integer types */
typedef int8_t           int_least8_t;
typedef int16_t         int_least16_t;
typedef int32_t         int_least32_t;
typedef int64_t         int_least64_t;
typedef uint8_t          uint_least8_t;
typedef uint16_t        uint_least16_t;
typedef uint32_t        uint_least32_t;
typedef uint64_t        uint_least64_t;

/* Fastest minimum-width integer types */
typedef int8_t            int_fast8_t;
typedef int16_t          int_fast16_t;
typedef int32_t          int_fast32_t;
typedef int64_t          int_fast64_t;
typedef uint8_t           uint_fast8_t;
typedef uint16_t         uint_fast16_t;
typedef uint32_t         uint_fast32_t;
typedef uint64_t         uint_fast64_t;

/* Integer types capable of holding object pointers */
#if defined(__LP64__)
typedef long                 intptr_t;
typedef unsigned long       uintptr_t;
#else
typedef int                  intptr_t;
typedef unsigned int        uintptr_t;
#endif

/* Greatest-width integer types */
typedef long long            intmax_t;
typedef unsigned long long  uintmax_t;

/* Limits of exact-width integer types */
#define INT8_MIN         (-127-1)
#define INT16_MIN        (-32767-1)
#define INT32_MIN        (-2147483647-1)
#define INT64_MIN        (-9223372036854775807LL-1)

#define INT8_MAX         127
#define INT16_MAX        32767
#define INT32_MAX        2147483647
#define INT64_MAX        9223372036854775807LL

#define UINT8_MAX         255
#define UINT16_MAX        65535
#define UINT32_MAX        4294967295U
#define UINT64_MAX        18446744073709551615ULL

/* Limits of minimum-width integer types */
#define INT_LEAST8_MIN    INT8_MIN
#define INT_LEAST16_MIN   INT16_MIN
#define INT_LEAST32_MIN   INT32_MIN
#define INT_LEAST64_MIN   INT64_MIN

#define INT_LEAST8_MAX    INT8_MAX
#define INT_LEAST16_MAX   INT16_MAX
#define INT_LEAST32_MAX   INT32_MAX
#define INT_LEAST64_MAX   INT64_MAX

#define UINT_LEAST8_MAX   UINT8_MAX
#define UINT_LEAST16_MAX  UINT16_MAX
#define UINT_LEAST32_MAX  UINT32_MAX
#define UINT_LEAST64_MAX  UINT64_MAX

/* Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN     INT8_MIN
#define INT_FAST16_MIN    INT16_MIN
#define INT_FAST32_MIN    INT32_MIN
#define INT_FAST64_MIN    INT64_MIN

#define INT_FAST8_MAX     INT8_MAX
#define INT_FAST16_MAX    INT16_MAX
#define INT_FAST32_MAX    INT32_MAX
#define INT_FAST64_MAX    INT64_MAX

#define UINT_FAST8_MAX    UINT8_MAX
#define UINT_FAST16_MAX   UINT16_MAX
#define UINT_FAST32_MAX   UINT32_MAX
#define UINT_FAST64_MAX   UINT64_MAX

/* Limits of integer types capable of holding object pointers */
#if defined(__LP64__)
#define INTPTR_MIN        INT64_MIN
#define INTPTR_MAX        INT64_MAX
#define UINTPTR_MAX       UINT64_MAX
#else
#define INTPTR_MIN        INT32_MIN
#define INTPTR_MAX        INT32_MAX
#define UINTPTR_MAX       UINT32_MAX
#endif

/* Limits of greatest-width integer types */
#define INTMAX_MIN        INT64_MIN
#define INTMAX_MAX        INT64_MAX
#define UINTMAX_MAX       UINT64_MAX

#endif /* _STDINT_H_ */
EOF

    echo "  ✓ Updated EXTERNAL_HEADERS/stdint.h"
else
    echo "  ✗ Warning: EXTERNAL_HEADERS/stdint.h not found"
fi

# Fix 2: Create missing sys/_posix_availability.h
echo "Fix 2: Creating missing sys/_posix_availability.h"
mkdir -p BUILD/obj/EXPORT_HDRS/bsd/sys

cat > BUILD/obj/EXPORT_HDRS/bsd/sys/_posix_availability.h << 'EOF'
/*
 * POSIX availability macros
 */

#ifndef _SYS__POSIX_AVAILABILITY_H_
#define _SYS__POSIX_AVAILABILITY_H_

/* Define empty macros for POSIX availability annotations */
#if !defined(__POSIX_C_DEPRECATED)
#define __POSIX_C_DEPRECATED(ver)
#endif

#if !defined(__DARWIN_C_ANSI)
#define __DARWIN_C_ANSI         010000L
#endif

#if !defined(__DARWIN_C_FULL)
#define __DARWIN_C_FULL         900000L
#endif

#if !defined(__DARWIN_C_LEVEL)
#define __DARWIN_C_LEVEL        __DARWIN_C_FULL
#endif

#if !defined(__DARWIN_ONLY_64_BIT_INO_T)
#define __DARWIN_ONLY_64_BIT_INO_T 0
#endif

#if !defined(__DARWIN_ONLY_UNIX_CONFORMANCE)
#define __DARWIN_ONLY_UNIX_CONFORMANCE 0
#endif

#if !defined(__DARWIN_ONLY_VERS_1050)
#define __DARWIN_ONLY_VERS_1050 0
#endif

#if !defined(__DARWIN_SUF_64_BIT_INO_T)
#define __DARWIN_SUF_64_BIT_INO_T
#endif

#if !defined(__DARWIN_SUF_1050)
#define __DARWIN_SUF_1050
#endif

#if !defined(__DARWIN_SUF_EXTSN)
#define __DARWIN_SUF_EXTSN
#endif

#if !defined(__DARWIN_SUF_UNIX03)
#define __DARWIN_SUF_UNIX03
#endif

#if !defined(__DARWIN_UNIX03)
#define __DARWIN_UNIX03 1
#endif

#if !defined(__DARWIN_64_BIT_INO_T)
#define __DARWIN_64_BIT_INO_T 0
#endif

#if !defined(__DARWIN_VERS_1050)
#define __DARWIN_VERS_1050 0
#endif

#if !defined(__DARWIN_NON_CANCELABLE)
#define __DARWIN_NON_CANCELABLE 0
#endif

/* Version macros */
#ifndef __API_TO_BE_DEPRECATED
#define __API_TO_BE_DEPRECATED 100000
#endif

#ifndef __MAC_10_0
#define __MAC_10_0      1000
#define __MAC_10_1      1010
#define __MAC_10_2      1020
#define __MAC_10_3      1030
#define __MAC_10_4      1040
#define __MAC_10_5      1050
#define __MAC_10_6      1060
#define __MAC_10_7      1070
#define __MAC_10_8      1080
#define __MAC_10_9      1090
#define __MAC_10_10     101000
#define __MAC_10_11     101100
#define __MAC_10_12     101200
#endif

#endif /* _SYS__POSIX_AVAILABILITY_H_ */
EOF

echo "  ✓ Created BUILD/obj/EXPORT_HDRS/bsd/sys/_posix_availability.h"

echo ""
echo "Additional fixes applied successfully!"
echo ""
echo "You can now try building again with: make"