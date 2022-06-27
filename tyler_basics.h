#ifndef TYLER_BASICS_H
#define TYLER_BASICS_H

#ifndef TYLER_BASICS_NO_UNITY_BUILD
#define TYLER_BASICS_IMPLEMENTATION
#define tyler_function internal
#define tyler_global   global
#else // TYLER_BASICS_UNITY_BUILD
#error This file is not arranged yet to work without a unity build!
#endif // TYLER_BASICS_UNITY_BUILD

#include <stdint.h>
#include <float.h>
// Remove this dependency
#include <math.h>

//~ Primitive types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef size_t umw;

typedef float  f32;
typedef double f64;

#define internal        static
#define global          static
#define global_constant static const
#define local_persist   static
#define local_constant  static const

#define Swap(A, B) do { decltype(A) Temp = (A); (A) = (B); (B) = Temp; } while (0)
#define Minimum(A, B) ((A) > (B) ? (B) : (A))
#define Maximum(A, B) ((A) > (B) ? (A) : (B))
#define Clamp(Value, Min, Max) (((Value) < (Min)) ? (Min) : (((Value) > (Max)) ? (Max) : (Value)))

#define ArrayCount(Arr) (sizeof(Arr)/sizeof(*Arr))
#define Kilobytes(Size) (1024*(Size))
#define Megabytes(Size) (1024*Kilobytes(Size))
#define Gigabytes(Size) (1024L*(u64)Megabytes(Size))

#define BEGIN_STRING(X) #X

#if defined(SNAIL_JUMPY_DEBUG_BUILD)
#define Assert(Expr) {if (!(Expr)) __debugbreak();}
#else
#define Assert(Expr) {(Expr);}
#endif

#define INVALID_CODE_PATH   Assert(0)
#define NOT_IMPLEMENTED_YET Assert(0)
#define DONT_COMPILE _ThIs_sHaLlnOt_CoMpIle

#define U8_MAX  0xff
#define U16_MAX 0xffff
#define U32_MAX 0xffffffff
#define U64_MAX 0xffffffffffffffff

#define S8_MAX  0x7f
#define S16_MAX 0x7fff
#define S32_MAX 0x7fffffff
#define S64_MAX 0x7fffffffffffffff

#define S8_MIN  (-S8_MAX-1)
#define S16_MIN (-S16_MAX-1)
#define S32_MIN (-S32_MAX-1)
#define S64_MIN (-S64_MAX-1)

#define F32_POSITIVE_INFINITY INFINITY 
#define F32_NEGATIVE_INFINITY -INFINITY

#define F64_NAN               0x7FF7FFFFFFFFFFFF 
#define F64_POSITIVE_INFINITY 0x7FF0000000000000 
#define F64_NEGATIVE_INFINITY 0xFFF0000000000000 

#define enum8(Enum)  u8
#define enum16(Enum) u16
#define enum32(Enum) u32
#define enum64(Enum) u64

#define GLUE_(a,b) a##b
#define GLUE(a,b) GLUE_(a,b)

//~ Linked list
#define DLIST_ADD(Sentinel, Node) do{ \
(Node)->Prev = (Sentinel); \
(Node)->Next = (Sentinel)->Next; \
(Node)->Prev->Next = (Node); \
(Node)->Next->Prev = (Node); \
}while(0)

#define DLIST_ADD_LAST(Sentinel, Node) do{ \
(Node)->Prev = (Sentinel)->Prev; \
(Node)->Next = (Sentinel); \
(Node)->Prev->Next = (Node); \
(Node)->Next->Prev = (Node); \
}while(0)

#define DLIST_REMOVE(Node) do{ \
(Node)->Next->Prev = (Node)->Prev; \
(Node)->Prev->Next = (Node)->Next; \
(Node)->Next = (Node)->Prev = 0; \
}while(0); 

#define DLIST_INIT(Sentinel) do{ \
(Sentinel)->Next = (Sentinel); \
(Sentinel)->Prev = (Sentinel); \
}while(0);

template <typename T> internal inline T *
FreelistAlloc_(T **List){
    T *Result = *List;
    *List = (*List)->NextFree;
    return Result;
}

#define FREELIST_ALLOC(List, AllocCode) ((List) ? FreelistAlloc_(&(List)) : (AllocCode))

#define FREELIST_FREE(List, Node) do{ \
(Node)->NextFree = (List); \
(List) = (Node); \
}while(0);

//~ Required declarations for platforms
// NOTE(Tyler): The declarations are here so that one knows what tyler_functions need to be implemented for
// different platforms.
// TODO(Tyler): Have this have a default option

#ifdef TYLER_BASICS_USE_MALLOC
#include <stdlib.h>
tyler_function void *
OSDefaultAlloc(umw Size){ return calloc(Size, 1); } // calloc does zero initialization
tyler_function void *
OSDefaultRealloc(void *Memory, umw Size) { return realloc(Memory, Size);  }
tyler_function void *
OSDefaultFree(void *Pointer){ return free(Pointerm); }
#else // TYLER_BASICS_USE_MALLOC
tyler_function void *OSDefaultAlloc(umw Size);
tyler_function void *OSDefaultRealloc(void *Memory, umw Size);
tyler_function void  OSDefaultFree(void *Pointer);
#endif // TYLER_BASICS_USE_MALLOC

#ifdef TYLER_BASICS_IMPLEMENTATION

//~ Basic Math

global_constant f32 PI = 3.141592653589f;
global_constant f32 TAU = 2.0f*PI;

tyler_function inline s32
RoundToS32(f32 A){
    s32 Result;
    if(A < 0) Result = (s32)(A - 0.5f);
    else Result = (s32)(A + 0.5f);
    return(Result);
}

tyler_function inline s32
Truncate(f32 A){
    return((s32)A);
}

tyler_function inline f32
Floor(f32 A){
    return(floorf(A));
}

tyler_function inline f32
Ceil(f32 A){
    return(ceilf(A));
}

tyler_function inline f32
Round(f32 A)
{
    f32 Result;
    if(A < 0) Result = Floor((A - 0.5f));
    else Result = Floor((A + 0.5f));
    return(Result);
}

tyler_function inline u32
CeilToS32(f32 A)
{
    u32 Result = (u32)ceilf(A);
    return(Result);
}

tyler_function inline f32
Square(f32 A)
{
    f32 Result = A*A;
    return(Result);
}

tyler_function inline f32
SquareRoot(f32 A)
{
    f32 Result = sqrtf(A);
    return(Result);
}

tyler_function inline f32
Sin(f32 A)
{
    return(sinf(A));
}

tyler_function inline f32
Cos(f32 A)
{
    return(cosf(A));
}

tyler_function inline f32
Tan(f32 A)
{
    return(tanf(A));
}

tyler_function inline f32
ModF32(f32 A, f32 B)
{
    f32 Result = (f32)fmod(A, B);
    return(Result);
}

tyler_function inline f32
AbsoluteValue(f32 A)
{
    f32 Result = (A < 0) ? -A : A;
    return(Result);
}

tyler_function inline s32
AbsoluteValue(s32 A)
{
    s32 Result = (A < 0) ? -A : A;
    return(Result);
}

tyler_function inline f32
SignOf(f32 A){
    f32 Result = (A < 0) ? -1.0f : 1.0f;
    return(Result);
}

tyler_function inline f32
ToPowerOf(f32 Base, f32 Exponent){
    f32 Result = powf(Base, Exponent);
    return(Result);
}

tyler_function inline f32
SafeRatioN(f32 Numerator, f32 Denominator, f32 N){
    f32 Result = N;
    
    if(Denominator != 0.0f){
        Result = Numerator / Denominator;
    }
    
    return(Result);
}

tyler_function inline f32
SafeRatio0(f32 Numerator, f32 Denominator){
    f32 Result = SafeRatioN(Numerator, Denominator, 0.0f);
    return(Result);
}

tyler_function inline f32
SafeRatio1(f32 Numerator, f32 Denominator){
    f32 Result = SafeRatioN(Numerator, Denominator, 1.0f);
    return(Result);
}

tyler_function inline u64
SafeRatioN(u64 Numerator, u64 Denominator, u64 N){
    u64 Result = N;
    
    if(Denominator != 0.0f){
        Result = Numerator / Denominator;
    }
    
    return(Result);
}

tyler_function inline u64
SafeRatio0(u64 Numerator, u64 Denominator){
    u64 Result = SafeRatioN(Numerator, Denominator, 0);
    return(Result);
}

tyler_function inline f32
Lerp(f32 A, f32 B, f32 T){
    T = Clamp(T, 0.0f, 1.0f);
    f32 Result = T*A + (1.0f-T)*B;
    return(Result);
}

