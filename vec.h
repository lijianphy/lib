#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>
#include <string.h>

/**
 * @brief Define vector implementation for a given data type.
 * @param dtype Data type [type].
 * @param vtype Vector type [symbol].
 */
#define VEC_IMPL(dtype, vtype)                                                \
    typedef struct                                                            \
    {                                                                         \
        size_t size;     /* current number of elements */                     \
        size_t capacity; /* allocated capacity */                             \
        dtype *data;     /* array pointer */                                  \
    } vtype;                                                                  \
                                                                              \
    /* Initialize vector */                                                   \
    static inline void vtype##_init(vtype *v)                                 \
    {                                                                         \
        memset(v, 0, sizeof(vtype));                                          \
    }                                                                         \
                                                                              \
    /* Free vector memory */                                                  \
    static inline void vtype##_destroy(vtype *v)                              \
    {                                                                         \
        free(v->data);                                                        \
        memset(v, 0, sizeof(vtype));                                          \
    }                                                                         \
                                                                              \
    /* Get element at index */                                                \
    static inline dtype vtype##_get(vtype *v, size_t i)                       \
    {                                                                         \
        return v->data[i];                                                    \
    }                                                                         \
                                                                              \
    /* Set element at index */                                                \
    static inline void vtype##_set(vtype *v, size_t i, dtype value)           \
    {                                                                         \
        v->data[i] = value;                                                   \
    }                                                                         \
                                                                              \
    /* Get current size */                                                    \
    static inline size_t vtype##_size(vtype *v)                               \
    {                                                                         \
        return v->size;                                                       \
    }                                                                         \
                                                                              \
    /* Remove and return last element */                                      \
    static inline dtype vtype##_pop(vtype *v)                                 \
    {                                                                         \
        return v->data[--(v->size)];                                          \
    }                                                                         \
                                                                              \
    /* Resize vector capacity */                                              \
    static inline void vtype##_resize(vtype *v, size_t size)                  \
    {                                                                         \
        v->capacity = size;                                                   \
        v->data = (dtype *)realloc(v->data, sizeof(dtype) * v->capacity);     \
    }                                                                         \
                                                                              \
    /* Push element to vector */                                              \
    static inline void vtype##_push(vtype *v, dtype x)                        \
    {                                                                         \
        if (v->size == v->capacity)                                           \
        {                                                                     \
            v->capacity = v->capacity ? v->capacity << 1 : 2;                 \
            v->data = (dtype *)realloc(v->data, sizeof(dtype) * v->capacity); \
        }                                                                     \
        v->data[v->size++] = x;                                               \
    }                                                                         \
                                                                              \
    /* Copy vector */                                                         \
    static inline void vtype##_copy(vtype *restrict dst, vtype *restrict src) \
    {                                                                         \
        if (dst->capacity < src->size)                                        \
        {                                                                     \
            vtype##_resize(dst, src->size);                                   \
        }                                                                     \
        dst->size = src->size;                                                \
        memcpy(dst->data, src->data, sizeof(dtype) * src->size);              \
    }                                                                         \
                                                                              \
    /* Move vector */                                                         \
    static inline void vtype##_move(vtype *restrict dst, vtype *restrict src) \
    {                                                                         \
        free(dst->data);                                                      \
        memcpy(dst, src, sizeof(vtype));                                      \
        memset(src, 0, sizeof(vtype));                                        \
    }

#endif // VEC_H_
