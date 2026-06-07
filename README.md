# ufbx_write

Single source file FBX file exporter.

**NOTE:** ufbx_write is still a work-in-progress, and issues/breaking changes are to be expected.

```c
ufbxw_scene_opts scene_opts = { 0 }; // Optional, pass NULL for defaults
ufbxw_scene *scene = ufbxw_create_scene(&scene_opts);

// Create a node 'Plane', and set its position
// In ufbx_write, all lifetimes are managed within `ufbxw_scene`
ufbxw_node node = ufbxw_create_node(scene);
ufbxw_set_name(scene, node.id, "Plane");

ufbxw_vec3 position = { 1.0f, 2.0f, 3.0f };
ufbxw_node_set_translation(scene, node, position);

// Create a mesh and attach it to the 'Plane' node
ufbxw_mesh mesh = ufbxw_create_mesh(scene);
ufbxw_mesh_add_instance(scene, mesh, node);

// Create some geometry
// In ufbx_write, data is represented by buffer objects, allowing you to write
// directly to ufbx_write backed memory, or even streaming the data
ufbxw_vec3 vertices[] = {
    { -1.0f, 0.0f, -1.0f },
    { +1.0f, 0.0f, -1.0f },
    { -1.0f, 0.0f, +1.0f },
    { +1.0f, 0.0f, +1.0f },
};
int32_t indices[] = { 0, 2, 3, 1 };
int32_t face_offsets[] = { 0, 4 };

ufbxw_vec3_buffer vertex_buffer = ufbxw_view_vec3_array(scene, vertices, 4);
ufbxw_int_buffer index_buffer = ufbxw_view_int_array(scene, indices, 4);
ufbxw_int_buffer face_buffer = ufbxw_view_int_array(scene, face_offsets, 2);

ufbxw_mesh_set_vertices(scene, mesh, vertex_buffer);
ufbxw_mesh_set_polygons(scene, mesh, index_buffer, face_buffer);

// Prepare the scene for saving, ufbx_write automatically patches up redundant
// FBX structures that can be inferred from the scene for you
ufbxw_prepare_scene(scene, &ufbxw_default_prepare_opts);

// Save the scene into a file
ufbxw_save_opts save_opts = { 0 };
save_opts.format = UFBXW_SAVE_FORMAT_BINARY;
save_opts.version = 7500;

ufbxw_error save_error; // Optional, pass NULL if you don't care about errors
bool ok = ufbxw_save_file(scene, "my-scene.fbx", &save_opts, &save_error);
if (!ok) {
    fprintf(stderr, "Failed to save: %s\n", save_error.description);
    exit(1);
}

ufbxw_free_scene(scene);
```

## Setup

Copy `ufbx_write.h` and `ufbx_write.c` to your project, `ufbx_write.c` needs to be compiled as C99/C++11 or more recent.

### ASCII formatting

The internal ASCII formatting uses `snprintf()` and is very slow compared to the state-of-the-art.
Under `extra/`, you can find options for faster alternatives for C++17 `std::to_chars()`, [fmtlib](https://github.com/fmtlib/fmt) and [Żmij](https://github.com/vitaut/zmij).

### Threading

ufbx_write does not have any OS-specific code, and by default does not use multiple threads.
`extra/ufbxw_cpp_threads.h` has a C++11-based threading implementation you can plug into ufbx_write.

## License

```
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Samuli Raivio
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------
```