tyler_function inline b8
IsEven(s32 A){
    b8 Result = (A % 2) == 0;
    return(Result);
}

tyler_function inline b8
IsOdd(s32 A){
    b8 Result = (A % 2) == 1;
    return(Result);
}

tyler_function inline s32
NormalizeDegrees(s32 D){
    s32 Result;
    if(D < 0){
        Result = D;
        while(Result < 0) Result += 360; 
    }else{
        Result = D % 360;
    }
    return(Result);
}

//~ V2

union v2 {
    struct {
        f32 X;
        f32 Y;
    };
    struct {
        f32 Width;
        f32 Height;
    };
    f32 E[2];
};

tyler_function inline v2
V2(f32 X, f32 Y){ 
    v2 Result = v2{X, Y}; 
    return(Result);
}

tyler_function inline v2
V2(f32 XY){ 
    v2 Result = V2(XY, XY); 
    return(Result);
}

// TODO(Tyler): Possibly implement operations for this?
union v2s {
    struct {
        s32 X;
        s32 Y;
    };
    struct {
        s32 Width;
        s32 Height;
    };
};

tyler_function inline v2s
V2S(s32 X, s32 Y){ 
    v2s Result = v2s{X, Y}; 
    return(Result);
}

tyler_function inline v2s
V2S(s32 XY){ 
    v2s Result = v2s{XY, XY}; 
    return(Result);
}

tyler_function inline v2s
V2S(v2 A){ 
    v2s Result; 
    Result.X = (s32)A.X;
    Result.Y = (s32)A.Y;
    return(Result);
}

tyler_function inline v2
V2(v2s A){ 
    v2 Result = v2{(f32)A.X, (f32)A.Y}; 
    return(Result);
}

tyler_function inline v2
operator+(v2 A, v2 B)
{
    v2 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return(Result);
}

tyler_function inline v2
operator-(v2 A, v2 B)
{
    v2 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return(Result);
}

tyler_function inline v2
operator-(v2 A)
{
    v2 Result;
    Result.X = -A.X;
    Result.Y = -A.Y;
    return(Result);
}

tyler_function inline v2
V2Invert(v2 A)
{
    v2 Result;
    Result.X = -A.X;
    Result.Y = -A.Y;
    return(Result);
}

tyler_function inline v2
operator*(v2 A, f32 B)
{
    v2 Result;
    Result.X = A.X * B;
    Result.Y = A.Y * B;
    return(Result);
}

tyler_function inline v2
operator*(f32 B, v2 A)
{
    v2 Result;
    Result.X = A.X * B;
    Result.Y = A.Y * B;
    return(Result);
}

tyler_function inline v2
operator/(v2 A, f32 B)
{
    v2 Result;
    Result.X = A.X / B;
    Result.Y = A.Y / B;
    return(Result);
}

tyler_function inline v2
operator+=(v2 &A, v2 B)
{
    A = A + B;
    return(A);
}

tyler_function inline v2
operator-=(v2 &A, v2 B)
{
    A = A - B;
    return(A);
}

tyler_function inline v2
operator*=(v2 &A, f32 B)
{
    A = B * A;
    return(A);
}

tyler_function inline v2
operator/=(v2 &A, f32 B)
{
    A = A / B;
    return(A);
}

tyler_function inline f32
Dot(v2 A, v2 B) {
    f32 Result = (A.X*B.X)+(A.Y*B.Y);
    return(Result);
}

tyler_function inline v2
Hadamard(v2 A, v2 B){
    v2 Result;
    Result.X = A.X*B.X;
    Result.Y = A.Y*B.Y;
    return(Result);
}

tyler_function inline v2
Clockwise90(v2 A, v2 Origin=V2(0,0)){
    A -= Origin;
    v2 Result = V2(A.Y, -A.X);
    Result += Origin;
    return(Result);
}

tyler_function inline v2
CounterClockwise90(v2 A, v2 Origin=V2(0,0)){
    A -= Origin;
    v2 Result = V2(-A.Y, A.X);
    Result += Origin;
    return(Result);
}

tyler_function inline f32
LengthSquared(v2 V){
    f32 Result = Dot(V, V);
    return(Result);
}

tyler_function inline f32
Length(v2 V){
    f32 Result = SquareRoot(LengthSquared(V));
    return(Result);
}

tyler_function inline v2
Normalize(v2 V){
    f32 Length = SquareRoot(LengthSquared(V));
    v2 Result = {};
    if(Length > 0.0f){
        Result = V/Length;
    }
    return(Result);
}

// Perpendicular to A in the direction of B
tyler_function inline v2 
TripleProduct(v2 A, v2 B){
    // A cross B cross A = (A cross B) cross A
    f32 Z = (A.X*B.Y)-(A.Y*B.X);
    v2 Result = V2(-Z*A.Y, Z*A.X);
    return(Result);
}

tyler_function inline v2
MinimumV2(v2 A, v2 B){
    v2 Result;
    Result.X = Minimum(A.X, B.X);
    Result.Y = Minimum(A.Y, B.Y);
    
    return(Result);
}

tyler_function inline v2
MaximumV2(v2 A, v2 B){
    v2 Result;
    Result.X = Maximum(A.X, B.X);
    Result.Y = Maximum(A.Y, B.Y);
    
    return(Result);
}

tyler_function inline v2s
MinimumV2S(v2s A, v2s B){
    v2s Result;
    Result.X = Minimum(A.X, B.X);
    Result.Y = Minimum(A.Y, B.Y);
    
    return(Result);
}

tyler_function inline v2s
MaximumV2S(v2s A, v2s B){
    v2s Result;
    Result.X = Maximum(A.X, B.X);
    Result.Y = Maximum(A.Y, B.Y);
    
    return(Result);
}

tyler_function inline v2
SquareV2(v2 V){
    v2 Result;
    Result.X = V.X*V.X;
    Result.Y = V.Y*V.Y;
    return(Result);
}

tyler_function inline v2
FloorV2(v2 V){
    v2 Result;
    Result.X = Floor(V.X);
    Result.Y = Floor(V.Y);
    return(Result);
}

tyler_function inline v2
RoundV2(v2 V){
    v2 Result;
    Result.X = Round(V.X);
    Result.Y = Round(V.Y);
    return(Result);
}

tyler_function inline v2
CeilV2(v2 V){
    v2 Result;
    Result.X = Ceil(V.X);
    Result.Y = Ceil(V.Y);
    return(Result);
}

tyler_function inline v2
Lerp(v2 A, v2 B, f32 T){
    T = Clamp(T, 0.0f, 1.0f);
    v2 Result = T*A + (1.0f-T)*B;
    return(Result);
}

//~ V2S
tyler_function inline v2s
operator+(v2s A, v2s B)
{
    v2s Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return(Result);
}

tyler_function inline v2s
operator-(v2s A, v2s B)
{
    v2s Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return(Result);
}

tyler_function inline v2s
operator-(v2s A)
{
    v2s Result;
    Result.X = -A.X;
    Result.Y = -A.Y;
    return(Result);
}

tyler_function inline v2s
V2Invert(v2s A)
{
    v2s Result;
    Result.X = -A.X;
    Result.Y = -A.Y;
    return(Result);
}

tyler_function inline v2s
operator*(v2s A, s32 B)
{
    v2s Result;
    Result.X = A.X * B;
    Result.Y = A.Y * B;
    return(Result);
}

tyler_function inline v2s
operator*(s32 B, v2s A)
{
    v2s Result;
    Result.X = A.X * B;
    Result.Y = A.Y * B;
    return(Result);
}

tyler_function inline v2s
operator/(v2s A, s32 B)
{
    v2s Result;
    Result.X = A.X / B;
    Result.Y = A.Y / B;
    return(Result);
}

tyler_function inline v2s
operator+=(v2s &A, v2s B)
{
    A = A + B;
    return(A);
}

tyler_function inline v2s
operator-=(v2s &A, v2s B)
{
    A = A - B;
    return(A);
}

tyler_function inline v2s
operator*=(v2s &A, s32 B)
{
    A = B * A;
    return(A);
}

tyler_function inline v2s
operator/=(v2s &A, s32 B)
{
    A = A / B;
    return(A);
}

//~
union v3s {
    struct {
        s32 X;
        s32 Y;
        s32 Z;
    };
    struct {
        v2s XY;
        s32 Z;
    };
    struct {
        s32 Width;
        s32 Height;
        s32 Depth;
    };
};

tyler_function inline v3s 
V3S(s32 X, s32 Y, s32 Z){
    v3s Result = {X, Y, Z};
    return Result;
}

