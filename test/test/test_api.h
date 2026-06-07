#undef UFBXWT_TEST_GROUP
#define UFBXWT_TEST_GROUP "api"

UFBXWT_TEST(set_connection_replace_null)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	{
		ufbxw_node parent = ufbxwt_create_node(scene, "Parent");
		ufbxw_node child = ufbxwt_create_node(scene, "Child");

		ufbxw_node_set_parent(scene, child, parent);
		ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == parent.id);

		ufbxw_node_set_parent(scene, child, ufbxw_null_node);
		ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == ufbxw_null_id);
		ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 0);
	}

	{
		ufbxw_cache_deformer deformer = ufbxw_create_cache_deformer(scene, ufbxw_null_mesh);
		ufbxw_cache_file cache_a = ufbxw_create_cache_file(scene);
		ufbxw_cache_file cache_b = ufbxw_create_cache_file(scene);

		ufbxw_cache_deformer_set_cache_file(scene, deformer, cache_a);
		ufbxwt_assert(ufbxw_cache_deformer_get_cache_file(scene, deformer).id == cache_a.id);

		ufbxw_cache_deformer_set_cache_file(scene, deformer, cache_b);
		ufbxwt_assert(ufbxw_cache_deformer_get_cache_file(scene, deformer).id == cache_b.id);

		ufbxw_cache_deformer_set_cache_file(scene, deformer, ufbxw_null_cache_file);
		ufbxwt_assert(ufbxw_cache_deformer_get_cache_file(scene, deformer).id == ufbxw_null_id);
	}

	{
		ufbxw_material material = ufbxw_create_material(scene, UFBXW_MATERIAL_CUSTOM);
		ufbxw_implementation implementation_a = ufbxw_create_implementation(scene);
		ufbxw_implementation implementation_b = ufbxw_create_implementation(scene);

		ufbxw_material_set_implementation(scene, material, implementation_a);
		ufbxwt_assert(ufbxw_material_get_implementation(scene, material).id == implementation_a.id);

		ufbxw_material_set_implementation(scene, material, implementation_b);
		ufbxwt_assert(ufbxw_material_get_implementation(scene, material).id == implementation_b.id);

		ufbxw_material_set_implementation(scene, material, ufbxw_null_implementation);
		ufbxwt_assert(ufbxw_material_get_implementation(scene, material).id == ufbxw_null_id);
	}

	{
		ufbxw_implementation implementation = ufbxw_create_implementation(scene);
		ufbxw_binding_table table_a = ufbxw_create_binding_table(scene);
		ufbxw_binding_table table_b = ufbxw_create_binding_table(scene);

		ufbxw_implementation_set_binding_table(scene, implementation, table_a);
		ufbxwt_assert(ufbxw_implementation_get_binding_table(scene, implementation).id == table_a.id);

		ufbxw_implementation_set_binding_table(scene, implementation, table_b);
		ufbxwt_assert(ufbxw_implementation_get_binding_table(scene, implementation).id == table_b.id);

		ufbxw_implementation_set_binding_table(scene, implementation, ufbxw_null_binding_table);
		ufbxwt_assert(ufbxw_implementation_get_binding_table(scene, implementation).id == ufbxw_null_id);
	}

	{
		ufbxw_texture texture = ufbxw_create_texture(scene, UFBXW_TEXTURE_FILE);
		ufbxw_video video_a = ufbxw_create_video(scene);
		ufbxw_video video_b = ufbxw_create_video(scene);

		ufbxw_texture_set_video(scene, texture, video_a);
		ufbxwt_assert(ufbxw_texture_get_video(scene, texture).id == video_a.id);

		ufbxw_texture_set_video(scene, texture, video_b);
		ufbxwt_assert(ufbxw_texture_get_video(scene, texture).id == video_b.id);

		ufbxw_texture_set_video(scene, texture, ufbxw_null_video);
		ufbxwt_assert(ufbxw_texture_get_video(scene, texture).id == ufbxw_null_id);
	}

	ufbxw_free_scene(scene);
}
#endif

