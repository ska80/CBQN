#include "../core.h"

B m_i8(i8 x) { return m_i32(x); } B m_i16(i16 x) { return m_i32(x); }
B m_c8(u8 x) { return m_c32(x); } B m_c16(u16 x) { return m_c32(x); }
#define TU I8
#define TP(W,X) W##i8##X
#include "tyarrTemplate.c"
#define TU I16
#define TP(W,X) W##i16##X
#include "tyarrTemplate.c"
#define TU I32
#define TP(W,X) W##i32##X
#include "tyarrTemplate.c"
#define TU C8
#define TP(W,X) W##c8##X
#include "tyarrTemplate.c"
#define TU C16
#define TP(W,X) W##c16##X
#include "tyarrTemplate.c"
#define TU C32
#define TP(W,X) W##c32##X
#include "tyarrTemplate.c"
#define TU F64
#define TP(W,X) W##f64##X
#include "tyarrTemplate.c"

NOINLINE B m_caf64(usz sz, f64* a) { f64* rp; B r = m_f64arrv(&rp, sz); for (usz i = 0; i < sz; i++) rp[i] = a[i]; return r; }
NOINLINE B m_cai32(usz sz, i32* a) { i32* rp; B r = m_i32arrv(&rp, sz); for (usz i = 0; i < sz; i++) rp[i] = a[i]; return r; }
NOINLINE B m_str8(usz sz, char* s) { u8*  rp; B r = m_c8arrv (&rp, sz); for (u64 i = 0; i < sz; i++) rp[i] = s[i]; return r; }
NOINLINE B m_str8l(char* s) { return m_str8(strlen(s), s); }
NOINLINE B m_str32(u32* s) {
  usz sz = 0; while(s[sz]) sz++;
  u32* rp; B r = m_c32arrv(&rp, sz);
  for (usz i = 0; i < sz; i++) rp[i] = s[i];
  return r;
}

#define MAKE_ICPY(T,E) T##Arr* cpy##T##Arr(B x) { \
  usz ia = a(x)->ia;     \
  E* rp; Arr* r = m_##E##arrp(&rp, ia); \
  arr_shCopy(r, x);      \
  u8 xe = TI(x,elType);  \
  if      (xe==el_i8 ) { i8*  xp = i8any_ptr (x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else if (xe==el_i16) { i16* xp = i16any_ptr(x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else if (xe==el_i32) { i32* xp = i32any_ptr(x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else if (xe==el_f64) { f64* xp = f64any_ptr(x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else {                 \
    B* xp = arr_bptr(x); \
    if (xp!=NULL) { for (usz i=0; i<ia; i++) rp[i]=o2fu(xp[i]    ); } \
    else { SGetU(x) for (usz i=0; i<ia; i++) rp[i]=o2fu(GetU(x,i)); } \
  }                      \
  dec(x);                \
  return (T##Arr*)r;     \
}

MAKE_ICPY(I8, i8)
MAKE_ICPY(I16, i16)
MAKE_ICPY(I32, i32)
MAKE_ICPY(F64, f64)
#undef MAKE_ICPY

#define MAKE_CCPY(T,E)     \
T##Arr* cpy##T##Arr(B x) { \
  usz ia = a(x)->ia;       \
  T##Atom* rp; Arr* r = m_##E##arrp(&rp, ia); \
  arr_shCopy(r, x);        \
  u8 xe = TI(x,elType);    \
  if      (xe==el_c8 ) { u8*  xp = c8any_ptr (x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else if (xe==el_c16) { u16* xp = c16any_ptr(x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else if (xe==el_c32) { u32* xp = c32any_ptr(x); for(usz i=0; i<ia; i++) rp[i]=xp[i]; } \
  else {                   \
    B* xp = arr_bptr(x);   \
    if (xp!=NULL) { for (usz i=0; i<ia; i++) rp[i]=o2cu(xp[i]    ); } \
    else { SGetU(x) for (usz i=0; i<ia; i++) rp[i]=o2cu(GetU(x,i)); } \
  }                        \
  dec(x);                  \
  return (T##Arr*)r;       \
}

MAKE_CCPY(C8, c8)
MAKE_CCPY(C16, c16)
MAKE_CCPY(C32, c32)
#undef MAKE_CCPY

void tyarr_init() {
  i8arr_init(); i16arr_init(); i32arr_init();
  c8arr_init(); c16arr_init(); c32arr_init(); f64arr_init();
  
  { i8* tmp; bi_emptyIVec = m_i8arrv(&tmp, 0); gc_add(bi_emptyIVec); }
  { u8* tmp; bi_emptyCVec = m_c8arrv(&tmp, 0); gc_add(bi_emptyCVec); }
  
  Arr* emptySVec = m_fillarrp(0); arr_shVec(emptySVec);
  fillarr_setFill(emptySVec, emptyCVec());
  bi_emptySVec = taga(emptySVec); gc_add(bi_emptySVec);
}