tyler_function inline v3s 
V3S(s32 XYZ){
    v3s Result = {XYZ, XYZ, XYZ};
    return Result;
}

tyler_function inline v3s 
V3S(v2s XY, s32 Z){
    v3s Result = {XY.X, XY.Y, Z};
    return Result;
}

//~
union v4s {
    struct {
        s32 X;
        s32 Y;
        s32 Z;
        s32 W;
    };
    struct {
        v2s XY;
        v2s ZW;
    };
    struct {
        v3s XYZ;
        s32 W;
    };
};

tyler_function inline v4s 
V4S(s32 X, s32 Y, s32 Z, s32 W){
    v4s Result = {X, Y, Z, W};
    return Result;
}

tyler_function inline v4s 
V4S(s32 XYZW){
    v4s Result = {XYZW, XYZW, XYZW, XYZW};
    return Result;
}

tyler_function inline v4s 
V4S(v2s XY, v2s ZW){
    v4s Result = {XY.X, XY.Y, ZW.X, ZW.Y};
    return Result;
}

//~ Colors
union color {
    struct { 
        f32 R, G, B, A;
    };
    f32 E[4];
};

tyler_function inline color
MakeColor(f32 R, f32 G, f32 B, f32 A=1.0f){
    color Result = color{R, G, B, A};
    return(Result);
}

tyler_function inline color
MixColor(color A, color B, f32 Value){
    Value = Clamp(Value, 0.0f, 1.0f);
    color Result;
    Result.R = Value*A.R + (1.0f-Value)*B.R;
    Result.G = Value*A.G + (1.0f-Value)*B.G;
    Result.B = Value*A.B + (1.0f-Value)*B.B;
    Result.A = Value*A.A + (1.0f-Value)*B.A;
    return(Result);
}

tyler_function inline color
Alphiphy(color Color, f32 Alpha){
    Alpha = Clamp(Alpha, 0.0f, 1.0f);
    color Result = Color;
    Result.A *= Alpha;
    return(Result);
}

tyler_function inline color
operator*(f32 X, color Color)
{
    color Result;
    Result.R = X*Color.R;
    Result.G = X*Color.G;
    Result.B = X*Color.B;
    Result.A = X*Color.A;
    return Result;
}

tyler_function inline color
operator*(color Color, f32 X)
{
    color Result = X*Color;
    return Result;
}

tyler_function inline color
operator*=(color &Color, f32 X)
{
    Color = X*Color;
    return Color;
}

tyler_function inline color
operator/(color Color, f32 X)
{
    color Result;
    Result.R = Color.R/X;
    Result.G = Color.G/X;
    Result.B = Color.B/X;
    Result.A = Color.A/X;
    return Result;
}

tyler_function inline color
operator/=(color &Color, f32 X)
{
    Color = Color/X;
    return Color;
}

typedef u32 color_u32;
tyler_function inline color_u32
ColorU32(color C){
    C*=255.0f;
    
    color_u32 Result = ((((u32)C.R) << 24) |
                        (((u32)C.G) << 16) |
                        (((u32)C.B) << 8) |
                        (((u32)C.A) << 0));
    return Result;
}

tyler_function inline color
MakeColor(color_u32 C){
    color Result = {};
    Result.R = (f32)((C >> 24) & 0xff);
    Result.G = (f32)((C >> 16) & 0xff);
    Result.B = (f32)((C >> 8)  & 0xff);
    Result.A = (f32)((C >> 0)  & 0xff);
    Result /= 255.0f;
    return Result;
}

//~ Basic colors

global_constant color BLACK      = MakeColor(0.0f,  0.0f,  0.0f, 1.0f);
global_constant color WHITE      = MakeColor(1.0f,  1.0f,  1.0f, 1.0f);
global_constant color RED        = MakeColor(1.0f,  0.0f,  0.0f, 1.0f);
global_constant color YELLOW     = MakeColor(1.0f,  1.0f,  0.0f, 1.0f);
global_constant color BLUE       = MakeColor(0.0f,  0.0f,  1.0f, 1.0f);
global_constant color GREEN      = MakeColor(0.0f,  1.0f,  0.0f, 1.0f);
global_constant color DARK_GREEN = MakeColor(0.0f,  0.5f,  0.0f, 1.0f);
global_constant color BROWN      = MakeColor(0.41f, 0.20f, 0.0f, 1.0f);
global_constant color PINK       = MakeColor(1.0f,  0.0f,  1.0f, 1.0f);
global_constant color PURPLE     = MakeColor(0.42f, 0.05f, 0.68f,1.0f);
global_constant color ORANGE     = MakeColor(1.0f,  0.5f,  0.0f, 1.0f);

//~ HSB color
struct hsb_color {
    f32 Hue, Saturation, Brightness;
};

tyler_function hsb_color 
HSBColor(f32 Hue, f32 Saturation, f32 Brightness){
    hsb_color Result = {Hue, Saturation, Brightness};
    return(Result);
}

tyler_function color
HSBToRGB(hsb_color HSBColor){
    f32 Hue = Clamp(HSBColor.Hue, 0.0f, 360.0f);
    Hue /= 60.0f;
    f32 Saturation = Clamp(HSBColor.Saturation, 0.0f, 1.0f);
    f32 Brightness = Clamp(HSBColor.Brightness, 0.0f, 1.0f);
    
    
    f32 Chroma = Brightness*Saturation;
    f32 X      = Chroma * (1.0f - AbsoluteValue(ModF32(Hue, 2.0f) - 1.0f)); 
    
    color Result = {};
    u32 HueU32 = (u32)Hue;
    switch(HueU32){
        case 0: { Result = MakeColor(Chroma,      X,   0.0f); }break;
        case 1: { Result = MakeColor(     X, Chroma,   0.0f); }break;
        case 2: { Result = MakeColor(  0.0f, Chroma,      X); }break;
        case 3: { Result = MakeColor(  0.0f,      X, Chroma); }break;
        case 4: { Result = MakeColor(     X,   0.0f, Chroma); }break;
        case 5: 
        case 6: { Result = MakeColor(Chroma,   0.0f,      X); }break;
        default: { INVALID_CODE_PATH; }break;
    }
    
    f32 M = Brightness-Chroma;
    Result.R += M;
    Result.G += M;
    Result.B += M;
    
    return(Result);
}

//~ Rectangles
union rect {
    struct {
        v2 Min;
        v2 Max;
    };
    struct {
        v2 E[2];
    };
    struct {
        f32 X0, Y0;
        f32 X1, Y1;
    };
    struct {
        f32 Left,  Bottom;
        f32 Right, Top;
    };
};

struct rect_s32 {
    v2s Min;
    v2s Max;
};

tyler_function inline rect
operator+(rect A, v2 B){
    rect Result;
    Result.Min = A.Min + B;
    Result.Max = A.Max + B;
    return(Result);
}

tyler_function inline rect
operator+(v2 B, rect A){
    rect Result;
    Result.Min = A.Min + B;
    Result.Max = A.Max + B;
    return(Result);
}

tyler_function inline rect
operator-(rect A, v2 B){
    rect Result;
    Result.Min = A.Min - B;
    Result.Max = A.Max - B;
    return(Result);
}

tyler_function inline rect
operator*(rect A, f32 B){
    rect Result;
    Result.Min = A.Min * B;
    Result.Max = A.Max * B;
    return(Result);
}

tyler_function inline rect
operator*(f32 B, rect A){
    rect Result;
    Result.Min = A.Min * B;
    Result.Max = A.Max * B;
    return(Result);
}

tyler_function inline rect
operator/(rect A, f32 B){
    rect Result;
    Result.Min = A.Min / B;
    Result.Max = A.Max / B;
    return(Result);
}

tyler_function inline rect
operator+=(rect &A, v2 B){
    A.Min += B;
    A.Max += B;
    return(A);
}

tyler_function inline rect
operator-=(rect &A, v2 B){
    A.Min -= B;
    A.Max -= B;
    return(A);
}

tyler_function inline rect
operator*=(rect &A, f32 B){
    A.Min *= B;
    A.Max *= B;
    return(A);
}

tyler_function inline rect
operator/=(rect &A, f32 B){
    A.Min /= B;
    A.Max /= B;
    return(A);
}

tyler_function inline rect
MakeRect(v2 Min, v2 Max){
    rect Result;
    Result.Min = Min;
    Result.Max = Max;
    return(Result);
}

tyler_function inline rect_s32
RectS32(v2s Min, v2s Max){
    rect_s32 Result;
    Result.Min = Min;
    Result.Max = Max;
    return(Result);
}

