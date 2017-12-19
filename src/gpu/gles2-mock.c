#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include <api.h>
#include <log.h>

static const char* gl_renderer = NULL;

void CPUINFO_ABI cpuinfo_mock_gl_renderer(const char* renderer) {
	gl_renderer = renderer;
}

bool cpuinfo_gpu_query_gles2(char renderer[restrict static CPUINFO_GPU_NAME_MAX]) {
	if (gl_renderer != NULL) {
		strncpy(renderer, gl_renderer, CPUINFO_GPU_NAME_MAX);
	}
	return gl_renderer != NULL;
}
