#ifndef LUSTRE_LIB_H
#define LUSTRE_LIB_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Lib__foo_out {
    float x;
    float y;
} Lib__foo_out;

void Lib__foo_step(float x, int y, Lib__foo_out* out);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LUSTRE_LIB_H
