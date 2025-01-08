#ifndef LUSTRE_LIB_H
#define LUSTRE_LIB_H

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

#define DECL_FUNC0(name)               \
    typedef struct Lib__##name##_out { \
        float x;                       \
    } Lib__##name##_out;               \
    void Lib__##name##_step(float x, Lib__##name##_out *out);

#define DECL_FUNC1(name)               \
    typedef struct Lib__##name##_out { \
        float x;                       \
    } Lib__##name##_out;               \
    void Lib__##name##_step(float x, float y, Lib__##name##_out *out);

#define DECL_FUNC2(name)               \
    typedef struct Lib__##name##_out { \
        float x;                       \
    } Lib__##name##_out;               \
    void Lib__##name##_step(float x, float y, float z, Lib__##name##_out *out);

DECL_FUNC0(abs)
DECL_FUNC0(sqrt)
DECL_FUNC1(min)
DECL_FUNC1(max)
DECL_FUNC2(clamp)
DECL_FUNC1(mod)

DECL_FUNC0(deg2rad)
DECL_FUNC0(rad2deg)
DECL_FUNC0(sin)
DECL_FUNC0(cos)
DECL_FUNC0(tan)
DECL_FUNC0(asin)
DECL_FUNC0(acos)
DECL_FUNC0(atan)

#ifdef __cplusplus
}
#endif// __cplusplus

#endif// LUSTRE_LIB_H