tyler_function inline rect_s32 
RectS32(rect Rect){
    rect_s32 Result;
    Result.Min.X = Truncate(Rect.Min.X);
    Result.Min.Y = Truncate(Rect.Min.Y);
    Result.Max.X = (s32)Ceil(Rect.Max.X);
    Result.Max.X = (s32)Ceil(Rect.Max.X);
    Result.Max.Y = (s32)Ceil(Rect.Max.Y);
    return(Result);
}

tyler_function inline rect
CenterRect(v2 P, v2 Size){
    rect Result;
    Result.Min = P - 0.5f*Size;
    Result.Max = P + 0.5f*Size;
    return(Result);
}

tyler_function inline rect
RectRectify(rect Rect){
    rect Result = {};
    Result.Min.X = Minimum(Rect.Min.X, Rect.Max.X);
    Result.Min.Y = Minimum(Rect.Min.Y, Rect.Max.Y);
    Result.Max.X = Maximum(Rect.Min.X, Rect.Max.X);
    Result.Max.Y = Maximum(Rect.Min.Y, Rect.Max.Y);
    
    return(Result);
}

tyler_function inline rect
TopLeftRect(v2 TopLeft, v2 Size){
    rect Result;
    Result.Min = V2(TopLeft.X,        TopLeft.Y-Size.Y);
    Result.Max = V2(TopLeft.X+Size.X, TopLeft.Y);
    return(Result);
}

tyler_function inline rect
SizeRect(v2 Min, v2 Size){
    rect Result;
    Result.Min = Min;
    Result.Max = Min+Size;
    return(Result);
}

tyler_function inline v2
RectSize(rect Rect){
    v2 Result = Rect.Max - Rect.Min;
    return(Result);
}

tyler_function inline f32
RectWidth(rect Rect){
    f32 Result = Rect.Right - Rect.Left;
    return Result;
}

tyler_function inline b8
IsPointInRect(v2 Point, rect Rect){
    b8 Result = ((Rect.Min.X < Point.X) && (Point.X < Rect.Max.X) &&
                 (Rect.Min.Y < Point.Y) && (Point.Y < Rect.Max.Y));
    return(Result);
}

tyler_function inline b8
DoRectsOverlap(rect A, rect B){
    b8 Result = ((A.Min.X <= B.Max.X) &&
                 (B.Min.X <= A.Max.X) &&
                 (A.Min.Y <= B.Max.Y) &&
                 (B.Min.Y <= A.Max.Y));
    return(Result);
}

tyler_function inline rect
RectGrow(rect Rect, f32 G){
    rect Result = Rect;
    Result.Min -= V2(G, G);
    Result.Max += V2(G, G);
    return(Result);
}

tyler_function inline rect
RectGrow(rect Rect, v2 G){
    rect Result = Rect;
    Result.Min -= G;
    Result.Max += G;
    return(Result);
}

tyler_function inline v2
RectCenter(rect Rect){
    v2 Result = {};
    v2 Size = RectSize(Rect);
    Result = Rect.Min + 0.5f*Size;
    
    return(Result);
}

tyler_function inline rect
RectSweep(rect RectA, v2 Delta){
    rect RectB = RectA + Delta;
    rect Result;
    Result.Min = MinimumV2(RectA.Min, RectB.Min);
    Result.Max = MaximumV2(RectA.Max, RectB.Max);
    
    return(Result);
}

tyler_function inline rect
RectLerp(rect A, rect B, f32 T){
    rect Result;
    Result.Min = Lerp(A.Min, B.Min, T);
    Result.Max = Lerp(A.Max, B.Max, T);
    return(Result);
}

tyler_function inline rect
RectPercent(rect R, f32 X0Percent, f32 Y0Percent, f32 X1Percent, f32 Y1Percent){
    rect Result;
    v2 Size = RectSize(R);
    Result.X0 = R.X0 + Size.X*X0Percent;
    Result.Y0 = R.Y0 + Size.Y*Y0Percent;
    Result.X1 = R.X0 + Size.X*X1Percent;
    Result.Y1 = R.Y0 + Size.Y*Y1Percent;
    return Result;
}

tyler_function inline rect
RectMoveUp(rect R, f32 Amount){
    R.Y0 += Amount;
    R.Y1 += Amount;
    return R;
}

tyler_function inline rect
RectMoveRight(rect R, f32 Amount){
    R.X0 += Amount;
    R.X1 += Amount;
    return R;
}

tyler_function inline rect
RectRound(rect R){
    R.Min = RoundV2(R.Min);
    R.Max = RoundV2(R.Max);
    return R;
}

//~ Range
union range_s32 {
    struct {
        s32 Min;
        s32 Max;
    };
    struct {
        s32 Start;
        s32 End;
    };
};

tyler_function inline range_s32
MakeRangeS32(s32 Start, s32 End){
    range_s32 Result;
    Result.Start = Minimum(Start, End);
    Result.End   = Maximum(Start, End);
    return Result;
}

tyler_function inline range_s32
SizeRangeS32(s32 Start, s32 Size){
    range_s32 Result = MakeRangeS32(Start, Start+Size);
    return Result;
}

tyler_function inline s32
RangeSize(range_s32 Range){
    return Range.End-Range.Start;
}

tyler_function inline range_s32
RangeRectify(range_s32 Range){
    range_s32 Result;
    Result.Start = Minimum(Range.Start, Range.End);
    Result.End   = Maximum(Range.Start, Range.End);
    return Result;
}

tyler_function inline range_s32
RangeShift(range_s32 Range, s32 Shift){
    range_s32 Result = Range;
    Result.Start += Shift;
    Result.End   += Shift;
    return Result;
}

tyler_function inline b8
RangeContains(range_s32 Range, s32 Value){
    b8 Result = (Range.Min < Value) && (Value < Range.Max);
    return Result;
}

tyler_function inline b8
RangeContainsInclusive(range_s32 Range, s32 Value){
    b8 Result = (Range.Min <= Value) && (Value <= Range.Max);
    return Result;
}

tyler_function inline b8
RangeContains(range_s32 A, range_s32 B){
    b8 Result = (A.Min < B.Min) && (B.Max < A.Max);
    return Result;
}

tyler_function inline b8
RangeContainsInclusive(range_s32 A, range_s32 B){
    b8 Result = (A.Min <= B.Min) && (B.Max <= A.Max);
    return Result;
}

tyler_function inline range_s32
RangeCrop(range_s32 Super, range_s32 Sub){
    range_s32 Result;
    Result.Min = Maximum(Super.Min, Sub.Min);
    Result.Max = Minimum(Super.Max, Sub.Max);
    return Result;
}

//~ Intrinsics
struct bit_scan_result
{
    u32 Index;
    b8 Found;
};

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#pragma intrinsic(_BitScanForward)

tyler_function inline bit_scan_result
ScanForLeastSignificantSetBit(u64 Mask){
    bit_scan_result Result;
    Result.Found = _BitScanForward64(&(unsigned long)Result.Index, Mask);
    return(Result);
}

tyler_function inline bit_scan_result
ScanForMostSignificantSetBit(u64 Mask){
    bit_scan_result Result;
    Result.Found = _BitScanReverse64(&(unsigned long)Result.Index, Mask);
    return(Result);
}

tyler_function inline u32
CountLeadingZeroes(u32 Value){
    u32 Result = (u32)__lzcnt(Value);
    return(Result);
}

tyler_function inline u32
CountLeadingOnes(u32 Value){
    u32 Result = (u32)__lzcnt(~Value);
    return(Result);
}

tyler_function void
CopyMemory(void *To, const void *From, umw Size) {
#if 0
    for (umw I = 0; I < Size; I++)
    {
        *((u8*)To+I) = *((u8*)From+I);
    }
#else
    __movsb((u8 *)To, (u8 *)From, Size);
#endif
}

tyler_function void
ZeroMemory(void *Memory, umw Size) {
#if 0
    for (umw I = 0; I < Size; I++){
        *((u8*)Memory+I) = 0;
    }
#else
    __stosb((u8 *)Memory, 0, Size);
#endif
}

tyler_function u32 
PopcountU32(u32 A){
    u32 Result = _mm_popcnt_u32(A);
    return(Result);
}

tyler_function inline u64 
AtomicAddU64(u64 volatile *Value, u64 Addend)
{
    u64 Result = _InterlockedExchangeAdd64((__int64 volatile *)Value, Addend);
    return Result;
}

#else 
#error Please implement intrinsics for this compiler!
#endif

//~ Threading

struct ticket_mutex {
    u64 TicketCounter;
    u64 Serving;
};

