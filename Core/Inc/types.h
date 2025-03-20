#ifndef TYPES_H
#define TYPES_H

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i08;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u08;
typedef uint8_t u8;

typedef i64 b64;
typedef i32 b32;
typedef i16 b16;
typedef i08 b08;
typedef i8 b8;

typedef float r32;
typedef double r64;

#define function static
#define local_persist static
#define global_variable static

#define Assert(exp) {if(!(exp)){while(1);}}
#define Array_Count(arr) (sizeof(arr) / sizeof(arr[0]))
#define Member(type, member) (((type *) 0)->member)
#define offset_of(type, member) (&Member(type, member)) 
#define Invalid_Code_Path Assert(!"Invalid code path")
#define Invalid_Default_Case default: {Invalid_Code_Path;} break;
#define FOURCC(str) (((u32)(str[0]) << 0) | ((u32)(str[1]) << 8) | ((u32)(str[2]) << 16) | ((u32)(str[3]) << 24))

#define maximum(a,b) ((a) > (b)) ? (a) : (b)
#define minimum(a,b) ((a) < (b)) ? (a) : (b)
#define in_between(val, a, b) (((val) > a) && ((val) <= b))

#define KB(val) (1024 * (val))
#define MB(val) (1024 * KB(val))
#define GB(val) (1024 * MB(val))

#define PI 3.14159265359f

#endif //TYPES_H
