#ifndef __QNX_API__
#define __QNX_API__

#define MAX_PROCESSOR	8
#define MAX_CACHE	5 // -> L1i, L1d, L2, L3, L4

extern struct cpuinfo_processor * qnx_processors;
extern struct cpuinfo_cache * qnx_cache;
extern struct cpuinfo_core * qnx_core;
extern struct cpuinfo_cluster * qnx_cluster;
extern struct cpuinfo_package * qnx_package;

void cpuinfo_qnx_init();
int cpuinfo_qnx_fill_processors_info(struct cpuinfo_processor * processor_list);
const struct cpuinfo_processor * cpuinfo_qnx_get_processor(uint32_t index);
const struct cpuinfo_core* cpuinfo_qnx_get_core(void);
const struct cpuinfo_cluster* cpuinfo_qnx_get_cluster(void);
const struct cpuinfo_package* cpuinfo_qnx_get_package(void);
uint32_t cpuinfo_qnx_get_processors_count(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l1i_caches(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l1d_caches(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l2_caches(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l3_caches(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l4_caches(void);
const struct cpuinfo_cache* cpuinfo_qnx_get_l1i_cache(uint32_t index);
const struct cpuinfo_cache* cpuinfo_qnx_get_l1d_cache(uint32_t index);
const struct cpuinfo_cache* cpuinfo_qnx_get_l2_cache(uint32_t index);
const struct cpuinfo_cache* cpuinfo_qnx_get_l3_cache(uint32_t index);
const struct cpuinfo_cache* cpuinfo_qnx_get_l4_cache(uint32_t index);
enum cpuinfo_uarch cpuinfo_qnx_get_uarch();
enum cpuinfo_vendor cpuinfo_qnx_get_vendor();
void cpuinfo_qnx_deinit();
void cpuinfo_arm_qnx_decode_cache( enum cpuinfo_uarch uarch, uint32_t cluster_cores, uint32_t midr, struct cpuinfo_cache l1i[restrict static 1], struct cpuinfo_cache l1d[restrict static 1], struct cpuinfo_cache l2[restrict static 1], struct cpuinfo_cache l3[restrict static 1]);
uint32_t cpuinfo_arm_compute_max_cache_size(const struct cpuinfo_processor* processor);

#endif // __QNX_API__
