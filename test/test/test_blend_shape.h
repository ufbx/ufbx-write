#undef UFBXWT_TEST_GROUP
#define UFBXWT_TEST_GROUP "blend_shape"

UFBXWT_SCENE_TEST(shape_plane)
#if UFBXWT_IMPL
{
	ufbxw_node node = ufbxwt_create_node(scene, "Node");
	ufbxw_mesh mesh = ufbxw_create_mesh(scene);

	ufbxw_mesh_add_instance(scene, mesh, node);

	ufbxw_vec3 vertices[] = {
		{ -1.0f, 0.0f, -1.0f },
		{ +1.0f, 0.0f, -1.0f },
		{ -1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, +1.0f },
	};
	int32_t indices[] = {
		0, 2, 3, 1,
	};
	int32_t face_offsets[] = {
		0, 4,
	};

	ufbxw_vec3_buffer vertex_buffer = ufbxw_view_vec3_array(scene, vertices, ufbxwt_arraycount(vertices));
	ufbxw_int_buffer index_buffer = ufbxw_view_int_array(scene, indices, ufbxwt_arraycount(indices));
	ufbxw_int_buffer face_buffer = ufbxw_view_int_array(scene, face_offsets, ufbxwt_arraycount(face_offsets));

	ufbxw_mesh_set_vertices(scene, mesh, vertex_buffer);
	ufbxw_mesh_set_polygons(scene, mesh, index_buffer, face_buffer);

	ufbxw_blend_deformer deformer = ufbxw_create_blend_deformer(scene, mesh);
	ufbxw_blend_channel left_channel = ufbxw_create_blend_channel(scene, deformer);
	ufbxw_blend_channel right_channel = ufbxw_create_blend_channel(scene, deformer);

	ufbxw_set_name(scene, left_channel.id, "Left");
	ufbxw_set_name(scene, right_channel.id, "Right");

	ufbxw_blend_shape left_shape = ufbxw_create_blend_shape(scene);
	ufbxw_blend_shape right_shape = ufbxw_create_blend_shape(scene);

	ufbxw_set_name(scene, left_shape.id, "Left");
	ufbxw_set_name(scene, right_shape.id, "Right");

	int32_t left_indices[] = { 0, 2 };
	ufbxw_vec3 left_offsets[] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
	};

	int32_t right_indices[] = { 1, 3 };
	ufbxw_vec3 right_offsets[] = {
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
	};

	ufbxw_blend_shape_set_offsets(scene, left_shape,
		ufbxw_view_int_array(scene, left_indices, ufbxwt_arraycount(left_indices)),
		ufbxw_view_vec3_array(scene, left_offsets, ufbxwt_arraycount(left_offsets)));

	ufbxw_blend_shape_set_offsets(scene, right_shape,
		ufbxw_view_int_array(scene, right_indices, ufbxwt_arraycount(right_indices)),
		ufbxw_view_vec3_array(scene, right_offsets, ufbxwt_arraycount(right_offsets)));

	ufbxw_blend_channel_set_shape(scene, left_channel, left_shape);
	ufbxw_blend_channel_set_shape(scene, right_channel, right_shape);

	ufbxw_blend_channel_set_weight(scene, left_channel, 100.0);
}
#endif

UFBXWT_SCENE_CHECK(shape_plane)
#if UFBXWT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Node");
	ufbxwt_assert(node);

	ufbx_mesh *mesh = node->mesh;
	ufbxwt_assert(mesh);

	ufbxwt_assert(mesh->faces.count == 1);

	ufbx_face face = mesh->faces.data[0];
	ufbxwt_assert(face.index_begin == 0);
	ufbxwt_assert(face.num_indices == 4);

	ufbx_vec3 vertices[] = {
		{ -1.0f, 0.0f, -1.0f },
		{ -1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, -1.0f },
	};

	for (size_t i = 0; i < 4; i++) {
		ufbx_vec3 v = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbxwt_assert_close_uvec3(err, v, vertices[i]);
	}
}
#endif