tyler_function inline void 
TicketMutexBegin(ticket_mutex *Mutex){
    u64 Ticket = AtomicAddU64(&Mutex->TicketCounter, 1);
    while(Mutex->Serving != Ticket) { _mm_pause(); }
}

tyler_function inline void 
TicketMutexEnd(ticket_mutex *Mutex){
    AtomicAddU64(&Mutex->Serving, 1);
}

//~ General helpers

tyler_function u32
CStringLength(const char *String){
    u32 Result = 0;
    for(char C = *String; C; C = *(++String)){
        Result++;
    }
    return(Result);
}

tyler_function void
CopyCString(char *To, const char *From, u32 MaxSize){
    u32 I = 0;
    while(From[I] && (I < MaxSize)){
        To[I] = From[I];
        I++;
    }
    To[I] = '\0';
}

tyler_function void
CopyCString(char *To, const char *From){
    CopyCString(To, From, CStringLength(From));
}

//~
// NOTE(Tyler): This exists because windows has crlf line endings!!!
tyler_function inline b8
IsNewLine(char C){
    b8 Result = ((C == '\n') ||
                 (C == '\r'));
    return Result;
}

tyler_function inline b8
IsWhiteSpace(char C){
    b8 Result = ((C == ' ') ||
                 (C == '\t') ||
                 (C == '\n') ||
                 (C == '\r'));
    return(Result);
}

tyler_function inline b8
IsALetter(char C){
    b8 Result = ((('a' <= C) && (C <= 'z')) ||
                 (('A' <= C) && (C <= 'Z')));
    return(Result);
}

tyler_function inline b8
IsANumber(char C){
    b8 Result = (('0' <= C) && (C <= '9'));
    return(Result);
}

tyler_function inline char 
CharToLower(char C){
    if((('A' <= C) && (C <= 'Z'))) C -= 'A'-'a';
    return C;
}

tyler_function inline void 
CStringMakeLower(char *S){
    for(u32 I=0; S[I]; I++){
        if((('A' <= S[I]) && (S[I] <= 'Z'))) S[I] -= 'A'-'a';
    }
}

tyler_function constexpr b8
CompareCStrings(const char *A, const char *B){
    while(*A && *B){
        if(*A++ != *B++){
            return false;
        }
    }
    if(*A != *B) return false;
    
    return true;
}

tyler_function b8
IsStringASubset(const char *A, const char *B){
    while(*A && *B){
        if(*A++ != *B++){
            return false;
        }
    }
    
    return true;
}

tyler_function constexpr u64
HashString(const char *String){
    u64 Result = 71984823;
    while(char Char = *String++) {
        Result += (Char << 5) + Char;
    }
    return(Result);
}

tyler_function inline b8
IsFirstStringFirst(const char *A, const char *B){
    while(*A && *B){
        if(*A < *B) return true;
        else if(*A > *B) return false;
        
        *A++;
        *B++;
    }
    
    return (*A == 0);
}

//~ Memory arena

tyler_function inline umw
AlignValue(umw Value, umw Alignment){
    umw Result = ((Value+(Alignment-1)) & ~(Alignment-1));
    return(Result);
}

typedef u32 memory_flags;
enum memory_flags_ {
    MemoryFlag_None       = (0 << 0),
    MemoryFlag_ZeroMemory = (1 << 1),
};

struct memory_info {
    memory_flags Flags;
    umw Alignment;
};

tyler_function inline memory_info
ZeroAndAlign(umw Alignment=4){
    memory_info Result;
    Result.Flags = MemoryFlag_ZeroMemory;
    Result.Alignment = Alignment;
    return(Result);
}

tyler_function inline memory_info
NoneAndAlign(umw Alignment=4){
    memory_info Result;
    Result.Flags = MemoryFlag_None;
    Result.Alignment = Alignment;
    return(Result);
}

struct memory_arena {
    u8 *Memory;
    umw Used;
    umw Size;
};

tyler_function void
InitializeArena(memory_arena *Arena, void *Memory, umw Size){
    *Arena = {};
    Arena->Memory = (u8 *)Memory;
    Arena->Size = Size;
}

tyler_function memory_arena
MakeArena(memory_arena *Arena, umw Size){
    Assert((Arena->Used+Size) < Arena->Size);
    memory_arena Result;
    Result.Memory = Arena->Memory+Arena->Used;
    Arena->Used += Size;
    Result.Size = Size;
    Result.Used = 0;
    ZeroMemory(Result.Memory, Result.Size);
    return(Result);
}

#define PushStruct(Arena, Type) (Type *)ArenaPush(Arena, sizeof(Type))
#define PushArray(Arena, Type, Count) (Type *)ArenaPush(Arena, sizeof(Type)*(Count))

#define PushSpecialStruct(Arena, Type, Info) (Type *)ArenaPush(Arena, sizeof(Type), Info)
#define PushSpecialArray(Arena, Type, Count, Info) (Type *)ArenaPush(Arena, sizeof(Type)*(Count), Info)


tyler_function void *
ArenaPush(memory_arena *Arena, umw Size, memory_info Info=ZeroAndAlign(4)){
    Size = AlignValue(Size, Info.Alignment);
    umw UnAligned = (umw)(Arena->Memory+Arena->Used);
    u8 *Result = (u8 *)AlignValue(UnAligned, Info.Alignment);
    umw Difference = (umw)Result - UnAligned;
    Arena->Used += Size+Difference;
    Assert(Arena->Used <= Arena->Size);
    
    if(Info.Flags & MemoryFlag_ZeroMemory){
        ZeroMemory(Result, Size);
    }
    
    
    return(Result);
}

tyler_function void *
ArenaResizeMemory(memory_arena *Arena, void *OldMemory, umw OldSize, umw NewSize, 
                  memory_info Info=ZeroAndAlign(4)){
    // We just forget about the old allocation, this shouldn't probably shouldn't be
    // used in arenas that are never cleared
    void *Result = ArenaPush(Arena, NewSize, Info);
    CopyMemory(Result, OldMemory, OldSize);
    
    return(Result);
}

tyler_function inline char *
ArenaPushCString(memory_arena *Arena, const char *String){
    u32 Size = CStringLength(String)+1;
    char *Result = PushArray(Arena, char, Size);
    CopyCString(Result, String, Size);
    return(Result);
}

tyler_function inline char *
ArenaPushLowerCString(memory_arena *Arena, const char *S){
    u32 L = CStringLength(S);
    char *Result = PushArray(Arena, char, L+1);
    for(u32 I=0; S[I]; I++){
        Result[I] = CharToLower(S[I]);
    }
    Result[L] = 0;
    return Result;
}

tyler_function void
ArenaClear(memory_arena *Arena){
    Arena->Used = 0;
}

//~ Memory arena markers
struct memory_arena_marker {
    umw Used;
};

tyler_function inline memory_arena_marker 
ArenaBeginMarker(memory_arena *Arena){
    memory_arena_marker Result = {};
    Result.Used = Arena->Used;
    return(Result);
}

tyler_function inline void
ArenaEndMarker(memory_arena *Arena, memory_arena_marker *Marker){
    Assert(Arena->Used >= Marker->Used);
    Arena->Used = Marker->Used;
}

//~ Variable definitions

global memory_arena GlobalPermanentMemory;
global memory_arena GlobalTransientMemory;

//~

tyler_function void
MoveMemory(void *To, const void *From, umw Size){
    if(Size == 0) return;
    u8 *Temp = PushArray(&GlobalTransientMemory, u8, Size);
    CopyMemory(Temp, From, Size);
    CopyMemory(To, Temp, Size);
}

//~ Array

template<typename T>
struct array {
    T *Items;
    u32 Count;
    u32 MaxCount;
    
    inline T &operator[](s64 Index){
        Assert(Index < Count);
        return(Items[Index]);
    }
    
    inline operator b8(){  return (Items != 0) || (Count > 0); }
};

template<typename T> tyler_function inline array<T>
MakeArray(memory_arena *Arena, u32 MaxCount){
    array<T> Result = {};
    Result.Items = PushArray(Arena, T, MaxCount);
    Result.MaxCount = MaxCount;
    return(Result);
}

template<typename T> tyler_function inline array<T>
MakeFullArray(memory_arena *Arena, u32 Count, umw Alignment=4){
    array<T> Result = {};
    Result.Items = PushSpecialArray(Arena, T, Count, ZeroAndAlign(Alignment));
    Result.Count = Count;
    Result.MaxCount = Count;
    return(Result);
}

