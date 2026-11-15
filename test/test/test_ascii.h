#undef UFBXWT_TEST_GROUP
#define UFBXWT_TEST_GROUP "ascii"

#if UFBXWT_IMPL

static void ufbxwt_ascii_format_test(const char *name, ufbxw_scene *scene, const ufbxw_save_opts *opts, size_t result_size)
{
	static const uint32_t versions[] = { 7400, 7500 };

	void *result = malloc(result_size);
	ufbxwt_assert(result);

	ufbxwt_memory_stream stream = { result, result_size };

	ufbxw_write_stream ws = { 0 };
	ws.write_fn = &ufbxwt_memory_stream_write;
	ws.user = &stream;

	static const ufbxw_save_format formats[] = { UFBXW_SAVE_FORMAT_BINARY, UFBXW_SAVE_FORMAT_ASCII };

	for (int thread_ix = 0; thread_ix < UFBXWT_THREAD_IMPL_COUNT; thread_ix++) {
		for (int format_ix = 0; format_ix < ufbxwt_arraycount(formats); format_ix++) {
			for (int ascii_ix = 0; ascii_ix < UFBXWT_ASCII_FORMAT_IMPL_COUNT; ascii_ix++) {
				for (int float_ix = 0; float_ix < 2; float_ix++) {

					ufbxw_save_opts save_opts = { 0 };
					if (opts) {
						save_opts = *opts;
					}

					save_opts.version = 7500;
					save_opts.format = formats[format_ix];

					save_opts.threaded_min_ascii_floats = 1;
					save_opts.threaded_min_ascii_ints = 1;

					const char *float_name = "";
					switch (float_ix) {
					case 0:
						save_opts.ascii_float_format = UFBXW_ASCII_FLOAT_FORMAT_FIXED_PRECISION;
						float_name = "fixed_precision";
						break;
					case 1:
						save_opts.ascii_float_format = UFBXW_ASCII_FLOAT_FORMAT_ROUND_TRIP;
						float_name = "round_trip";
						break;
					}

					ufbxwt_ascii_format_impl ascii_impl = (ufbxwt_ascii_format_impl)ascii_ix;
					ufbxwt_thread_impl thread_impl = (ufbxwt_thread_impl)thread_ix;
					const char *format = save_opts.format == UFBXW_SAVE_FORMAT_ASCII ? "ascii" : "binary";

					if (save_opts.format == UFBXW_SAVE_FORMAT_BINARY && ascii_impl != UFBXWT_ASCII_FORMAT_IMPL_DEFAULT) {
						continue;
					}

					if (save_opts.format == UFBXW_SAVE_FORMAT_BINARY && float_ix != 0) {
						continue;
					}

					if (!ufbxwt_ascii_format_setup(&save_opts.ascii_formatter, ascii_impl)) {
						continue;
					}

					if (!ufbxwt_thread_setup(&save_opts.thread_sync, &save_opts.thread_pool, thread_impl)) {
						continue;
					}

					const char *ascii = ufbxwt_ascii_format_name(ascii_impl);
					const char *thread = ufbxwt_thread_impl_name(thread_impl);
					ufbxwt_logf("format: %s, version: %u, ascii: %s, float: %s, thread: %s", format, save_opts.version, ascii, float_name, thread);

					memset(result, 0, result_size);

					ufbxw_error save_error;
					bool save_ok = ufbxw_save_stream(scene, &ws, &save_opts, &save_error);

					if (!ufbxwt_ascii_format_supports_float_format(ascii_impl, save_opts.ascii_float_format)) {
						ufbxwt_assert(!save_ok);
						ufbxwt_assert(save_error.type == UFBXW_ERROR_ASCII_FORMAT);
						continue;
					}
					if (save_error.type != UFBXW_ERROR_NONE) {
						ufbxwt_log_error(&save_error);
					}
					ufbxwt_assert(save_ok);

					ufbx_error load_error;
					ufbx_scene *loaded_scene = ufbx_load_memory(result, result_size, NULL, &load_error);
					if (load_error.type != UFBX_ERROR_NONE) {
						ufbxwt_log_uerror(&load_error);
					}
					ufbxwt_assert(loaded_scene);
					ufbx_free_scene(loaded_scene);
				}
			}
		}
	}

	free(result);
	ufbxw_free_scene(scene);
}

#endif

UFBXWT_TEST(ascii_format_simple)
#if UFBXWT_IMPL
{
	ufbxwt_ascii_format_test("simple", ufbxwt_deflate_scene_simple(), NULL, 64 * 1024);
}
#endif

