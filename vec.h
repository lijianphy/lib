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
    /* Reserve vector data */                                                 \
    static inline int vtype##_reserve(vtype *v, size_t capacity)              \
    {                                                                         \
        dtype *new_data;                                                      \
        new_data = (dtype *)realloc(v->data, sizeof(dtype) * capacity);       \
        if (!new_data)                                                        \
            return -1;                                                        \
        v->data = new_data;                                                   \
        v->capacity = capacity;                                               \
        return 0;                                                             \
    }                                                                         \
                                                                              \
    /* Push element to vector */                                              \
    static inline int vtype##_push(vtype *v, dtype x)                         \
    {                                                                         \
        if (v->size == v->capacity)                                           \
        {                                                                     \
            size_t new_capacity = v->capacity ? v->capacity << 1 : 2;         \
            if (vtype##_reserve(v, new_capacity) != 0)                        \
                return -1;                                                    \
        }                                                                     \
        v->data[v->size++] = x;                                               \
        return 0;                                                             \
    }                                                                         \
                                                                              \
    /* Copy vector */                                                         \
    static inline int vtype##_copy(vtype *restrict dst, vtype *restrict src)  \
    {                                                                         \
        if (dst->capacity < src->size)                                        \
        {                                                                     \
            if (vtype##_reserve(dst, src->size) != 0)                         \
                return -1;                                                    \
        }                                                                     \
        dst->size = src->size;                                                \
        memcpy(dst->data, src->data, sizeof(dtype) * src->size);              \
        return 0;                                                             \
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