template<typename T> tyler_function inline array<T>
MakeFullArray(T *Items, u32 Count){
    array<T> Result = {};
    Result.Items = Items;
    Result.Count = Count;
    Result.MaxCount = Count;
    return(Result);
}

template<typename T> tyler_function inline T
ArrayGet(array<T> *Array, s64 Index){
    Assert(Index < Array->Count);
    return Array->Items[Index];
}

template<typename T> tyler_function inline void
ArrayClear(array<T> *Array){
    Array->Count = 0;
}

template<typename T> tyler_function inline void
ArrayAdd(array<T> *Array, T Item){
    if(Array->Count+1 <= Array->MaxCount){
        Array->Items[Array->Count++] = Item;
    }else{
        Assert(0);
    }
}

template<typename T> tyler_function inline b8
ArrayMaybeAdd(array<T> *Array, T Item){
    if(Array->Count+1 <= Array->MaxCount){
        Array->Items[Array->Count++] = Item;
        return true;
    }else{
        return false;
    }
}

template<typename T> tyler_function inline T *
ArrayAlloc(array<T> *Array, u32 N=1){
    T *Result = 0;
    if(Array->Count+N <= Array->MaxCount){
        Result = &Array->Items[Array->Count];
        Array->Count += N;
    }else{
        Assert(0);
    }
    ZeroMemory(Result, sizeof(T));
    return(Result);
}

// A better insert might be better,
// following the same logic as ordered and unordered remove 
template<typename T> void
ArrayInsert(array<T> *Array, u32 Index, T Item){
    Assert(Index <= Array->Count);
    MoveMemory(&Array->Items[Index+1], 
               &Array->Items[Index], 
               (Array->Count-Index)*sizeof(T));
    Array->Items[Index] = Item;
    Array->Count++;
}

template<typename T> tyler_function inline T *
ArrayInsertAlloc(array<T> *Array, u32 Index){
    Assert(Index <= Array->Count);
    MoveMemory(&Array->Items[Index+1], 
               &Array->Items[Index], 
               (Array->Count-Index)*sizeof(T));
    T *NewItem = &Array->Items[Index];
    Array->Count++;
    return(NewItem);
}

template<typename T> tyler_function inline void
ArrayOrderedRemove(array<T> *Array, u32 Index){
    Assert(Index < Array->Count);
    MoveMemory(&Array->Items[Index], 
               &Array->Items[Index+1], 
               (Array->Count-Index)*sizeof(T));
    Array->Count--;
}

template<typename T> tyler_function inline void
ArrayUnorderedRemove(array<T> *Array, u32 Index){
    Array->Items[Index] = Array->Items[Array->Count-1];
    Array->Count--;
}

template<typename T> tyler_function inline b8
ArrayRemoveByValue(array<T> *Array, T Value){
    for(u32 I=0; I<Array->Count; I++){
        if(ArrayGet(Array, I) == Value){
            ArrayOrderedRemove(Array, I);
            return true;
        }
    }
    return false;
}

template<typename T> tyler_function inline array<T>
ArrayFinalize(memory_arena *Arena, array<T> *Array){
    array<T> Result = MakeArray<T>(Arena, Array->Count);
    for(u32 I=0; I<Array->Count; I++){
        *ArrayAlloc(&Result) = ArrayGet(Array, I);
    }
    return Result;
}

//~ Stack
template<typename T> 
using stack = array<T>;

template<typename T> tyler_function inline stack<T>
MakeStack(memory_arena *Arena, u32 MaxCount){
    stack<T> Result = {};
    Result = MakeArray<T>(Arena, MaxCount);
    return(Result);
}

template<typename T> tyler_function inline void
StackPush(stack<T> *Stack, T Item){
    ArrayAdd(Stack, Item);
}

template<typename T> tyler_function inline void
StackPushSafe(stack<T> *Stack, T Item){
    if(Stack->Count == Stack->MaxCount){
        ArrayOrderedRemove(Stack, 0);
    }
    ArrayAdd(Stack, Item);
}

template<typename T> tyler_function inline T *
StackPushAlloc(stack<T> *Stack, u32 N=1){
    T *Result = ArrayAlloc(Stack, N);
    return(Result);
}

template<typename T> tyler_function inline T
StackPop(stack<T> *Stack){
    T Result = ArrayGet(Stack, Stack->Count-1);
    ArrayOrderedRemove(Stack, Stack->Count-1);
    return(Result);
}

template<typename T> tyler_function inline T
StackPeek(stack<T> *Stack, u32 N=0){
    N++;
    Assert(N <= Stack->Count);
    T Result = Stack->Items[Stack->Count-N];
    return(Result);
}

template<typename T> tyler_function inline void
StackClear(stack<T> *Stack){
    ArrayClear(Stack);
}


//~ Dynamic array
template <typename T>
struct dynamic_array {
    memory_arena *Arena;
    
    T *Items;
    u32 Count;
    u32 Capacity;
    
    inline T &operator[](s64 Index){
        Assert(Index < Count);
        return(Items[Index]);
    }
    
    inline operator b8(){  return(Items != 0); }
    inline operator b16(){ return(Items != 0); }
    inline operator b32(){ return(Items != 0); }
    inline operator b64(){ return(Items != 0); }
};

template <typename T> tyler_function void 
InitializeArray(dynamic_array<T> *Array, int InitialCapacity, memory_arena *Arena=0){
    *Array = {};
    if(Arena) Array->Items = PushArray(Arena, T, InitialCapacity);
    else Array->Items = (T *)OSDefaultAlloc(InitialCapacity*sizeof(T));
    Array->Arena = Arena;
    Array->Capacity = InitialCapacity;
}

template <typename T> tyler_function inline dynamic_array<T>
MakeDynamicArray(s32 InitialCapacity, memory_arena *Arena=0){
    dynamic_array<T> Result = {};
    if(Arena) Result.Items = PushArray(Arena, T, InitialCapacity);
    else Result.Items = (T *)OSDefaultAlloc(InitialCapacity*sizeof(T));
    Result.Arena = Arena;
    Result.Capacity = InitialCapacity;
    
    return Result;
}

template <typename T> tyler_function inline dynamic_array<T>
MakeDynamicArray(memory_arena *Arena, s32 InitialCapacity){
    return MakeDynamicArray<T>(InitialCapacity, Arena);
}

template <typename T> tyler_function inline array<T>
MakeArray(dynamic_array<T> *Array){
    array<T> Result = MakeFullArray(Array->Items, Array->Count);
    return Result;
}

template <typename T> void 
DeleteArray(dynamic_array<T> *Array){
    if(!Array->Arena) DefaultFree(Array->Items);
}

template <typename T> tyler_function void 
ArrayClear(dynamic_array<T> *Array){
    Array->Count = 0;
}

template <typename T> tyler_function void 
ArrayAdd(dynamic_array<T> *Array, T *item){
    if(Array->Count >= Array->Capacity){
        umw OldSize = Array->Capacity*sizeof(T);
        umw NewSize = 2*Array->Capacity*sizeof(T);
        Array->Capacity *= 2;
        if(Array->Arena) Array->Items = (T *)ArenaResizeMemory(Array->Arena, Array->Items, OldSize, NewSize);
        else Array->Items = (T *)OSDefaultRealloc(Array->Items, NewSize);
    }
    Array->Items[Array->Count++] = *item;
}

template <typename T> void 
ArrayAdd(dynamic_array<T> *Array, T item){
    ArrayAdd(Array, &item);
}

template<typename T> tyler_function inline T *
ArrayAlloc(dynamic_array<T> *Array, u32 N=1){
    T *Result = 0;
    if(Array->Count+N >= Array->Capacity){
        umw OldSize = Array->Capacity*sizeof(T);
        umw NewSize = 2*Array->Capacity*sizeof(T);
        Array->Capacity *= 2;
        if(Array->Arena) Array->Items = (T *)ArenaResizeMemory(Array->Arena, Array->Items, OldSize, NewSize);
        else Array->Items = (T *)OSDefaultRealloc(Array->Items, NewSize);
    }
    Result = &Array->Items[Array->Count];
    Array->Count += N;
    *Result = {};
    return(Result);
}

template<typename T> tyler_function void
ArrayInsert(dynamic_array<T> *Array, u32 Index, T Item){
    if(Array->Count+1 >= Array->Capacity){
        umw OldSize = Array->Capacity*sizeof(T);
        umw NewSize = 2*Array->Capacity*sizeof(T);
        Array->Capacity *= 2;
        if(Array->Arena) Array->Items = (T *)ArenaResizeMemory(Array->Arena, Array->Items, OldSize, NewSize);
        else Array->Items = (T *)OSDefaultRealloc(Array->Items, NewSize);
    }
    MoveMemory(&Array->Items[Index+1], 
               &Array->Items[Index], 
               (Array->Count-Index)*sizeof(T));
    Array->Items[Index] = Item;
    Array->Count++;
}