UFBXWT_SCENE_TEST_FLAGS(blender_full_weights, UFBXWT_SCENE_TEST_FLAG_BLENDER_CREATOR)
#if UFBXWT_IMPL
{
	ufbxw_node node = ufbxwt_create_node(scene, "Node");
	ufbxw_mesh mesh = ufbxw_create_mesh(scene);

	ufbxw_mesh_add_instance(scene, mesh, node);

	ufbxw_vec3 vertices[] = {
		{ -1.0f, 0.0f, -1.0f },
		{ +1.0f, 0.0f, -1.0f },
		{ -1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, +1.0f },
	};
	int32_t indices[] = {
		0, 2, 3, 1,
	};
	int32_t face_offsets[] = {
		0, 4,
	};

	ufbxw_vec3_buffer vertex_buffer = ufbxw_view_vec3_array(scene, vertices, ufbxwt_arraycount(vertices));
	ufbxw_int_buffer index_buffer = ufbxw_view_int_array(scene, indices, ufbxwt_arraycount(indices));
	ufbxw_int_buffer face_buffer = ufbxw_view_int_array(scene, face_offsets, ufbxwt_arraycount(face_offsets));

	ufbxw_mesh_set_vertices(scene, mesh, vertex_buffer);
	ufbxw_mesh_set_polygons(scene, mesh, index_buffer, face_buffer);

	ufbxw_blend_deformer deformer = ufbxw_create_blend_deformer(scene, mesh);
	ufbxw_blend_channel left_channel = ufbxw_create_blend_channel(scene, deformer);
	ufbxw_blend_channel right_channel = ufbxw_create_blend_channel(scene, deformer);

	ufbxw_set_name(scene, left_channel.id, "Left");
	ufbxw_set_name(scene, right_channel.id, "Right");

	ufbxw_blend_shape left_shape = ufbxw_create_blend_shape(scene);
	ufbxw_blend_shape right_shape = ufbxw_create_blend_shape(scene);

	ufbxw_set_name(scene, left_shape.id, "Left");
	ufbxw_set_name(scene, right_shape.id, "Right");

	int32_t left_indices[] = { 0, 2 };
	ufbxw_vec3 left_offsets[] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
	};

	int32_t right_indices[] = { 1, 3 };
	ufbxw_vec3 right_offsets[] = {
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
	};

	ufbxw_blend_shape_set_offsets(scene, left_shape,
		ufbxw_view_int_array(scene, left_indices, ufbxwt_arraycount(left_indices)),
		ufbxw_view_vec3_array(scene, left_offsets, ufbxwt_arraycount(left_offsets)));

	ufbxw_blend_shape_set_offsets(scene, right_shape,
		ufbxw_view_int_array(scene, right_indices, ufbxwt_arraycount(right_indices)),
		ufbxw_view_vec3_array(scene, right_offsets, ufbxwt_arraycount(right_offsets)));

	ufbxw_blend_channel_set_shape(scene, left_channel, left_shape);
	ufbxw_blend_channel_set_shape(scene, right_channel, right_shape);

	ufbxw_blend_channel_set_weight(scene, left_channel, 100.0);

	ufbxw_real left_weights[] = { 25.0, 50.0 };
	ufbxw_real right_weights[] = { 50.0, 75.0 };

	ufbxw_blend_channel_set_custom_full_weights(scene, left_channel,
		ufbxw_view_real_array(scene, left_weights, ufbxwt_arraycount(left_weights)));
	ufbxw_blend_channel_set_custom_full_weights(scene, right_channel,
		ufbxw_view_real_array(scene, right_weights, ufbxwt_arraycount(right_weights)));
}
#endif

UFBXWT_SCENE_CHECK(blender_full_weights)
#if UFBXWT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Node");
	ufbxwt_assert(node);

	ufbx_mesh *mesh = node->mesh;
	ufbxwt_assert(mesh);

	ufbxwt_assert(mesh->faces.count == 1);

	ufbx_face face = mesh->faces.data[0];
	ufbxwt_assert(face.index_begin == 0);
	ufbxwt_assert(face.num_indices == 4);

	ufbx_vec3 vertices[] = {
		{ -1.0f, 0.0f, -1.0f },
		{ -1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, +1.0f },
		{ +1.0f, 0.0f, -1.0f },
	};

	for (size_t i = 0; i < 4; i++) {
		ufbx_vec3 v = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbxwt_assert_close_uvec3(err, v, vertices[i]);
	}

	ufbxwt_assert(mesh->blend_deformers.count == 1);
	ufbx_blend_deformer *deformer = mesh->blend_deformers.data[0];

	ufbxwt_assert(deformer->channels.count == 2);

	{
		ufbx_blend_channel *channel = deformer->channels.data[0];
		ufbxwt_assert(!strcmp(channel->name.data, "Left"));

		ufbxwt_assert(channel->keyframes.count == 1);
		ufbx_blend_shape *shape = channel->target_shape;

		ufbx_real_list weights = shape->offset_weights;
		ufbxwt_assert(weights.count == 2);
		ufbxwt_assert_close_real(err, weights.data[0], 0.25f);
		ufbxwt_assert_close_real(err, weights.data[1], 0.5f);
	}

	{
		ufbx_blend_channel *channel = deformer->channels.data[1];
		ufbxwt_assert(!strcmp(channel->name.data, "Right"));

		ufbxwt_assert(channel->keyframes.count == 1);
		ufbx_blend_shape *shape = channel->target_shape;

		ufbx_real_list weights = shape->offset_weights;
		ufbxwt_assert(weights.count == 2);
		ufbxwt_assert_close_real(err, weights.data[0], 0.5f);
		ufbxwt_assert_close_real(err, weights.data[1], 0.75f);
	}
}
#endif
