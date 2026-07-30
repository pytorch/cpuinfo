#include <stdio.h>
#include <string.h>
#include <cpuinfo-mock.h>
#include <arm/linux/api.h>

static struct cpuinfo_mock_file mock_files[] = {
    {
        .path = "/proc/cpuinfo",
        .size = 0, // Will be initialized in main
        .content =
            "Hardware\t: "
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n",
    },
    {.path = NULL}};

int main() {
  mock_files[0].size = strlen(mock_files[0].content);
  cpuinfo_mock_filesystem(mock_files);

  char hardware[64];
  char revision[9];
  struct cpuinfo_arm_linux_processor processors[1];

  printf("Calling cpuinfo_arm_linux_parse_proc_cpuinfo...\n");
  cpuinfo_arm_linux_parse_proc_cpuinfo(hardware, revision, 1, processors);

  printf("Hardware parsed: %s\n", hardware);

  char expected[64];
  memset(expected, 'A', 63);
  expected[63] = '\0';

  if (strcmp(hardware, expected) != 0) {
    fprintf(stderr, "FAIL: expected length 63, got %s (length %zu)\n", hardware, strlen(hardware));
    return 1;
  }
  printf("PASS\n");
  return 0;
}