template<typename T> tyler_function inline void
ArrayOrderedRemove(dynamic_array<T> *Array, u32 Index){
    MoveMemory(&Array->Items[Index], 
               &Array->Items[Index+1], 
               (Array->Count-Index)*sizeof(T));
    Array->Count--;
}

template<typename T> tyler_function inline void
ArrayUnorderedRemove(dynamic_array<T> *Array, u32 Index){
    Array->Items[Index] = Array->Items[Array->Count-1];
    Array->Count--;
}

//~ Bucket array


global_constant u32 MAX_BUCKET_ITEMS = 64;
template<typename T, u32 U>
struct bucket_array_bucket {
    static_assert(U <= MAX_BUCKET_ITEMS);
    
    u32 Index;
    u32 Count;
    u64 Occupancy;// TODO(Tyler): This won't work for 32 bit systems, as there isn't
    // a _BitScanForward64 in those systems;
    T Items[U];
};

template<typename T, u32 U>
struct bucket_array {
    memory_arena *Arena;
    u32 Count;
    dynamic_array<bucket_array_bucket<T, U> *> Buckets;
    dynamic_array<bucket_array_bucket<T, U> *> UnfullBuckets;
};

struct bucket_index {
    u32 Bucket;
    u32 Item;
};

template<typename T>
struct bucket_array_iterator {
    T *Item;
    bucket_index Index;
    u32 I;
};

tyler_function inline bucket_index
BucketIndex(u32 Bucket, u32 Item){
    bucket_index Result;
    Result.Bucket = Bucket;
    Result.Item = Item;
    return Result;
}


template<typename T, u32 U>
tyler_function bucket_array_bucket<T, U> *
BucketArrayAllocBucket(bucket_array<T, U> *Array){
    typedef bucket_array_bucket<T, U> this_bucket; 
    // To avoid a comma inside the macro because it doesn't like that bucket<T, U> has a comma
    bucket_array_bucket<T,U> *Result = PushStruct(Array->Arena, this_bucket);
    *Result = {};
    Result->Index = Array->Buckets.Count;
    ArrayAdd(&Array->Buckets, Result);
    ArrayAdd(&Array->UnfullBuckets, Result);
    
    return(Result);
}

template<typename T, u32 U>
tyler_function void
InitializeBucketArray(bucket_array<T, U> *Array, memory_arena *Arena, u32 InitialBuckets=4){
    Assert(Arena);
    
    *Array = {};
    Array->Arena = Arena;
    InitializeArray(&Array->Buckets, InitialBuckets, Array->Arena);
    InitializeArray(&Array->UnfullBuckets, InitialBuckets, Array->Arena);
    bucket_index Index;
    bucket_array_bucket<T, U> *Bucket = BucketArrayAllocBucket(Array);
}

template<typename T, u32 U>
tyler_function T *
BucketArrayAlloc(bucket_array<T, U> *Array){
    T *Result = 0;
    if(Array->UnfullBuckets.Count == 0){
        BucketArrayAllocBucket(Array);
    }
    
    bucket_array_bucket<T, U> *Bucket = Array->UnfullBuckets[0];
    Assert(Bucket->Count < U);
    bit_scan_result BitScan = ScanForLeastSignificantSetBit(~Bucket->Occupancy);
    Assert(BitScan.Found);
    u32 Item = BitScan.Index;
    Result = &Bucket->Items[Item];
    *Result = {};
    Bucket->Occupancy |= (1ULL << Bucket->Count);
    Bucket->Count++;
    Array->Count++;
    
    if(Bucket->Count >= U){ 
        ArrayUnorderedRemove(&Array->UnfullBuckets, 0);
    }
    
    return(Result);
}

template<typename T, u32 U>
tyler_function void
BucketArrayRemove(bucket_array<T, U> *Array, bucket_index Index){
    bucket_array_bucket<T, U> *Bucket = Array->Buckets[Index.Bucket];
    Assert(Bucket->Occupancy & (1ULL << Index.Item));
    
    Array->Count--;
    b8 WasFull = (Bucket->Count == U);
    Bucket->Count--;
    Bucket->Occupancy &= ~(1ULL << Index.Item);
    
    if(WasFull) ArrayAdd(&Array->UnfullBuckets, Bucket);
}

template<typename T, u32 U>
tyler_function void
BucketArrayRemoveAll(bucket_array<T, U> *Array){
    for(u32 I = 0; I < Array->Buckets.Count; I++){
        bucket_array_bucket<T, U> *Bucket = Array->Buckets[I];
        
        b8 WasFull = (Bucket->Count == U);
        Bucket->Count = 0;
        Bucket->Occupancy = 0;
        
        if(WasFull) ArrayAdd(&Array->UnfullBuckets, Bucket);
    }
    Array->Count = 0;
}

template<typename T, u32 U>
tyler_function inline bucket_array_iterator<T>
BucketArrayIteratorFromIndex(bucket_array<T, U> *Array, bucket_index Index){
    bucket_array_iterator<T> Result = {0, Index};
    return(Result);
}

template<typename T, u32 U>
tyler_function inline bucket_array_iterator<T>
BucketArrayBeginIteration(bucket_array<T, U> *Array){
    bucket_array_iterator<T> Result = {};
    while(true){
        bucket_array_bucket<T, U> *Bucket = Array->Buckets[Result.Index.Bucket];
        if(Bucket->Count > 0){
            bit_scan_result BitScan = ScanForLeastSignificantSetBit(Bucket->Occupancy);
            Assert(BitScan.Found);
            Result.Index.Item = BitScan.Index;
            break;
        }else{
            if(Result.Index.Bucket == Array->Buckets.Count-1) break;
            Result.Index.Bucket++;
        }
    }
    Result.Item = BucketArrayGet(Array, Result.Index);
    
    return(Result);
}

template<typename T, u32 U>
tyler_function inline b8
BucketArrayNextIteration(bucket_array<T, U> *Array, bucket_array_iterator<T> *Iterator){
    b8 Result = false;
    
    bucket_array_bucket<T, U> *Bucket = Array->Buckets[Iterator->Index.Bucket];
    b8 FoundNextItem = false;
    for(u32 I = Iterator->Index.Item+1; I < U; I++){
        if(Bucket->Occupancy & (1ULL << I)){
            FoundNextItem = true;
            Result = true;
            Iterator->Index.Item = I;
            break;
        }
    }
    if(!FoundNextItem){
        Iterator->Index.Bucket++;
        while(Iterator->Index.Bucket < Array->Buckets.Count){
            bucket_array_bucket<T, U> *Bucket = Array->Buckets[Iterator->Index.Bucket];
            if(Bucket->Count > 0){
                bit_scan_result BitScan = ScanForLeastSignificantSetBit(Bucket->Occupancy);
                Assert(BitScan.Found);
                Iterator->Index.Item = BitScan.Index;
                Result = true;
                break;
            }else{
                Iterator->Index.Bucket++;
            }
        }
    }
    
    Iterator->I++;
    
    return Result;
}

template<typename T, u32 U>
tyler_function inline b8
BucketArrayContinueIteration(bucket_array<T, U> *Array, bucket_array_iterator<T> *Iterator){
    b8 Result = Iterator->I < Array->Count;
    if(Result){
        Iterator->Item = BucketArrayGet(Array, Iterator->Index);
    }
    
    return(Result);
}

template<typename T, u32 U>
tyler_function inline T *
BucketArrayGet(bucket_array<T, U> *Array, bucket_index Index){
    bucket_array_bucket<T, U> *Bucket = Array->Buckets[Index.Bucket];
    T *Result = 0;
    if(Bucket->Occupancy & (1ULL << Index.Item))
        Result = &Bucket->Items[Index.Item];
    return(Result);
}

#define FOR_BUCKET_ARRAY(Iterator, Array) \
for(auto Iterator = BucketArrayBeginIteration(Array); \
BucketArrayContinueIteration(Array, &Iterator); \
BucketArrayNextIteration(Array, &Iterator))

#define FOR_BUCKET_ARRAY_FROM(Iterator, Array, Initial) \
for(auto Iterator = BucketArrayIteratorFromIndex(Array, Initial); \
BucketArrayContinueIteration(Array, &Iterator); \
BucketArrayNextIteration(Array, &Iterator))

//~ Simple and very naive hash table
// // TODO(Tyler): This hash table implementation could be improved significantly