UFBXWT_TEST(ascii_format_fixed_precision)
#if UFBXWT_IMPL
{
	static const float float_values[] = {
		0.0f,
		-0.0f,
		1.23456789f,
		-1.23456789f,
		0.000123456789f,
		12345678.0f,
		1.0e-40f,
		3.4e38f,
		9.87654321f,
	};
	static const double double_values[] = {
		0.0,
		-0.0,
		1.2345678901234567,
		-1.2345678901234567,
		0.00012345678901234567,
		1234567890123456.0,
		1.0e-310,
		1.7e308,
		9.8765432109876543,
	};
	static const ufbxwt_ascii_format_impl ascii_impls[] = {
		UFBXWT_ASCII_FORMAT_IMPL_FMTLIB,
		UFBXWT_ASCII_FORMAT_IMPL_TO_CHARS,
		UFBXWT_ASCII_FORMAT_IMPL_ZMIJ,
		UFBXWT_ASCII_FORMAT_IMPL_ZMIJ_C,
	};

	for (int ascii_ix = 0; ascii_ix < ufbxwt_arraycount(ascii_impls); ascii_ix++) {
		ufbxwt_ascii_format_impl ascii_impl = ascii_impls[ascii_ix];
		ufbxw_ascii_formatter formatter = { 0 };
		if (!ufbxwt_ascii_format_setup(&formatter, ascii_impl)) {
			continue;
		}
		ufbxwt_logf("ascii: %s", ufbxwt_ascii_format_name(ascii_impl));

		char result[512], ref[512];
		size_t ref_size = 0;
		for (size_t i = 0; i < ufbxwt_arraycount(float_values); i++) {
			ref_size += (size_t)snprintf(ref + ref_size, sizeof(ref) - ref_size, "%.7g,", (double)float_values[i]);
		}

		size_t result_size = formatter.format_float_fn(formatter.user, result, sizeof(result), float_values, ufbxwt_arraycount(float_values), UFBXW_ASCII_FLOAT_FORMAT_FIXED_PRECISION);
		ufbxwt_assert(result_size == ref_size);
		ufbxwt_assert(memcmp(result, ref, result_size) == 0);

		ref_size = 0;
		for (size_t i = 0; i < ufbxwt_arraycount(double_values); i++) {
			ref_size += (size_t)snprintf(ref + ref_size, sizeof(ref) - ref_size, "%.15g,", double_values[i]);
		}

		result_size = formatter.format_double_fn(formatter.user, result, sizeof(result), double_values, ufbxwt_arraycount(double_values), UFBXW_ASCII_FLOAT_FORMAT_FIXED_PRECISION);
		ufbxwt_assert(result_size == ref_size);
		ufbxwt_assert(memcmp(result, ref, result_size) == 0);
	}
}
#endif

UFBXWT_TEST(ascii_format_round_trip)
#if UFBXWT_IMPL
{
	static const float float_values[] = {
		0.0f,
		-0.0f,
		1.23456789f,
		-1.23456789f,
		0.000123456789f,
		12345678.0f,
		1.0e-40f,
		3.4e38f,
		9.87654321f,
	};
	static const double double_values[] = {
		0.0,
		-0.0,
		1.2345678901234567,
		-1.2345678901234567,
		0.00012345678901234567,
		1234567890123456.0,
		1.0e-310,
		1.7e308,
		9.8765432109876543,
	};
	static const ufbxwt_ascii_format_impl ascii_impls[] = {
		UFBXWT_ASCII_FORMAT_IMPL_FMTLIB,
		UFBXWT_ASCII_FORMAT_IMPL_TO_CHARS,
		UFBXWT_ASCII_FORMAT_IMPL_ZMIJ,
		UFBXWT_ASCII_FORMAT_IMPL_ZMIJ_C,
	};

	for (int ascii_ix = 0; ascii_ix < ufbxwt_arraycount(ascii_impls); ascii_ix++) {
		ufbxwt_ascii_format_impl ascii_impl = ascii_impls[ascii_ix];
		ufbxw_ascii_formatter formatter = { 0 };
		if (!ufbxwt_ascii_format_setup(&formatter, ascii_impl)) {
			continue;
		}
		ufbxwt_logf("ascii: %s", ufbxwt_ascii_format_name(ascii_impl));

		char result[512];
		size_t result_size = formatter.format_float_fn(formatter.user, result, sizeof(result), float_values, ufbxwt_arraycount(float_values), UFBXW_ASCII_FLOAT_FORMAT_ROUND_TRIP);
		ufbxwt_assert(result_size < sizeof(result));
		result[result_size] = '\0';
		char *pos = result;
		for (size_t i = 0; i < ufbxwt_arraycount(float_values); i++) {
			char *end;
			float value = strtof(pos, &end);
			ufbxwt_assert(end != pos && *end == ',');
			ufbxwt_assert(memcmp(&value, &float_values[i], sizeof(value)) == 0);
			pos = end + 1;
		}
		ufbxwt_assert(pos == result + result_size);

		result_size = formatter.format_double_fn(formatter.user, result, sizeof(result), double_values, ufbxwt_arraycount(double_values), UFBXW_ASCII_FLOAT_FORMAT_ROUND_TRIP);
		ufbxwt_assert(result_size < sizeof(result));
		result[result_size] = '\0';
		pos = result;
		for (size_t i = 0; i < ufbxwt_arraycount(double_values); i++) {
			char *end;
			double value = strtod(pos, &end);
			ufbxwt_assert(end != pos && *end == ',');
			ufbxwt_assert(memcmp(&value, &double_values[i], sizeof(value)) == 0);
			pos = end + 1;
		}
		ufbxwt_assert(pos == result + result_size);
	}
}
#endif
