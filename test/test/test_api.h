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