// NOTE(Tyler): For each key type that is defined for this hash table implementation,
// one must define two tyler_functions for that key type: 'HashKey' AND 'CompareKeys'. 

//- Predefined key type tyler_functions
// NOTE(Tyler): These are basic keys that would be commonly used in a hash table
tyler_function constexpr u64
HashKey(const char *String) {
    u64 Result = HashString(String);
    return(Result);
}

tyler_function constexpr u64
HashKey(u64 Value) {
    u64 Result = Value;
    return(Result);
}

tyler_function constexpr b32
CompareKeys(const char *A, const char *B){
    b32 Result = CompareCStrings(A, B);
    return(Result);
}

tyler_function constexpr b32
CompareKeys(u64 A, u64 B){
    b32 Result = (A == B);
    return(Result);
}

//- Implementation
template <typename KeyType, typename ValueType>
struct hash_table {
    u32 BucketsUsed;
    u32 MaxBuckets;
    u64 *Hashes;
    KeyType *Keys;
    ValueType *Values;
};

template <typename KeyType, typename ValueType>
tyler_function constexpr hash_table<KeyType, ValueType>
MakeHashTable(memory_arena *Arena, u32 MaxBuckets){
    hash_table<KeyType, ValueType> Result = {};
    Result.MaxBuckets = MaxBuckets;
    Result.Hashes = PushArray(Arena, u64, MaxBuckets);
    Result.Keys = PushArray(Arena, KeyType, MaxBuckets);
    Result.Values = PushArray(Arena, ValueType, MaxBuckets);
    ZeroMemory(Result.Hashes, MaxBuckets*sizeof(u64));
    ZeroMemory(Result.Keys, MaxBuckets*sizeof(KeyType));
    ZeroMemory(Result.Values, MaxBuckets*sizeof(ValueType));
    return(Result);
}

template <typename KeyType, typename ValueType>
tyler_function inline void 
HashTableCopy(hash_table<KeyType, ValueType> *OutTable, hash_table<KeyType, ValueType> *InTable){
    Assert(OutTable->MaxBuckets >= InTable->BucketsUsed);
    for(u32 I=0; I<InTable->MaxBuckets; I++){
        if(InTable->Hashes[I]){
            HashTableInsert(OutTable, InTable->Keys[I], InTable->Values[I]);
        }
    }
}

template <typename KeyType, typename ValueType>
tyler_function constexpr void
HashTableInsert(hash_table<KeyType, ValueType> *Table, KeyType Key, ValueType Value){
    //TIMED_FUNCTION();
    Assert(Table->BucketsUsed < Table->MaxBuckets);
    
    u64 Hash = HashKey(Key);
    if(Hash == 0){Hash++;}
    
    u32 Index = Hash % Table->MaxBuckets;
    while(true){
        u64 TestHash = Table->Hashes[Index];
        if(TestHash == Hash){
            if(CompareKeys(Key, Table->Keys[Index])) break;
        }else if(TestHash == 0){
            Table->BucketsUsed++;
            break;
        }
        
        Index++;
        Index %= Table->MaxBuckets;
    }
    
    Table->Hashes[Index] = Hash;
    Table->Keys[Index] = Key;
    Table->Values[Index] = Value;
}

template <typename KeyType, typename ValueType>
tyler_function ValueType *
HashTableAlloc(hash_table<KeyType, ValueType> *Table, KeyType Key){
    //TIMED_FUNCTION();
    Assert(Table->BucketsUsed < Table->MaxBuckets);
    
    u64 Hash = HashKey(Key);
    if(Hash == 0){Hash++;}
    
    u32 Index = Hash % Table->MaxBuckets;
    while(true){
        u64 TestHash = Table->Hashes[Index];
        if(TestHash == Hash){
            if(CompareKeys(Key, Table->Keys[Index])) break;
        }else if(TestHash == 0){
            Table->BucketsUsed++;
            break;
        }else{
            Index++;
            Index %= Table->MaxBuckets;
        }
    }
    
    Table->Hashes[Index] = Hash;
    Table->Keys[Index] = Key;
    ValueType *Result = &Table->Values[Index];
    return(Result);
}

template <typename KeyType, typename ValueType>
tyler_function constexpr ValueType *
HashTableFindPtr(hash_table<KeyType, ValueType> *Table, KeyType Key){
    u64 Hash = HashKey(Key);
    if(Hash == 0) Hash++; 
    
    b8 IsValid = true;
    b8 FirstIteration = true;
    u32 Index = Hash % Table->MaxBuckets;
    u32 StartIndex = Index;
    while(true){
        u64 TestHash = Table->Hashes[Index];
        if((TestHash == Hash) &&
           CompareKeys(Key, Table->Keys[Index])){
            break;
        }else if(TestHash == 0){
            IsValid = false;
            break;
        }else if(!FirstIteration &&
                 (StartIndex == Index)){
            IsValid = false;
            break;
        }else{
            Index++;
            Index %= Table->MaxBuckets;
        }
        FirstIteration = false;
    }
    
    ValueType *Result = 0;
    if(IsValid) Result = &Table->Values[Index];
    return(Result);
}

template <typename KeyType, typename ValueType>
tyler_function constexpr ValueType
HashTableFind(hash_table<KeyType, ValueType> *Table, KeyType Key, b8 *Found=0){
    ValueType *ResultPtr = HashTableFindPtr(Table, Key);
    ValueType Result = {};
    if(ResultPtr){
        Result = *ResultPtr;
        if(Found) *Found = true;
    }
    return(Result);
}

template <typename KeyType, typename ValueType>
tyler_function constexpr ValueType
HashTableGet(hash_table<KeyType, ValueType> *Table, KeyType Key){
    Assert(Table->BucketsUsed < Table->MaxBuckets);
    
    u64 Hash = HashKey(Key);
    if(Hash == 0) Hash++; 
    
    u32 Index = Hash % Table->MaxBuckets;
    while(true){
        u64 TestHash = Table->Hashes[Index];
        if((TestHash == Hash) &&
           CompareKeys(Key, Table->Keys[Index])){
            break;
        }else if(TestHash == 0){
            Table->BucketsUsed++;
            DoesExist = false;
            Table->Hashes[Index] = Hash;
            Table->Keys[Index] = Key;
            break;
        }else{
            Index++;
            Index %= Table->MaxBuckets;
        }
    }
    
    ValueType Result = Table->Values[Index];
    return(Result);
}


// TODO(Tyler): This could be way more efficient
template <typename KeyType, typename ValueType>
tyler_function constexpr ValueType *
HashTableGetPtr(hash_table<KeyType, ValueType> *Table, KeyType Key){
    Assert(Table->BucketsUsed < Table->MaxBuckets);
    
    u64 Hash = HashKey(Key);
    if(Hash == 0) Hash++; 
    
    b8 FirstIteration = true;
    u32 Index = Hash % Table->MaxBuckets;
    u32 StartIndex = Index;
    while(true){
        u64 TestHash = Table->Hashes[Index];
        if((TestHash == Hash) &&
           CompareKeys(Key, Table->Keys[Index])){
            break;
        }else if(TestHash == 0){
            Table->BucketsUsed++;
            Table->Hashes[Index] = Hash;
            Table->Keys[Index] = Key;
            break;
        }else if(!FirstIteration &&
                 (StartIndex == Index)){
            INVALID_CODE_PATH;
        }else{
            Index++;
            Index %= Table->MaxBuckets;
        }
        FirstIteration = false;
    }
    
    ValueType *Result = 0;
    Result = &Table->Values[Index];
    return(Result);
}

template <typename KeyType, typename ValueType>
tyler_function constexpr b8
HashTableRemove(hash_table<KeyType, ValueType> *Table, KeyType Key){
    u64 Hash = HashKey(Key);
    if(Hash == 0){ Hash++; }
    
    u32 Index = Hash % Table->MaxBuckets;
    if(Index == 0){ Index++; }
    while(u64 TestHash = Table->Hashes[Index]) {
        if(Index == 0){ Index++; }
        if((TestHash == Hash) &&
           CompareKeys(Key, Table->Keys[Index])){
            break;
        }else if(TestHash == 0){
            break;
        }else{
            Index++;
            Index %= Table->MaxBuckets;
        }
    }
    
    b8 Result = false;
    if(Index != 0){
        Table->BucketsUsed--;
        Table->Hashes[Index] = 0; 
        Table->Keys[Index] = {}; 
        Table->Values[Index] = {}; 
        Result = true;
    }
    
    return(Result);
}

#endif //TYLER_BASICS_IMPLEMENTATION

#endif //TYLER_BASICS_H
