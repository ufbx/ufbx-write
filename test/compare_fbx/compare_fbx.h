#pragma once

#include <stdbool.h>

typedef struct compare_fbx_opts {
	double approx_epsilon;
	bool compare_anim;
	bool evaluate_scene_anim;
	bool compare_save_info;
} compare_fbx_opts;

typedef struct compare_fbx_input {
	const char *file_path;

	const void *memory_data;
	size_t memory_size;
} compare_fbx_input;

#if defined(__cplusplus)
extern "C" {
#endif

bool compare_fbx(compare_fbx_input input, const char *ref_path, const compare_fbx_opts *opts);

#if defined(__cplusplus)
}
#endif
