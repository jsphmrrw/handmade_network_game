#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define memory_set memset
#define memory_copy memcpy

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   b8;
typedef int16_t  b16;
typedef int32_t  b32;
typedef int64_t  b64;
typedef float    f32;
typedef double   f64;

#define bytes(n)     (n)
#define kilobytes(n) (bytes(n)*1024)
#define megabytes(n) (kilobytes(n)*1024)

#define global         static
#define local_persist  static

typedef union v2 {
    struct {
        f32 x;
        f32 y;
    };
} v2;

typedef union v3 {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    struct {
        f32 r;
        f32 g;
        f32 b;
    };
} v3;

typedef union v4 {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
} v4;

#define v2(x, y) v2_init(x, y)
v2 v2_init(f32 x, f32 y) {
    v2 v = { x, y };
    return v;
}

#define v3(x, y, z) v3_init(x, y, z)
v3 v3_init(f32 x, f32 y, f32 z) {
    v3 v = { x, y, z };
    return v;
}

#define v4(x, y, z, w) v4_init(x, y, z, w)
v4 v4_init(f32 x, f32 y, f32 z, f32 w) {
    v4 v = { x, y, z, w };
    return v;
}
