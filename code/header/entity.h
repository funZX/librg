// file: header/entity.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// typedef enum librg_entity_refreshing {
//     LIBRG_TICK,     /* int argument, update entity every Nth tick */
//     LIBRG_LINEAR,   /* float argument, linearly decrease update interval based on distance step (argument) */
//     LIBRG_CUBIC,    /* float argument, cubicly decrease update inverval based on distance step (argument) */
// } librg_entity_refreshing;

typedef enum {
    LIBRG_ENTITY_DYNAMIC,
    LIBRG_ENTITY_STATIC,
} librg_behavior_type;

typedef enum {
    LIBRG_BEHAVIOR_TYPE,
    LIBRG_BEHAVIOR_STEPBACK,
} librg_behavior;

typedef enum {
    LIBRG_VISIBLITY_DEFAULT,
    LIBRG_VISIBLITY_NEVER,
    LIBRG_VISIBLITY_ALWAYS,
} librg_visibility;

// =======================================================================//
// !
// ! Basic entity manipulation
// !
// =======================================================================//

LIBRG_API int8_t  librg_entity_track(librg_world *, int64_t entity_id);
LIBRG_API int8_t  librg_entity_untrack(librg_world *, int64_t entity_id);
LIBRG_API int8_t  librg_entity_tracked(librg_world *, int64_t entity_id);
LIBRG_API int8_t  librg_entity_foreign(librg_world *, int64_t entity_id);
LIBRG_API int8_t  librg_entity_owned(librg_world *, int64_t entity_id);
LIBRG_API int32_t librg_entity_count(librg_world *);

// =======================================================================//
// !
// ! Entity data methods
// !
// =======================================================================//

LIBRG_API int8_t        librg_entity_chunk_set(librg_world *, int64_t entity_id, librg_chunk);
LIBRG_API librg_chunk   librg_entity_chunk_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_owner_set(librg_world *, int64_t entity_id, int64_t owner_id);
LIBRG_API int64_t       librg_entity_owner_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_radius_set(librg_world *, int64_t entity_id, int8_t observed_chunk_radius);
LIBRG_API int8_t        librg_entity_radius_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_dimension_set(librg_world *, int64_t entity_id, int32_t dimension);
LIBRG_API int32_t       librg_entity_dimension_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_userdata_set(librg_world *, int64_t entity_id, void *data);
LIBRG_API void *        librg_entity_userdata_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_chunkarray_set(librg_world *, int64_t entity_id, const librg_chunk *chunks, size_t chunk_amount);
LIBRG_API size_t        librg_entity_chunkarray_get(librg_world *, int64_t entity_id, LIBRG_OUT librg_chunk *chunks, size_t buffer_limit);
LIBRG_API int8_t        librg_entity_visibility_global_set(librg_world *, int64_t entity_id, librg_visibility value);
LIBRG_API int8_t        librg_entity_visibility_global_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_visibility_owner_set(librg_world *, int64_t entity_id, int64_t owner_id, librg_visibility value);
LIBRG_API int8_t        librg_entity_visibility_owner_get(librg_world *, int64_t entity_id, int64_t owner_id);

#if 0 /* for future releases */
LIBRG_API int8_t        librg_entity_behavior_set(librg_world *, int64_t entity_id, librg_behavior key, int32_t value);
LIBRG_API int32_t       librg_entity_behavior_get(librg_world *, int64_t entity_id, librg_behavior key);
#endif

LIBRG_END_C_DECLS