#if UFBXWT_IMPL
typedef struct {
	size_t num_calls;
	size_t size;
	bool fail;
} ufbxwt_result_alloc_state;

static void *ufbxwt_result_alloc(void *user, size_t size)
{
	ufbxwt_result_alloc_state *state = (ufbxwt_result_alloc_state*)user;
	state->num_calls++;
	state->size = size;
	if (state->fail) {
		return NULL;
	}
	return malloc(size);
}
#endif

UFBXWT_TEST(write_to_memory)
#if UFBXWT_IMPL
{
	ufbxw_write_buffer buffer;

	{
		ufbxw_scene *scene = ufbxw_create_scene(NULL);
		ufbxwt_assert(scene);

		ufbxwt_create_node(scene, "Test");
		ufbxwt_check_error(scene);

		ufbxw_error error;
		bool ok = ufbxw_save_memory(scene, &buffer, NULL, &error);
		if (!ok) {
			ufbxwt_log_error(&error);
		}
		ufbxwt_assert(ok);

		ufbxw_free_scene(scene);
	}

	{
		ufbx_scene *scene = ufbx_load_memory(buffer.data, buffer.size, NULL, NULL);
		ufbxwt_assert(scene);

		ufbx_node *node = ufbx_find_node(scene, "Test");
		ufbxwt_assert(node);

		ufbx_free_scene(scene);
	}

	ufbxw_free_write_buffer(buffer);
}
#endif

UFBXWT_TEST(write_to_memory_allocator)
#if UFBXWT_IMPL
{
	ufbxw_write_buffer buffer;
	ufbxwt_result_alloc_state state = { 0 };
	ufbxw_save_opts opts = { 0 };
	opts.result_alloc_cb.fn = &ufbxwt_result_alloc;
	opts.result_alloc_cb.user = &state;

	{
		ufbxw_scene *scene = ufbxw_create_scene(NULL);
		ufbxwt_assert(scene);

		ufbxwt_create_node(scene, "Test");
		ufbxwt_check_error(scene);

		ufbxw_error error;
		bool ok = ufbxw_save_memory(scene, &buffer, &opts, &error);
		if (!ok) {
			ufbxwt_log_error(&error);
		}
		ufbxwt_assert(ok);
		ufbxwt_assert(state.num_calls == 1);
		ufbxwt_assert(state.size == buffer.size);

		ufbxw_free_scene(scene);
	}

	{
		ufbx_scene *scene = ufbx_load_memory(buffer.data, buffer.size, NULL, NULL);
		ufbxwt_assert(scene);

		ufbx_node *node = ufbx_find_node(scene, "Test");
		ufbxwt_assert(node);

		ufbx_free_scene(scene);
	}

	free(buffer.data);
}
#endif

UFBXWT_TEST(write_to_memory_allocator_fail)
#if UFBXWT_IMPL
{
	ufbxw_write_buffer buffer = { 0 };

	ufbxwt_result_alloc_state state = { 0 };
	state.fail = true;

	ufbxw_save_opts opts = { 0 };
	opts.result_alloc_cb.fn = &ufbxwt_result_alloc;
	opts.result_alloc_cb.user = &state;

	{
		ufbxw_scene *scene = ufbxw_create_scene(NULL);
		ufbxwt_assert(scene);

		ufbxwt_create_node(scene, "Test");
		ufbxwt_check_error(scene);

		ufbxw_error error;
		bool ok = ufbxw_save_memory(scene, &buffer, &opts, &error);
		ufbxwt_assert(!ok);
		ufbxwt_assert_error(&error, UFBXW_ERROR_STREAM_BEGIN, "ufbxwi_save_imp", "failed to begin output stream");
		ufbxwt_assert(state.num_calls == 1);
		ufbxwt_assert(state.size > 0);
		ufbxwt_assert(buffer.data == NULL);
		ufbxwt_assert(buffer.size == 0);

		ufbxw_free_scene(scene);
	}
}
#endif
