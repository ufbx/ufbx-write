#undef UFBXWT_TEST_GROUP
#define UFBXWT_TEST_GROUP "basic"

#if UFBXWT_IMPL
const char *ufbxwt_prop_flag_string(ufbx_element *element, const char *prop)
{
	if (!element || !element->dom_node) return "";

	ufbx_dom_node *props = ufbx_dom_find(element->dom_node, "Properties70");
	if (!props) return "";

	for (size_t i = 0; i < props->children.count; i++) {
		ufbx_dom_node *child = props->children.data[i];
		ufbxwt_assert(child->values.count >= 4);

		ufbx_dom_value *values = child->values.data;
		ufbxwt_assert(values[0].type == UFBX_DOM_VALUE_STRING);
		ufbxwt_assert(values[1].type == UFBX_DOM_VALUE_STRING);
		ufbxwt_assert(values[2].type == UFBX_DOM_VALUE_STRING);
		ufbxwt_assert(values[3].type == UFBX_DOM_VALUE_STRING);
		if (!strcmp(values[0].value_str.data, prop)) {
			return values[3].value_str.data;
		}
	}

	return "";
}
#endif

UFBXWT_TEST(create_scene)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_TEST(memory_stats)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_memory_stats stats = ufbxw_get_memory_stats(scene);
	ufbxwt_assert(stats.allocated_bytes > 0);
	ufbxwt_assert(stats.allocation_count > 0);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_TEST(reconnect_node_parent)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_node parent_a = ufbxwt_create_node(scene, "ParentA");
	ufbxw_node parent_b = ufbxwt_create_node(scene, "ParentB");
	ufbxw_node child = ufbxwt_create_node(scene, "Child");

	ufbxw_node_set_parent(scene, child, parent_a);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == parent_a.id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent_a) == 1);
	ufbxwt_assert(ufbxw_node_get_child(scene, parent_a, 0).id == child.id);

	ufbxw_node_set_parent(scene, child, parent_b);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == parent_b.id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent_a) == 0);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent_b) == 1);
	ufbxwt_assert(ufbxw_node_get_child(scene, parent_b, 0).id == child.id);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_TEST(connect_disconnect)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_node parent = ufbxwt_create_node(scene, "Parent");
	ufbxw_node child = ufbxwt_create_node(scene, "Child");

	ufbxw_connect(scene, UFBXW_CONNECTION_NODE_PARENT, child.id, parent.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == parent.id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 1);

	ufbxw_disconnect(scene, UFBXW_CONNECTION_NODE_PARENT, child.id, parent.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == ufbxw_null_id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 0);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_TEST(fbx_connect_disconnect)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_node parent = ufbxwt_create_node(scene, "Parent");
	ufbxw_node child = ufbxwt_create_node(scene, "Child");

	ufbxw_fbx_connect(scene, child.id, parent.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == parent.id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 1);

	ufbxw_fbx_disconnect(scene, child.id, parent.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == ufbxw_null_id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 0);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_TEST(delete_middle_parent)
#if UFBXWT_IMPL
{
	ufbxw_scene *scene = ufbxw_create_scene(NULL);
	ufbxwt_assert(scene);

	ufbxw_node parent = ufbxwt_create_node(scene, "Parent");
	ufbxw_node middle = ufbxwt_create_node(scene, "Middle");
	ufbxw_node child = ufbxwt_create_node(scene, "Child");

	ufbxw_node_set_parent(scene, middle, parent);
	ufbxw_node_set_parent(scene, child, middle);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 1);
	ufbxwt_assert(ufbxw_node_get_child(scene, parent, 0).id == middle.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == middle.id);

	ufbxw_delete_element(scene, middle.id);
	ufbxwt_assert(ufbxw_node_get_parent(scene, child).id == ufbxw_null_id);
	ufbxwt_assert(ufbxw_node_get_num_children(scene, parent) == 0);

	ufbxw_free_scene(scene);
}
#endif

UFBXWT_SCENE_TEST(simple_node)
#if UFBXWT_IMPL
{
	ufbxw_node node = ufbxw_create_node(scene);
	ufbxw_set_name(scene, node.id, "Test_Node");

	ufbxw_vec3 translation = { 1.0f, 2.0f, 3.0f };
	ufbxw_node_set_translation(scene, node, translation);

	ufbxw_vec3 scaling = { 1.0f, 0.5f, 0.25f };
	ufbxw_set_vec3(scene, node.id, "Lcl Scaling", scaling);

	ufbxwt_check_error(scene);

	ufbxw_vec3 field_translation = ufbxw_node_get_translation(scene, node);
	ufbxw_vec3 prop_translation = ufbxw_get_vec3(scene, node.id, "Lcl Translation");
	ufbxwt_assert(ufbxwt_equal_vec3(field_translation, translation));
	ufbxwt_assert(ufbxwt_equal_vec3(prop_translation, translation));

	ufbxw_vec3 field_scaling = ufbxw_node_get_scaling(scene, node);
	ufbxw_vec3 prop_scaling = ufbxw_get_vec3(scene, node.id, "Lcl Scaling");
	ufbxwt_assert(ufbxwt_equal_vec3(field_scaling, scaling));
	ufbxwt_assert(ufbxwt_equal_vec3(prop_scaling, scaling));
}
#endif

