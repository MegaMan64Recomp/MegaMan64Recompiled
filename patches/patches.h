#ifndef __PATCHES_H__
#define __PATCHES_H__

#define RECOMP_EXPORT __attribute__((section(".recomp_export")))
#define RECOMP_PATCH __attribute__((section(".recomp_patch")))
#define RECOMP_FORCE_PATCH __attribute__((section(".recomp_force_patch")))
#define RECOMP_DECLARE_EVENT(func) \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
__attribute__((noinline, weak, used, section(".recomp_event"))) void func {} \
_Pragma("GCC diagnostic pop")

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320

#define osCreateMesgQueue osCreateMesgQueue_recomp
#define osRecvMesg osRecvMesg_recomp
#define osSendMesg osSendMesg_recomp
#define osViGetCurrentFramebuffer osViGetCurrentFramebuffer_recomp
#define osFlashWriteArray osFlashWriteArray_recomp
#define osFlashWriteBuffer osFlashWriteBuffer_recomp
#define osWritebackDCache osWritebackDCache_recomp
#define osInvalICache osInvalICache_recomp
#define osGetTime osGetTime_recomp

#define osContStartReadData osContStartReadData_recomp
#define osContGetReadData osContGetReadData_recomp
#define osContStartQuery osContStartQuery_recomp
#define osContGetQuery osContGetQuery_recomp

#define memcpy memcpy_recomp
#define osVirtualToPhysical osVirtualToPhysical_recomp
#define sinf __sinf_recomp
#define cosf __cosf_recomp
#define bzero bzero_recomp
#define gRandFloat sRandFloat
#include "PR/ultratypes.h"
#include "rt64_extended_gbi.h"
#include "patch_helpers.h"

#define gEXMatrixGroupSkipAll(cmd, id, push, proj, edit) \
    gEXMatrixGroup(cmd, id, G_EX_INTERPOLATE_SIMPLE, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, edit, G_EX_ASPECT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupNoInterpolation(cmd, push, proj, edit) \
    gEXMatrixGroup(cmd, G_EX_ID_IGNORE, G_EX_INTERPOLATE_SIMPLE, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupInterpolateOnlyTiles(cmd, push, proj, edit) \
    gEXMatrixGroup(cmd, G_EX_ID_IGNORE, G_EX_INTERPOLATE_SIMPLE, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedNormal(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedSkipRot(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedSkipPosRot(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedSkipAll(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedVerts(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

#define gEXMatrixGroupDecomposedVertsOrderAuto(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, edit)

void memcpy(void * dst, void *src, int size);
int recomp_printf(const char* fmt, ...);
void Update_UI_Alignments();

// #define INCBIN(identifier, filename)          \
//     asm(".pushsection .rodata\n"              \
//         "\t.local " #identifier "\n"          \
//         "\t.type " #identifier ", @object\n"  \
//         "\t.balign 8\n"                       \
//         #identifier ":\n"                     \
//         "\t.incbin \"" filename "\"\n\n"      \
//                                               \
//         "\t.balign 8\n"                       \
//         "\t.popsection\n");                   \
//     extern u8 identifier[]

typedef int bool;
void recomp_crash(const char* err);
void set_all_interpolation_skipped(bool skipped);
bool all_interpolation_skipped();
void recomp_interp(u32 id);
void recomp_push_viewport();
void recomp_pop_viewport();

typedef unsigned long collection_key_t;
typedef unsigned long U32ValueHashmapHandle;
U32ValueHashmapHandle recomputil_create_u32_value_hashmap();
int recomputil_u32_value_hashmap_insert(U32ValueHashmapHandle, collection_key_t, unsigned long);
int recomputil_u32_value_hashmap_get(U32ValueHashmapHandle, collection_key_t, unsigned long*);
int recomputil_u32_value_hashmap_erase(U32ValueHashmapHandle, collection_key_t);

DECLARE_FUNC(U32ValueHashmapHandle, recomputil_create_u32_value_hashmap, void);
DECLARE_FUNC(void, recomputil_destroy_u32_value_hashmap, U32ValueHashmapHandle handle);
DECLARE_FUNC(int, recomputil_u32_value_hashmap_contains, U32ValueHashmapHandle handle, collection_key_t key);
DECLARE_FUNC(int, recomputil_u32_value_hashmap_insert, U32ValueHashmapHandle handle, collection_key_t key, unsigned long value);
DECLARE_FUNC(int, recomputil_u32_value_hashmap_get, U32ValueHashmapHandle handle, collection_key_t key, unsigned long* out);
DECLARE_FUNC(int, recomputil_u32_value_hashmap_erase, U32ValueHashmapHandle handle, collection_key_t key);
DECLARE_FUNC(unsigned long, recomputil_u32_value_hashmap_size, U32ValueHashmapHandle handle);

typedef unsigned long U32MemoryHashmapHandle;

// DECLARE_FUNC(U32MemoryHashmapHandle, recomputil_create_u32_memory_hashmap, unsigned long element_size);
// DECLARE_FUNC(void, recomputil_destroy_u32_memory_hashmap, U32MemoryHashmapHandle handle);
// DECLARE_FUNC(int, recomputil_u32_memory_hashmap_contains, U32MemoryHashmapHandle handle, collection_key_t key);
// DECLARE_FUNC(int, recomputil_u32_memory_hashmap_create, U32MemoryHashmapHandle handle, collection_key_t key);
// DECLARE_FUNC(void*, recomputil_u32_memory_hashmap_get, U32MemoryHashmapHandle handle, collection_key_t key);
// DECLARE_FUNC(int, recomputil_u32_memory_hashmap_erase, U32MemoryHashmapHandle handle, collection_key_t key);
// DECLARE_FUNC(unsigned long, recomputil_u32_memory_hashmap_size, U32MemoryHashmapHandle handle);

#endif