UFBXWT_SCENE_CHECK(simple_node)
#if UFBXWT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Test_Node");
	ufbxwt_assert(node);

	ufbx_vec3 translation = { 1.0f, 2.0f, 3.0f };
	ufbxwt_assert_close_uvec3(err, node->local_transform.translation, translation);

	ufbx_vec3 scale = { 1.0f, 0.5f, 0.25f };
	ufbxwt_assert_close_uvec3(err, node->local_transform.scale, scale);
}
#endif

UFBXWT_SCENE_TEST(property_flags)
#if UFBXWT_IMPL
{
	ufbxw_node node = ufbxw_create_node(scene);
	ufbxw_set_name(scene, node.id, "Node");

	const uint32_t translation_flags = UFBXW_PROP_FLAG_ANIMATABLE | UFBXW_PROP_FLAG_HIDDEN;
	ufbxwt_assert(ufbxw_get_prop_flags(scene, node.id, UFBX_Lcl_Translation) == UFBXW_PROP_FLAG_ANIMATABLE);
	ufbxw_override_prop_flags(scene, node.id, UFBX_Lcl_Translation, translation_flags);
	ufbxwt_assert(ufbxw_get_prop_flags(scene, node.id, UFBX_Lcl_Translation) == translation_flags);

	ufbxw_add_string(scene, node.id, "Notes", UFBXW_PROP_TYPE_STRING, "It is a node");
	ufbxw_override_prop_flags(scene, node.id, "Notes", UFBXW_PROP_FLAG_USER_DEFINED);
	ufbxwt_assert(ufbxw_get_prop_flags(scene, node.id, "Notes") == UFBXW_PROP_FLAG_USER_DEFINED);

	const uint32_t offset_flags = UFBXW_PROP_FLAG_USER_DEFINED | UFBXW_PROP_FLAG_LOCK_X | UFBXW_PROP_FLAG_MUTE_Y | UFBXW_PROP_FLAG_MUTE_Z;
	ufbxw_add_vec3(scene, node.id, "Offset", UFBXW_PROP_TYPE_VECTOR3D, ufbxw_zero_vec3);
	ufbxw_override_prop_flags(scene, node.id, "Offset", offset_flags);
	ufbxwt_assert(ufbxw_get_prop_flags(scene, node.id, "Offset") == offset_flags);

	ufbxw_anim_layer layer = ufbxw_get_default_anim_layer(scene);
	ufbxw_animate_prop(scene, node.id, "Offset", layer);
}
#endif

UFBXWT_SCENE_CHECK(property_flags)
#if UFBXWT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Node");
	ufbxwt_assert(node);

	{
		ufbx_prop *prop = ufbx_find_prop(&node->props, UFBX_Lcl_Translation);
		ufbxwt_assert(prop);
		ufbxwt_assert(prop->flags & UFBX_PROP_FLAG_ANIMATABLE);
		ufbxwt_assert(prop->flags & UFBX_PROP_FLAG_HIDDEN);
		ufbxwt_assert(!strcmp(ufbxwt_prop_flag_string(&node->element, UFBX_Lcl_Translation), "AH"));
	}

	{
		ufbx_prop *prop = ufbx_find_prop(&node->props, "Notes");
		ufbxwt_assert(prop);
		ufbxwt_assert(prop->flags & UFBX_PROP_FLAG_USER_DEFINED);
		ufbxwt_assert(!strcmp(prop->value_str.data, "It is a node"));
		ufbxwt_assert(!strcmp(ufbxwt_prop_flag_string(&node->element, "Notes"), "U"));
	}

	{
		ufbx_prop *prop = ufbx_find_prop(&node->props, "Offset");
		ufbxwt_assert(prop);
		const uint32_t flags = UFBX_PROP_FLAG_ANIMATABLE | UFBX_PROP_FLAG_ANIMATED | UFBX_PROP_FLAG_USER_DEFINED | UFBX_PROP_FLAG_LOCK_X | UFBX_PROP_FLAG_MUTE_Y | UFBX_PROP_FLAG_MUTE_Z;
		ufbxwt_assert((prop->flags & flags) == flags);
		ufbxwt_assert(!strcmp(ufbxwt_prop_flag_string(&node->element, "Offset"), "A+UM6L1"));
	}
}
#endif
