#include <sys/neutrino.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/syspage.h>
#include <string.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <qnx/api.h>

#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
    #include <arm/midr.h>
    #include <arm/api.h>
#endif

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
        #include <x86_64/inline.h>
        #include <x86/api.h>
#endif

// Please note the following:
// Since not enough information is provided in system page the following assumptions has been made
// Assuming one core per package
// Assuming identical processors within a core

// Since Optimization is no needed by customer only features used by libtorch are implemented in this library to speed development

// TODO:
// For x86_64 cpuid assembly instruction can be used to read information about the processor
// For ARM Main Register ID can be read to get more information about CPU
// Use assembly to detect a lot more CPU features.

extern struct syspage_entry *_syspage_ptr;

struct cpuinfo_processor * qnx_processors;
struct cpuinfo_cache * qnx_cache;
struct cpuinfo_core * qnx_core;
struct cpuinfo_cluster * qnx_cluster;
struct cpuinfo_package * qnx_package;

enum cpuinfo_vendor cpuinfo_qnx_get_vendor(){
    // To figure out the vendor if the processor
    // we will read the name of the cpu from the system page
    int cpu_name_index = SYSPAGE_ARRAY_IDX(cpuinfo,0)->name;
    char * current_string = (char *)SYSPAGE_ENTRY(strings);
    if ( current_string == NULL ){
        cpuinfo_log_fatal("cpuinfo_qnx_get_vendor: system page strings is NULL");
        return cpuinfo_vendor_unknown;
    }
    char * cpu_name = &current_string[cpu_name_index];
    if ( cpu_name == NULL ){
        cpuinfo_log_fatal("cpuinfo_qnx_get_vendor: cpu_name is NULL");
        return cpuinfo_vendor_unknown;
    }
    // The list of cpus and their mappings, more processors can be added here
    if( strstr(cpu_name, "Cortex") != NULL )
        return cpuinfo_vendor_arm;
    if( strstr(cpu_name, "Intel") != NULL || strstr(cpu_name, "Core") != NULL )
        return cpuinfo_vendor_intel;
    else
        return cpuinfo_vendor_unknown;
}

enum cpuinfo_uarch cpuinfo_qnx_get_uarch(){

    int cpu_name_index = SYSPAGE_ARRAY_IDX(cpuinfo,0)->name;
    char * current_string = (char *)SYSPAGE_ENTRY(strings);
    if ( current_string == NULL ){
        cpuinfo_log_fatal("cpuinfo_qnx_get_vendor: system page strings is NULL");
        return cpuinfo_uarch_unknown;
    }
    char * cpu_name = &current_string[cpu_name_index];
    if ( cpu_name == NULL ){
        cpuinfo_log_fatal("cpuinfo_qnx_get_vendor: cpu_name is NULL");
        return cpuinfo_uarch_unknown;
    }
    // The list of cpus and their mappings, more processors can be added here
    if( strstr(cpu_name, "Core i7/5/3") != NULL )
        return cpuinfo_uarch_sky_lake;
    // cpu_name is an ASCII, NULL-terminated machine name. http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.building/topic/syspage/cpuinfo.html
    char * token = strstr(cpu_name, "A7");
    if( token != NULL && (token+2) != NULL && (*(token+2)==' ' || *(token+2)=='\0') )
        return cpuinfo_uarch_cortex_a7;
    if( strstr(cpu_name, "A53") != NULL )
        return cpuinfo_uarch_cortex_a53;
    if( strstr(cpu_name, "A55R0") != NULL )
        return cpuinfo_uarch_cortex_a55r0;
    if( strstr(cpu_name, "A55") != NULL )
        return cpuinfo_uarch_cortex_a55;
    if( strstr(cpu_name, "A57") != NULL )
        return cpuinfo_uarch_cortex_a57;
    if( strstr(cpu_name, "A65") != NULL )
        return cpuinfo_uarch_cortex_a65;
    if( strstr(cpu_name, "A72") != NULL )
        return cpuinfo_uarch_cortex_a72;
    if( strstr(cpu_name, "A73") != NULL )
        return cpuinfo_uarch_cortex_a73;
    if( strstr(cpu_name, "A75") != NULL )
        return cpuinfo_uarch_cortex_a75;
    if( strstr(cpu_name, "A76") != NULL )
        return cpuinfo_uarch_cortex_a76;
    if( strstr(cpu_name, "A77") != NULL )
        return cpuinfo_uarch_cortex_a77;
    if( strstr(cpu_name, "A78") != NULL )
        return cpuinfo_uarch_cortex_a78;
    if( strstr(cpu_name, "Neoverse-N1") != NULL )
        return cpuinfo_uarch_neoverse_n1;
    if( strstr(cpu_name, "Neoverse-E1") != NULL )
        return cpuinfo_uarch_neoverse_e1;
    if( strstr(cpu_name, "Scorpion") != NULL )
        return cpuinfo_uarch_scorpion;
    if( strstr(cpu_name, "Krait") != NULL )
        return cpuinfo_uarch_krait;
    if( strstr(cpu_name, "Kryo") != NULL )
        return cpuinfo_uarch_kryo;
    if( strstr(cpu_name, "falkor") != NULL )
        return cpuinfo_uarch_falkor;
    if( strstr(cpu_name, "Saphira") != NULL )
        return cpuinfo_uarch_saphira;
    else
        return cpuinfo_uarch_unknown;
}

int cpuinfo_qnx_fill_core_info(struct cpuinfo_core * core){
    core->processor_start = 0;
    core->processor_count = _syspage_ptr->num_cpu;
    cpuinfo_cores_count = 1;
    if ( core->processor_count > MAX_PROCESSOR || core->processor_count == 0 ){
        cpuinfo_log_fatal("cpuinfo_qnx_fill_processors_info: num_cpu value is invalid");
        return -1;
    }
    core->core_id = 0;
    core->cluster = qnx_cluster;
    core->package = qnx_package;
    core->vendor = cpuinfo_qnx_get_vendor();
    core->uarch = cpuinfo_qnx_get_uarch();
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	/** Value of CPUID leaf 1 EAX register for this core */
    uint32_t EAX=0;
    uint32_t EBX=0;
    uint32_t ECX=0;
    uint32_t EDX=0;
    uint32_t leaf = 1;
    x86_cpuid1(leaf, &EAX, &EBX, &ECX, &EDX );
    core->cpuid = EAX;
#elif CPUINFO_ARCH_ARM64
	/** Value of Main ID Register (MIDR) for this core */
    __asm__("mrs %0, MIDR_EL1" : "=r"(core->midr));
#elif CPUINFO_ARCH_ARM
    /** Value of Main ID Register (MIDR) for this core */
    __asm__("mrc p15,0,%0,c0,c0,0":"=r"(core->midr));
#endif
    core->frequency = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
    return 0;
}

void cpuinfo_qnx_fill_cluster_info(struct cpuinfo_core * cluster){
    // TODO
}

void cpuinfo_qnx_fill_package_info(struct cpuinfo_core * package){
    // TODO
}

int cpuinfo_qnx_fill_processors_info(struct cpuinfo_processor * processor_list){

    int ret = -1;

    ret = cpuinfo_qnx_fill_core_info(qnx_core);
    if(ret!=0){
        cpuinfo_log_fatal("cpuinfo_qnx_fill_processors_info: cpuinfo_qnx_fill_core_info failed");
        return -1;
    }

    int num_cpu = _syspage_ptr->num_cpu;
    if ( num_cpu > MAX_PROCESSOR ){
        cpuinfo_log_fatal("cpuinfo_qnx_fill_processors_info: num_cpu value from syspage greater than MAX_PROCESSOR");
        return -1;
    }

    cpuinfo_clusters_count = 1;
    cpuinfo_packages_count = 1;

    int l1i_index = SYSPAGE_ARRAY_IDX(cpuinfo,0)->ins_cache;
    if ( l1i_index != 0 || l1i_index != 1 ){
        l1i_index = 0;
    }
    int l1d_index = SYSPAGE_ARRAY_IDX(cpuinfo,0)->data_cache;
    if ( l1d_index != 0 || l1d_index != 1 ){
        l1d_index = 1;
    }

#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
        uint32_t midr=0;
        #if CPUINFO_ARCH_ARM64
            /** Value of Main ID Register (MIDR) for this core */
            __asm__("mrs %0, MIDR_EL1" : "=r"(midr));
        #elif CPUINFO_ARCH_ARM
            /** Value of Main ID Register (MIDR) for this core */
            __asm__("mrc p15,0,%0,c0,c0,0":"=r"(midr));
        #endif
	    uint32_t cluster_cores = 1;
        enum cpuinfo_uarch uarch;
        enum cpuinfo_vendor vendor;
        #if CPUINFO_ARCH_ARM
            cpuinfo_arm_decode_vendor_uarch(midr,1,&vendor,&uarch);
        #else
            cpuinfo_arm_decode_vendor_uarch(midr,&vendor,&uarch);
        #endif
        cpuinfo_arm_qnx_decode_cache( uarch, cluster_cores, midr, &qnx_cache[0], &qnx_cache[1], &qnx_cache[2], &qnx_cache[3]);
#elif CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
        struct cpuid_regs regs;
        uint32_t package_cores_max = 0;
        struct cpuinfo_x86_caches caches;
        struct cpuid_regs leaf4;
		uint32_t input_ecx = 0;
		do {
            // To get L1, L2 and L3 cache sizes, you need to use CPUID with EAX=4, and set ECX to 0, 1, 2, ...
			x86_cpuid2(4, input_ecx++, &regs.eax, &regs.ebx, &regs.ecx, &regs.edx );
		} while (cpuinfo_x86_decode_deterministic_cache_parameters(regs, &caches, &package_cores_max));
        int j;
        struct cpuinfo_x86_cache * cptr = &caches.l1i;
        for( j = 0; ( j < input_ecx - 1 ) && ( j < MAX_CACHE ); j++ ){
            qnx_cache[j].associativity = cptr->associativity;
            qnx_cache[j].size = cptr->size;
            qnx_cache[j].line_size = cptr->line_size;
            qnx_cache[j].partitions = cptr->partitions;
            qnx_cache[j].sets = cptr->sets;
            cptr++;
        }
#endif

    int i;
    for( i=0; i<num_cpu && i<MAX_PROCESSOR; i++ ) {
        processor_list[i].smt_id = SYSPAGE_ARRAY_IDX(cpuinfo,0)->smp_hwcoreid;
        // Assuming processors have identical cache
        processor_list[i].cache.l1i = &qnx_cache[0];
        processor_list[i].cache.l1d = &qnx_cache[1];
        processor_list[i].cache.l2 = &qnx_cache[2];
        processor_list[i].cache.l3 = &qnx_cache[3];
        processor_list[i].cache.l4 = &qnx_cache[4];
        // Use same core, cluster and package info for all processors
        // Directly reading cpu id registers can allow more information about cpus to be parsed
        processor_list[i].core = qnx_core;
        processor_list[i].cluster = qnx_cluster;
        processor_list[i].package = qnx_package;
    }
    return 0;
}

void cpuinfo_qnx_init(void){

    bool cleanup = false;

    if ( ThreadCtl(_NTO_TCTL_IO_PRIV, NULL) == -1) {
        cpuinfo_log_fatal("Failed to get I/O access permission");
        cleanup = true;
    }

    // allocate memory:
    qnx_processors = calloc( MAX_PROCESSOR, sizeof( struct cpuinfo_processor) );
    if( qnx_processors == NULL ){
        cpuinfo_log_fatal("Failed to allocate memory for qnx_processors");
        cleanup = true;
    }

    qnx_cache = calloc( MAX_CACHE, sizeof( struct cpuinfo_cache ) );
    if( qnx_cache == NULL ){
        cpuinfo_log_fatal("Failed to allocate memory for qnx_cache");
        cleanup = true;
    }

    qnx_core = calloc( 1, sizeof( struct cpuinfo_core ) );
    if( qnx_core == NULL ){
        cpuinfo_log_fatal("Failed to allocate memory for qnx_core");
        cleanup = true;
    }

    qnx_cluster = calloc( 1, sizeof( struct cpuinfo_cluster ) );
    if( qnx_cluster == NULL ){
        cpuinfo_log_fatal("Failed to allocate memory for qnx_cluster");
        cleanup = true;
    }

    qnx_package = calloc( 1, sizeof( struct cpuinfo_package ) );
    if( qnx_package == NULL ){
        cpuinfo_log_fatal("Failed to allocate memory for qnx_package");
        cleanup = true;
    }

    int ret = cpuinfo_qnx_fill_processors_info(qnx_processors);
    if( ret != 0 ){
        cleanup = true;
    }

    __sync_synchronize();

    if( cleanup == true ){
        cpuinfo_log_fatal("CPUINIT-FAILED");
         // free memory on failure:
         if(qnx_processors != NULL){
             free(qnx_processors);
         }

         if(qnx_cache != NULL){
             free(qnx_cache);
         }

         if(qnx_core != NULL){
             free(qnx_core);
         }

         if(qnx_cluster != NULL){
             free(qnx_cluster);
         }

         if(qnx_package != NULL){
             free(qnx_package);
         }
     }
     else{
         cpuinfo_is_initialized = true;
     }
}

const struct cpuinfo_processor * cpuinfo_qnx_get_processor(uint32_t index){
    return (const struct cpuinfo_processor *)&qnx_processors[index];
}

const struct cpuinfo_core * cpuinfo_qnx_get_core(void){
    return (const struct cpuinfo_core *)qnx_core;
}

const struct cpuinfo_cluster* cpuinfo_qnx_get_cluster(void){
    return (const struct cpuinfo_cluster*)qnx_cluster;
}

const struct cpuinfo_package* cpuinfo_qnx_get_package(void){
    return (const struct cpuinfo_package*)qnx_package;
}

uint32_t cpuinfo_qnx_get_processors_count(void){
    cpuinfo_processors_count = _syspage_ptr->num_cpu;
    if ( cpuinfo_processors_count > MAX_PROCESSOR ){
        cpuinfo_log_fatal("cpuinfo_qnx_fill_processors_info: num_cpu value from syspage greater than MAX_PROCESSOR");
        return 1;
    }
    return cpuinfo_processors_count;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l1i_caches(void){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l1i;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l1d_caches(void){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l1d;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l2_caches(void){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l2;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l3_caches(void){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l3;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l4_caches(void){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l4;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l1i_cache(uint32_t index){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l1i;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l1d_cache(uint32_t index){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l1d;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l2_cache(uint32_t index){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l2;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l3_cache(uint32_t index){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l3;
}

const struct cpuinfo_cache* cpuinfo_qnx_get_l4_cache(uint32_t index){
    return (const struct cpuinfo_cache*)qnx_processors[0].cache.l4;
}

void cpuinfo_qnx_deinit(){
    // free memory:
     if( qnx_processors != NULL ){
         free(qnx_processors);
     }

     if( qnx_cache!=NULL ){
         free(qnx_cache);
     }

     if( qnx_core != NULL ){
         free(qnx_core);
     }

     if( qnx_cluster != NULL ){
         free(qnx_cluster);
     }

     if( qnx_package != NULL ){
         free(qnx_package);
     }
}

#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64

void cpuinfo_arm_qnx_decode_cache(
	enum cpuinfo_uarch uarch,
	uint32_t cluster_cores,
	uint32_t midr,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1])
{
	switch (uarch) {
#if CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_7A__) && !defined(__ARM_ARCH_8A__)
		case cpuinfo_uarch_xscale:
			switch (midr_get_part(midr) >> 8) {
				case 2:
					/*
					 * PXA 210/25X/26X
					 *
					 * See "Computer Organization and Design, Revised Printing: The Hardware/Software Interface"
					 *     by David A. Patterson, John L. Hennessy
					 */
					*l1i = (struct cpuinfo_cache) {
						.size = 16 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 16 * 1024,
						.associativity = 4,
						.line_size = 64
					};
					break;
				case 4:
					/* PXA 27X */
					*l1i = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					break;
				case 6:
					/*
					 * PXA 3XX
					 *
					 * See http://download.intel.com/design/intelxscale/31628302.pdf
					 */
					*l1i = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 4,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 4,
						.line_size = 32
					};
					*l2 = (struct cpuinfo_cache) {
						.size = 256 * 1024,
						.associativity = 8,
						.line_size = 32
					};
					break;
			}
			break;
		case cpuinfo_uarch_arm11:
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			break;
#endif /* CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_7A__) && !defined(__ARM_ARCH_8A__) */
#if CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_8A__)
		case cpuinfo_uarch_cortex_a5:
			/*
			 * Cortex-A5 Technical Reference Manual:
			 * 7.1.1. Memory system
			 *   The Cortex-A5 processor has separate instruction and data caches.
			 *   The caches have the following features:
			 *    - Data cache is 4-way set-associative.
			 *    - Instruction cache is 2-way set-associative.
			 *    - The cache line length is eight words.
			 *    - You can configure the instruction and data caches independently during implementation
			 *      to sizes of 4KB, 8KB, 16KB, 32KB, or 64KB.
			 * 1.1.3. System design components
			 *    PrimeCell Level 2 Cache Controller (PL310)
			 *      The addition of an on-chip secondary cache, also referred to as a Level 2 or L2 cache, is a
			 *      recognized method of improving the performance of ARM-based systems when significant memory traffic
			 *      is generated by the processor. The PrimeCell Level 2 Cache Controller reduces the number of external
			 *      memory accesses and has been optimized for use with the Cortex-A5 processor.
			 * 8.1.7. Exclusive L2 cache
			 *    The Cortex-A5 processor can be connected to an L2 cache that supports an exclusive cache mode.
			 *    This mode must be activated both in the Cortex-A5 processor and in the L2 cache controller.
			 *
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Processor model    | L1D cache | L1I cache | L2 cache | Reference |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Qualcomm MSM7225A  |           |           |          |           |
			 *  | Qualcomm MSM7625A  |           |           |          |           |
			 *  | Qualcomm MSM7227A  |           |           |          |           |
			 *  | Qualcomm MSM7627A  |    32K    |    32K    |   256K   | Wiki [1]  |
			 *  | Qualcomm MSM7225AB |           |           |          |           |
			 *  | Qualcomm MSM7225AB |           |           |          |           |
			 *  | Qualcomm QSD8250   |           |           |          |           |
			 *  | Qualcomm QSD8650   |           |           |          |           |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Spreadtrum SC6821  |    32K    |    32K    |    ?     |           |
			 *  | Spreadtrum SC6825  |    32K    |    32K    |   256K   | Wiki [2]  |
			 *  | Spreadtrum SC8810  |     ?     |     ?     |    ?     |           |
			 *  | Spreadtrum SC8825  |    32K    |    32K    |    ?     |           |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *
			 * [1] https://en.wikipedia.org/wiki/List_of_Qualcomm_Snapdragon_systems-on-chip#Snapdragon_S1
			 * [2] https://en.wikipedia.org/wiki/Spreadtrum
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024,
				/*
				 * Follow NXP specification: "Eight-way set-associative 512 kB L2 cache with 32B line size"
				 * Reference: http://www.nxp.com/assets/documents/data/en/application-notes/AN4947.pdf
				 */
				.associativity = 8,
				.line_size = 32
			};
			break;
		case cpuinfo_uarch_cortex_a7:
			/*
			 * Cortex-A7 MPCore Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. You can configure the
			 *   instruction and data caches independently during implementation to sizes of 8KB, 16KB, 32KB, or 64KB.
			 *
			 *   The L1 instruction memory system has the following features:
			 *    - Instruction side cache line length of 32-bytes.
			 *    - 2-way set-associative instruction cache.
			 *
			 *   The L1 data memory system has the following features:
			 *    - Data side cache line length of 64-bytes.
			 *    - 4-way set-associative data cache.
			 *
			 * 7.1. About the L2 Memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, and 1MB.
			 *      - Fixed line length of 64 bytes
			 *      - 8-way set-associative cache structure
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Allwinner A20      |   2   |    32K    |    32K    |   256K    |    [1]    |
			 *  | Allwinner A23      |   2   |    32K    |    32K    |   256K    |    [2]    |
			 *  | Allwinner A31      |   4   |    32K    |    32K    |    1M     |    [3]    |
			 *  | Allwinner A31s     |   4   |    32K    |    32K    |    1M     |    [4]    |
			 *  | Allwinner A33      |   4   |    32K    |    32K    |   512K    |    [5]    |
			 *  | Allwinner A80 Octa | 4(+4) |    32K    |    32K    | 512K(+2M) |    [6]    |
			 *  | Allwinner A81T     |   8   |    32K    |    32K    |    1M     |    [7]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Broadcom BCM2836   |   4   |    32K    |    32K    |    512K   |    [8]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Kirin 920          | 4(+4) |     ?     |     ?     |    512K   |    [9]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://linux-sunxi.org/A20
			 * [2] https://linux-sunxi.org/A23
			 * [3] http://dl.linux-sunxi.org/A31/A3x_release_document/A31/IC/A31%20datasheet%20V1.3%2020131106.pdf
			 * [4] https://github.com/allwinner-zh/documents/blob/master/A31s/A31s_Datasheet_v1.5_20150510.pdf
			 * [5] http://dl.linux-sunxi.org/A33/A33_Datasheet_release1.0.pdf
			 * [6] https://linux-sunxi.org/images/1/10/A80_Datasheet_Revision_1.0_0404.pdf
			 * [7] http://dl.linux-sunxi.org/A83T/A83T_datasheet_Revision_1.1.pdf
			 * [8] https://www.raspberrypi.org/forums/viewtopic.php?t=98428
			 * [9] http://www.gizmochina.com/2014/10/07/hisilicon-kirin-920-tear-down/
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 128 * 1024 * cluster_cores,
				.associativity = 8,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a8:
			/*
			 * Cortex-A8 Technical Reference Manual:
			 * 7.1. About the L1 memory system
			 *    The L1 memory system consists of separate instruction and data caches in a Harvard arrangement.
			 *    The L1 memory system provides the core with:
			 *     - fixed line length of 64 bytes
			 *     - support for 16KB or 32KB caches
			 *     - 4-way set associative cache structure
			 * 8.1. About the L2 memory system
			 *    The L2 memory system is tightly coupled to the L1 data cache and L1 instruction cache.
			 *    The key features of the L2 memory system include:
			 *     - configurable cache size of 0KB, 128KB, 256KB, 512KB, and 1MB
			 *     - fixed line length of 64 bytes
			 *     - 8-way set associative cache structure
			 *
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | Processor model      | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | Exynos 3 Single 3110 |    32K    |    32K    |   512K    |    [1]    |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | TI DM 3730           |    32K    |    32K    |   256K    |    [2]    |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://en.wikichip.org/w/images/0/04/Exynos_3110.pdf
			 * [2] https://www.ti.com/lit/ds/symlink/dm3725.pdf
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
                .size = 512 * 1024,
				.associativity = 8,
				.line_size = 64
			};

			break;
		case cpuinfo_uarch_cortex_a9:
			/*
			 * ARM Cortex‑A9 Technical Reference Manual:
			 * 7.1.1 Memory system
			 *    The Cortex‑A9 processor has separate instruction and data caches.
			 *    The caches have the following features:
			 *     - Both caches are 4-way set-associative.
			 *     - The cache line length is eight words.
			 *     - You can configure the instruction and data caches independently during implementation
			 *       to sizes of 16KB, 32KB, or 64KB.
			 * 8.1.5 Exclusive L2 cache
			 *    The Cortex‑A9 processor can be connected to an L2 cache that supports an exclusive cache mode.
			 *    This mode must be activated both in the Cortex‑A9 processor and in the L2 cache controller.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 4 Dual 4210 |   2   |    32K    |    32K    |    1M     |    [1]    |
			 *  | Exynos 4 Dual 4212 |   2   |    32K    |    32K    |    1M     |    [2]    |
			 *  | Exynos 4 Quad 4412 |   4   |    32K    |    32K    |    1M     |    [3]    |
			 *  | Exynos 4 Quad 4415 |   4   |    32K    |    32K    |    1M     |           |
			 *  | TI OMAP 4430       |   2   |    32K    |    32K    |    1M     |    [4]    |
			 *  | TI OMAP 4460       |   2   |    32K    |    32K    |    1M     |    [5]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Dual_45nm_User_Manaul_Public_REV1.00-0.pdf
			 * [2] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Dual_32nm_User_Manaul_Public_REV100-0.pdf
			 * [3] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Quad_User_Manaul_Public_REV1.00-0.pdf
			 * [4] https://www.hotchips.org/wp-content/uploads/hc_archives/hc21/2_mon/HC21.24.400.ClientProcessors-Epub/HC21.24.421.Witt-OMAP4430.pdf
			 * [5] http://www.anandtech.com/show/5310/samsung-galaxy-nexus-ice-cream-sandwich-review/9
			 */

			/* Use Exynos 4 specs */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 1024 * 1024,
				/* OMAP4460 in Pandaboard ES has 16-way set-associative L2 cache */
				.associativity = 16,
				.line_size = 32
			};
			break;
		case cpuinfo_uarch_cortex_a15:
			/*
			 * 6.1. About the L1 memory system
			 *    The L1 memory system consists of separate instruction and data caches.
			 *    The L1 instruction memory system has the following features:
			 *     - 32KB 2-way set-associative instruction cache.
			 *     - Fixed line length of 64 bytes.
			 *    The L1 data memory system has the following features:
			 *     - 32KB 2-way set-associative data cache.
			 *     - Fixed line length of 64 bytes.
			 * 7.1. About the L2 memory system
			 *    The features of the L2 memory system include:
			 *     - Configurable L2 cache size of 512KB, 1MB, 2MB and 4MB.
			 *     - Fixed line length of 64 bytes.
			 *     - 16-way set-associative cache structure.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 5 Dual 5250 |   2   |    32K    |    32K    |    1M     |    [1]    |
			 *  | Exynos 5 Hexa 5260 | 2(+4) |    32K    |    32K    | 1M(+512K) |    [2]    |
			 *  | Exynos 5 Octa 5410 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5420 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5422 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5430 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5800 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Kirin 920          | 4(+4) |     ?     |     ?     | 2M(+512K) |    [4]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.arndaleboard.org/wiki/downloads/supports/Exynos_5_Dual_User_Manaul_Public_REV1.00.pdf
			 * [2] http://www.yicsystem.com/wp-content/uploads/2014/08/Espresso5260P-Guide-Book.pdf
			 * [3] http://www.anandtech.com/show/6768/samsung-details-exynos-5-octa-architecture-power-at-isscc-13
			 * [4] http://www.gizmochina.com/2014/10/07/hisilicon-kirin-920-tear-down/
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = cluster_cores * 512 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a17:
			/*
			 * ARM Cortex-A17 MPCore Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *    The L1 memory system consists of separate instruction and data caches.
			 *    The size of the instruction cache is implemented as either 32KB or 64KB.
			 *    The size of the data cache is 32KB.
			 *
			 *    The L1 instruction cache has the following features:
			 *     - Instruction side cache line length of 64-bytes.
			 *     - 4-way set-associative instruction cache.
			 *
			 *    The L1 data cache has the following features:
			 *     - Data side cache line length of 64-bytes.
			 *     - 4-way set-associative data cache.
			 *
			 * 7.1. About the L2 Memory system
			 *    An integrated L2 cache:
			 *     - The cache size is implemented as either 256KB, 512KB, 1MB, 2MB, 4MB or 8MB.
			 *     - A fixed line length of 64 bytes.
			 *     - 16-way set-associative cache structure.
			 *
			 *  +------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model  | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +------------------+-------+-----------+-----------+-----------+-----------+
			 *  | MediaTek MT6595  | 4(+4) |    32K    |    32K    | 2M(+512K) |    [1]    |
			 *  +------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://blog.osakana.net/archives/5268
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = cluster_cores * 512 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
#endif /* CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_8A__) */
		case cpuinfo_uarch_cortex_a35:
			/*
			 * ARM Cortex‑A35 Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system includes several power-saving and performance-enhancing features.
			 *   These include separate instruction and data caches, which can be configured
			 *   independently during implementation to sizes of 8KB, 16KB, 32KB, or 64KB.
			 *
			 *   L1 instruction-side memory system
			 *     A dedicated instruction cache that:
			 *      - is virtually indexed and physically tagged.
			 *      - is 2-way set associative.
			 *      - is configurable to be 8KB, 16KB, 32KB, or 64KB.
			 *      - uses a cache line length of 64 bytes.
			 *
			 *   L1 data-side memory system
			 *     A dedicated data cache that:
			 *      - is physically indexed and physically tagged.
			 *      - is 4-way set associative.
			 *      - is configurable to be 8KB, 16KB, 32KB, or 64KB.
			 *      - uses a cache line length of 64 bytes.
			 *
			 * 7.1. About the L2 memory system
			 *   The L2 cache is 8-way set associative.
			 *   Further features of the L2 cache are:
			 *    - Configurable size of 128KB, 256KB, 512KB, and 1MB.
			 *    - Fixed line length of 64 bytes.
			 *    - Physically indexed and tagged.
			 *
			 *  +-----------------+---------+-----------+-----------+-----------+-----------+
			 *  | Processor model |  Cores  | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +-----------------+---------+-----------+-----------+-----------+-----------+
			 *  | MediaTek MT6599 | 4(+4+2) |     ?     |     ?     |     ?     |           |
			 *  +-----------------+---------+-----------+-----------+-----------+-----------+
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024, /* assumption based on low-end Cortex-A53 */
				.associativity = 2,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024, /* assumption based on low-end Cortex-A53 */
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024, /* assumption based on low-end Cortex-A53 */
				.associativity = 8,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a53:
			/*
			 * ARM Cortex-A53 MPCore Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. The implementer configures the
			 *   instruction and data caches independently during implementation, to sizes of 8KB, 16KB, 32KB, or 64KB.
			 *
			 *   The L1 Instruction memory system has the following key features:
			 *    - Instruction side cache line length of 64 bytes.
			 *    - 2-way set associative L1 Instruction cache.
			 *
			 *   The L1 Data memory system has the following features:
			 *    - Data side cache line length of 64 bytes.
			 *    - 4-way set associative L1 Data cache.
			 *
			 * 7.1. About the L2 memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, 1MB and 2MB.
			 *      - Fixed line length of 64 bytes.
			 *      - 16-way set-associative cache structure.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Broadcom BCM2837   |   4   |    16K    |    16K    |    512K   |    [1]    |
			 *  | Exynos 7420        | 4(+4) |    32K    |    32K    |    256K   |  [2, 3]   |
			 *  | Exynos 8890        | 4(+4) |    32K    |    32K    |    256K   |    [4]    |
			 *  | Rochchip RK3368    |  4+4  |    32K    |    32K    | 512K+256K |   sysfs   |
			 *  | MediaTek MT8173C   | 2(+2) |    32K    |    32K    | 512K(+1M) |   sysfs   |
			 *  | Snapdragon 410     |   4   |    32K    |    32K    |    512K   |    [3]    |
			 *  | Snapdragon 630     |  4+4  |    32K    |    32K    |  1M+512K  |   sysfs   |
			 *  | Snapdragon 636     | 4(+4) |  32K+64K  |  32K+64K  |   1M+1M   |   sysfs   |
			 *  | Snapdragon 660     | 4(+4) |  32K+64K  |  32K+64K  |   1M+1M   |   sysfs   |
			 *  | Snapdragon 835     | 4(+4) |  32K+64K  |  32K+64K  |  1M(+2M)  |   sysfs   |
			 *  | Kirin 620          |  4+4  |    32K    |    32K    |    512K   |    [5]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://www.raspberrypi.org/forums/viewtopic.php?f=91&t=145766
			 * [2] http://www.anandtech.com/show/9330/exynos-7420-deep-dive/2
			 * [3] https://www.usenix.org/system/files/conference/usenixsecurity16/sec16_paper_lipp.pdf
			 * [4] http://www.boardset.com/products/products_v8890.php
			 * [5] http://mirror.lemaker.org/Hi6220V100_Multi-Mode_Application_Processor_Function_Description.pdf
			 */
			if (midr_is_qualcomm_cortex_a53_silver(midr)) {
				/* Qualcomm-modified Cortex-A53 in Snapdragon 630/660/835 */
				uint32_t l2_size = 512 * 1024;
				*l1i = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 2,
					.line_size = 64
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64
				};
				*l2 = (struct cpuinfo_cache) {
					.size = l2_size,
					.associativity = 16,
					.line_size = 64
				};
			} else {
				/* Standard Cortex-A53 */

				/* Use conservative values by default */
				uint32_t l1_size = 16 * 1024;
				uint32_t l2_size = 256 * 1024;
				*l1i = (struct cpuinfo_cache) {
					.size = l1_size,
					.associativity = 2,
					.line_size = 64
				};
				*l1d = (struct cpuinfo_cache) {
					.size = l1_size,
					.associativity = 4,
					.line_size = 64
				};
				*l2 = (struct cpuinfo_cache) {
					.size = l2_size,
					.associativity = 16,
					.line_size = 64
				};
			}
			break;
		case cpuinfo_uarch_cortex_a55r0:
		case cpuinfo_uarch_cortex_a55:
			/*
			 * ARM Cortex-A55 Core Technical Reference Manual
			 * A6.1. About the L1 memory system
			 *   The Cortex®-A55 core's L1 memory system enhances core performance and power efficiency.
			 *   It consists of separate instruction and data caches. You can configure instruction and data caches
			 *   independently during implementation to sizes of 16KB, 32KB, or 64KB.
			 *
			 *   L1 instruction-side memory system
			 *   The L1 instruction-side memory system provides an instruction stream to the DPU. Its key features are:
			 *    - 64-byte instruction side cache line length.
			 *    - 4-way set associative L1 instruction cache.
			 *
			 *   L1 data-side memory system
			 *    - 64-byte data side cache line length.
			 *    - 4-way set associative L1 data cache.
			 *
			 * A7.1 About the L2 memory system
			 *   The Cortex-A55 L2 memory system is required to interface the Cortex-A55 cores to the L3 memory system.
			 *   The L2 memory subsystem consists of:
			 *    - An optional 4-way, set-associative L2 cache with a configurable size of 64KB, 128KB or 256KB. Cache
			 *      lines have a fixed length of 64 bytes.
			 *
			 *   The main features of the L2 memory system are:
			 *    - Strictly exclusive with L1 data cache.
			 *    - Pseudo-inclusive with L1 instruction cache.
			 *    - Private per-core unified L2 cache.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | L3 cache | Reference  |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Snapdragon 845     | 4(+4) |    32K    |    32K    |    128K   |    2M    | [1], sysfs |
			 *  | Exynos 9810        | 4(+4) |     ?     |     ?     |    None   |   512K   |     [2]    |
			 *  | Kirin 980          | 4(+4) |    32K    |    32K    |    128K   |    4M    |     [3]    |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *
			 * [1] https://www.anandtech.com/show/12114/qualcomm-announces-snapdragon-845-soc
			 * [2] https://www.anandtech.com/show/12478/exynos-9810-handson-awkward-first-results
			 * [3] https://en.wikichip.org/wiki/hisilicon/kirin/980
			 */
			if (midr_is_qualcomm_cortex_a55_silver(midr)) {
				/* Qualcomm-modified Cortex-A55 in Snapdragon 670 / 710 / 845 */
				uint32_t l3_size = 1024 * 1024;
				*l1i = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64,
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64,
				};
				*l2 = (struct cpuinfo_cache) {
					.size = 128 * 1024,
					.associativity = 4,
					.line_size = 64,
				};
				*l3 = (struct cpuinfo_cache) {
					.size = l3_size,
					.associativity = 16,
					.line_size = 64,
				};
			} else {
				/* Standard Cortex-A55 */

				*l1i = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64,
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64,
				};

				uint32_t l3_size = 1024 * 1024;
                *l2 = (struct cpuinfo_cache) {
                    .size = 128 * 1024,
                    .associativity = 4,
                    .line_size = 64,
                };
                *l3 = (struct cpuinfo_cache) {
                    .size = l3_size,
                    /* DynamIQ */
                    .associativity = 16,
                    .line_size = 64,
                };

			}
			break;
		case cpuinfo_uarch_cortex_a57:
			/*
			 * ARM Cortex-A57 MPCore Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches.
			 *
			 *   The L1 instruction memory system has the following features:
			 *    - 48KB 3-way set-associative instruction cache.
			 *    - Fixed line length of 64 bytes.
			 *
			 *   The L1 data memory system has the following features:
			 *    - 32KB 2-way set-associative data cache.
			 *    - Fixed line length of 64 bytes.
			 *
			 * 7.1 About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, and 2MB.
			 *    - Fixed line length of 64 bytes.
			 *    - 16-way set-associative cache structure.
			 *    - Inclusion property with L1 data caches.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Snapdragon 810     | 4(+4) |    32K    |    48K    |    2M     |    [1]    |
			 *  | Exynos 7420        | 4(+4) |    32K    |    48K    |    2M     |    [2]    |
			 *  | Jetson TX1         |   4   |    32K    |    48K    |    2M     |    [3]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/9837/snapdragon-820-preview
			 * [2] http://www.anandtech.com/show/9330/exynos-7420-deep-dive/2
			 * [3] https://devblogs.nvidia.com/parallelforall/jetson-tx2-delivers-twice-intelligence-edge/
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 48 * 1024,
				.associativity = 3,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = cluster_cores * 512 * 1024,
				.associativity = 16,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE
			};
			break;
		case cpuinfo_uarch_cortex_a65:
		{
			/*
			 * ARM Cortex‑A65 Core Technical Reference Manual
			 * A6.1. About the L1 memory system
			 *   The L1 memory system enhances the performance and power efficiency in the Cortex‑A65 core.
			 *   It consists of separate instruction and data caches. You can configure instruction and data caches
			 *   independently during implementation to sizes of 32KB or 64KB.
			 *
			 *   L1 instruction-side memory system
			 *   The L1 instruction-side memory system provides an instruction stream to the DPU. Its key features are:
			 *    - 64-byte instruction side cache line length.
			 *    - 4-way set associative L1 instruction cache.
			 *
			 *   L1 data-side memory system
			 *    - 64-byte data side cache line length.
			 *    - 4-way set associative L1 data cache.
			 *
			 * A7.1 About the L2 memory system
			 *   The Cortex‑A65 L2 memory system is required to interface the Cortex‑A65 cores to the L3 memory system.
			 *   The L2 memory subsystem consists of:
			 *    - An optional 4-way, set-associative L2 cache with a configurable size of 64KB, 128KB, or 256KB.
			 *      Cache lines have a fixed length of 64 bytes.
			 *
			 *   The main features of the L2 memory system are:
			 *    - Strictly exclusive with L1 data cache.
			 *    - Pseudo-inclusive with L1 instruction cache.
			 *    - Private per-core unified L2 cache.
			 */
			const uint32_t l1_size = 32 * 1024;
			const uint32_t l2_size = 128 * 1024;
			const uint32_t l3_size = 512 * 1024;
			*l1i = (struct cpuinfo_cache) {
				.size = l1_size,
				.associativity = 4,
				.line_size = 64,
			};
			*l1d = (struct cpuinfo_cache) {
				.size = l1_size,
				.associativity = 4,
				.line_size = 64,
			};
			*l2 = (struct cpuinfo_cache) {
				.size = l2_size,
				.associativity = 4,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE
			};
			*l3 = (struct cpuinfo_cache) {
				.size = l3_size,
				/* DynamIQ */
				.associativity = 16,
				.line_size = 64,
			};
			break;
		}
		case cpuinfo_uarch_cortex_a72:
		{
			/*
			 * ARM Cortex-A72 MPCore Processor Technical Reference Manual
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches.
			 *
			 *   The L1 instruction memory system has the following features:
			 *    - 48KB 3-way set-associative instruction cache.
			 *    - Fixed line length of 64 bytes.
			 *
			 *   The L1 data memory system has the following features:
			 *    - 32KB 2-way set-associative data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *
			 * 7.1 About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, 2MB and 4MB.
			 *    - Fixed line length of 64 bytes.
			 *    - Banked pipeline structures.
			 *    - Inclusion property with L1 data caches.
			 *    - 16-way set-associative cache structure.
			 *
			 *  +---------------------+---------+-----------+-----------+------------+-----------+
			 *  | Processor model     | Cores   | L1D cache | L1I cache | L2 cache   | Reference |
			 *  +---------------------+---------+-----------+-----------+------------+-----------+
			 *  | Snapdragon 650      |  2(+4)  | 32K(+32K) | 48K(+32K) |  1M(+512K) |    [1]    |
			 *  | Snapdragon 652      |  4(+4)  | 32K(+32K) | 48K(+32K) |  1M(+512K) |    [2]    |
			 *  | Snapdragon 653      |  4(+4)  | 32K(+32K) | 48K(+32K) |  1M(+512K) |    [3]    |
			 *  | HiSilicon Kirin 950 |  4(+4)  |  32K+32K  |  48K+32K  |     ?      |           |
			 *  | HiSilicon Kirin 955 |  4(+4)  |  32K+32K  |  48K+32K  |     ?      |           |
			 *  | MediaTek MT8173C    |  2(+2)  | 32K(+32K) | 48K(+32K) |  1M(+512K) |   sysfs   |
			 *  | MediaTek Helio X20  | 2(+4+4) |     ?     |     ?     |     ?      |           |
			 *  | MediaTek Helio X23  | 2(+4+4) |     ?     |     ?     |     ?      |           |
			 *  | MediaTek Helio X25  | 2(+4+4) |     ?     |     ?     |     ?      |           |
			 *  | MediaTek Helio X27  | 2(+4+4) |     ?     |     ?     |     ?      |           |
			 *  | Broadcom BCM2711    |    4    |    32K    |    48K    |     1M     |    [4]    |
			 *  +---------------------+---------+-----------+-----------+------------+-----------+
			 *
			 * [1] http://pdadb.net/index.php?m=processor&id=578&c=qualcomm_snapdragon_618_msm8956__snapdragon_650
			 * [2] http://pdadb.net/index.php?m=processor&id=667&c=qualcomm_snapdragon_620_apq8076__snapdragon_652
			 * [3] http://pdadb.net/index.php?m=processor&id=692&c=qualcomm_snapdragon_653_msm8976sg__msm8976_pro
			 * [4] https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2711/README.md
			 */
			uint32_t l2_size;
			l2_size = 1024 * 1024;
			*l1i = (struct cpuinfo_cache) {
				.size = 48 * 1024,
				.associativity = 3,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = l2_size,
				.associativity = 16,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE
			};
			break;
		}
		case cpuinfo_uarch_cortex_a73:
		{
			/*
			 * ARM Cortex‑A73 MPCore Processor Technical Reference Manual
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches.
			 *   The size of the instruction cache is 64KB.
			 *   The size of the data cache is configurable to either 32KB or 64KB.
			 *
			 *   The L1 instruction memory system has the following key features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), four-way set-associative instruction cache.
			 *    - Fixed cache line length of 64 bytes.
			 *
			 *   The L1 data memory system has the following features:
			 *    - ...the data cache behaves like an eight-way set associative PIPT cache (for 32KB configurations)
			 *      and a 16-way set associative PIPT cache (for 64KB configurations).
			 *    - Fixed cache line length of 64 bytes.
			 *
			 * 7.1 About the L2 memory system
			 *   The L2 memory system consists of:
			 *    - A tightly-integrated L2 cache with:
			 *      - A configurable size of 256KB, 512KB, 1MB, 2MB, 4MB, or 8MB.
			 *      - A 16-way, set-associative structure.
			 *      - A fixed line length of 64 bytes.
			 *
			 * The ARM Cortex A73 - Artemis Unveiled [1]
			 *   "ARM still envisions that most vendors will choose to use configurations of 1 to
			 *    2MB in consumer products. The L2 cache is inclusive of the L1 cache. "
			 *
			 *  +---------------------+---------+-----------+-----------+-----------+-----------+
			 *  | Processor model     | Cores   | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +---------------------+---------+-----------+-----------+-----------+-----------+
			 *  | HiSilicon Kirin 960 |  4(+4)  |  64K+32K  |  64K+32K  |     ?     |    [2]    |
			 *  | MediaTek Helio X30  | 2(+4+4) |     ?     |  64K+ ?   |     ?     |           |
			 *  | Snapdragon 636      |  4(+4)  | 64K(+32K) | 64K(+32K) |  1M(+1M)  |   sysfs   |
			 *  | Snapdragon 660      |  4(+4)  |  64K+32K  |  64K+32K  |  1M(+1M)  |    [3]    |
			 *  | Snapdragon 835      |  4(+4)  |  64K+32K  |  64K+32K  |  2M(+1M)  |   sysfs   |
			 *  +---------------------+---------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/10347/arm-cortex-a73-artemis-unveiled/2
			 * [2] http://www.anandtech.com/show/11088/hisilicon-kirin-960-performance-and-power/3
			 * [3] https://arstechnica.com/gadgets/2017/05/qualcomms-snapdragon-660-and-630-bring-more-high-end-features-to-midrange-chips/
			 */
			uint32_t l1d_size = 32 * 1024;
			uint32_t l2_size = 512 * 1024;
            switch (midr) {
                case UINT32_C(0x51AF8001): /* Kryo 280 Gold */
                    l1d_size = 64 * 1024;
                    l2_size = 2 * 1024 * 1024;
                    break;
                case UINT32_C(0x51AF8002): /* Kryo 260 Gold */
                    l1d_size = 64 * 1024;
                    l2_size = 1 * 1024 * 1024;
                    break;
            }

			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = l1d_size,
				.associativity = (l1d_size >> 12),
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = l2_size,
				.associativity = 16,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE
			};
			break;
		}
		case cpuinfo_uarch_cortex_a75:
		{
			/*
			 * ARM Cortex-A75 Core Technical Reference Manual
			 * A6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. Both have a fixed size of 64KB.
			 *
			 * A6.1.1 L1 instruction-side memory system
			 *   The L1 instruction memory system has the following key features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), four-way set-associative instruction cache.
			 *    - Fixed cache line length of 64 bytes.
			 *
			 * A6.1.2 L1 data-side memory system
			 *   The L1 data memory system has the following features:
			 *    - Physically Indexed, Physically Tagged (PIPT), 16-way set-associative L1 data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *    - Pseudo-random cache replacement policy.
			 *
			 * A7.1 About the L2 memory system
			 *   The L2 memory subsystem consist of:
			 *    - An 8-way set associative L2 cache with a configurable size of 256KB or 512KB.
			 *      Cache lines have a fixed length of 64 bytes.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | L3 cache | Reference  |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Snapdragon 845     | 4(+4) |    64K    |    64K    |    256K   |    2M    | [1], sysfs |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *
			 * [1] https://www.anandtech.com/show/12114/qualcomm-announces-snapdragon-845-soc
			 */
			uint32_t l3_size = 1024 * 1024;
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024,
				.associativity = 8,
				.line_size = 64
			};
			*l3 = (struct cpuinfo_cache) {
				.size = l3_size,
				.associativity = 16,
				.line_size = 64
			};
			break;
		}
		case cpuinfo_uarch_cortex_a76:
		{
			/*
			 * ARM Cortex-A76 Core Technical Reference Manual
			 * A6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. Both have a fixed size of 64KB.
			 *
			 * A6.1.1 L1 instruction-side memory system
			 *   The L1 instruction memory system has the following key features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), which behaves as a Physically Indexed,
			 *      Physically Tagged (PIPT) 4-way set-associative L1 data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *
			 * A6.1.2 L1 data-side memory system
			 *   The L1 data memory system has the following features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), which behaves as a Physically Indexed,
			 *      Physically Tagged (PIPT) 4-way set-associative L1 data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *    - Pseudo-LRU cache replacement policy.
			 *
			 * A7.1 About the L2 memory system
			 *   The L2 memory subsystem consist of:
			 *    - An 8-way set associative L2 cache with a configurable size of 128KB, 256KB or 512KB.
			 *      Cache lines have a fixed length of 64 bytes.
			 *    - Strictly inclusive with L1 data cache. Weakly inclusive with L1 instruction cache.
			 *    - Dynamic biased replacement policy.
			 *    - Modified Exclusive Shared Invalid (MESI) coherency.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | L3 cache | Reference  |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Kirin 980          | 4(+4) |    64K    |    64K    |    512K   |    4M    |  [1], [2]  |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *
			 * [1] https://www.anandtech.com/show/13298/hisilicon-announces-the-kirin-980-first-a76-g76-on-7nm
			 * [2] https://en.wikichip.org/wiki/hisilicon/kirin/980
			 */
			uint32_t l2_size = 256 * 1024;
			uint32_t l3_size = 1024 * 1024;
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64,
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64,
			};
			*l2 = (struct cpuinfo_cache) {
				.size = l2_size,
				.associativity = 8,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE,
			};
			*l3 = (struct cpuinfo_cache) {
				.size = l3_size,
				.associativity = 16,
				.line_size = 64,
			};
			break;
		}
		case cpuinfo_uarch_cortex_a77:
		{
			/*
			 * ARM Cortex-A77 Core Technical Reference Manual
			 * A6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. Both have a fixed size of 64KB.
			 *
			 * A6.1.1 L1 instruction-side memory system
			 *   The L1 instruction memory system has the following key features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), which behaves as a Physically Indexed,
			 *      Physically Tagged (PIPT) 4-way set-associative L1 data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *
			 * A6.1.2 L1 data-side memory system
			 *   The L1 data memory system has the following features:
			 *    - Virtually Indexed, Physically Tagged (VIPT), which behaves as a Physically Indexed,
			 *      Physically Tagged (PIPT) 4-way set-associative L1 data cache.
			 *    - Fixed cache line length of 64 bytes.
			 *    - Pseudo-LRU cache replacement policy.
			 *
			 * A7.1 About the L2 memory system
			 *   The L2 memory subsystem consist of:
			 *    - An 8-way set associative L2 cache with a configurable size of 128KB, 256KB or 512KB. Cache lines
			 *      have a fixed length of 64 bytes.
			 *    - Strictly inclusive with L1 data cache. Weakly inclusive with L1 instruction cache.
			 */
			const uint32_t l2_size = 256 * 1024;
			const uint32_t l3_size = 1024 * 1024;
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64,
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64,
			};
			*l2 = (struct cpuinfo_cache) {
				.size = l2_size,
				.associativity = 8,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE,
			};
			*l3 = (struct cpuinfo_cache) {
				.size = l3_size,
				.associativity = 16,
				.line_size = 64,
			};
			break;
		}
#if CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_8A__)
		case cpuinfo_uarch_scorpion:
			/*
			 * - "The CPU includes 32KB instruction and data caches as
			 *    well as a complete memory-management unit (MMU) suitable
			 *    for high-level operating systems. The CPU also has
			 *    256KB of SRAM that can be allocated in 64KB increments
			 *    to level-two (L2) cache or tightly coupled memory (TCM)." [1]
			 *    We interpret it as L2 cache being 4-way set-associative on single-core Scorpion.
			 * - L1 Data Cache = 32 KB. 32 B/line. [2]
             * - L2 Cache = 256 KB. 128 B/line. [2]
			 * - 256 KB (single-core) or 512 KB (dual-core) L2 cache [3]
			 * - Single or dual-core configuration [3]
			 * - For L1 cache assume the same associativity as Krait
			 *
			 * [1] https://www.qualcomm.com/media/documents/files/linley-report-on-dual-core-snapdragon.pdf
			 * [2] http://www.7-cpu.com/cpu/Snapdragon.html
			 * [3] https://en.wikipedia.org/wiki/Scorpion_(CPU)
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = cluster_cores * 256 * 1024,
				.associativity = 4,
				.line_size = 128
			};
			break;
		case cpuinfo_uarch_krait:
			/*
			 * - L0 Data cache = 4 KB. 64 B/line, direct mapped [1]
			 * - L0 Instruction cache = 4 KB. [1]
			 * - L1 Data cache = 16 KB. 64 B/line, 4-way [1]
			 * - L1 Instruction cache = 16 KB, 4-way [1]
			 * - L2 Cache = 1 MB, 128 B/line, 8-way. Each core has fast access only to 512 KB of L2 cache. [1]
			 * - L2	= 1MB (dual core) or 2MB (quad core), 8-way set associative [2]
			 *
			 * [1] http://www.7-cpu.com/cpu/Krait.html
			 * [2] http://www.anandtech.com/show/4940/qualcomm-new-snapdragon-s4-msm8960-krait-architecture/2
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 64 /* assume same as L1D */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = cluster_cores * 512 * 1024,
				.associativity = 8,
				.line_size = 128
			};
			break;
#endif /* CPUINFO_ARCH_ARM && !defined(__ARM_ARCH_8A__) */
		case cpuinfo_uarch_kryo:
			/*
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *  | Snapdragon 820  |  2+2  |    24K    |    32K    |  1M+512K  |   [1, 2]  |
			 *  | Snapdragon 821  |  2+2  |     ?     |     ?     |  1M+512K  |    [1]    |
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/9837/snapdragon-820-preview/2
			 * [2] https://www.inforcecomputing.com/public_docs/Inforce6601/Inforce_6601_Micro-SOM_FAQs_04-2016-1.pdf
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 24 * 1024,
				.associativity = 3,
				.line_size = 64
			};
			if (midr_is_kryo_silver(midr)) {
				/* Kryo "Silver" */
				*l2 = (struct cpuinfo_cache) {
					.size = 512 * 1024,
					.associativity = 8,
					.line_size = 128
				};
			} else {
				/* Kryo "Gold" */
				*l2 = (struct cpuinfo_cache) {
					.size = 1024 * 1024,
					.associativity = 8,
					.line_size = 128
				};
			}
			break;
		case cpuinfo_uarch_denver:
		case cpuinfo_uarch_denver2:
			/*
			 * The Denver chip includes a 128KB, 4-way level 1 instruction cache, a 64KB, 4-way level 2 data cache,
			 * and a 2MB, 16-way level 2 cache, all of which can service both cores. [1]
			 *
			 * All the caches have 64-byte lines. [2]
			 *
			 * [1] http://www.pcworld.com/article/2463900/nvidia-reveals-pc-like-performance-for-denver-tegra-k1.html
			 * [2] http://linleygroup.com/newsletters/newsletter_detail.php?num=5205&year=2014
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 128 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 2 * 1024 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_exynos_m1:
		case cpuinfo_uarch_exynos_m2:
			/*
			 * - "Moving past branch prediction we can see some elements of how the cache is set up for the L1 I$,
			 *    namely 64 KB split into four sets with 128-byte line sizes for 128 cache lines per set" [1]
			 * - "For loads and stores, a 32 KB, 8-way set associative cache with 64 byte line size is used" [1]
			 * - "The L2 cache here is 2MB shared across all cores split into 16 sets. This memory is also split
			 *    into 4 banks and has a 22 cycle latency" [1]
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 8 Octa 8890 | 4(+4) |    64K    |    32K    |    2M     |    [1]    |
			 *  | Exynos 8 Octa 8895 | 4(+4) |    64K    |    32K    |    2M     |    [2]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/10590/hot-chips-2016-exynos-m1-architecture-disclosed
			 * [2] https://www.extremetech.com/mobile/244949-samsungs-exynos-8895-features-custom-cpu-cores-first-10nm-chip-market
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 128
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 8,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 2 * 1024 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_exynos_m3:
			/*
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | L3 cache | Reference  |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *  | Exynos 9810        | 4(+4) |    64K    |     ?     |    512K   |    4M    |     [1]    |
			 *  +--------------------+-------+-----------+-----------+-----------+----------+------------+
			 *
			 * [1] https://www.anandtech.com/show/12478/exynos-9810-handson-awkward-first-results
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024 /* assume same as in Exynos M1/M2 cores */,
				.associativity = 4 /* assume same as in Exynos M1/M2 cores */,
				.line_size = 128 /* assume same as in Exynos M1/M2 cores */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 8 /* assume same as in Exynos M1/M2 cores */,
				.line_size = 64 /* assume same as in Exynos M1/M2 cores */,
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 512 * 1024,
				.associativity = 16 /* assume same as in Exynos M1/M2 cores */,
				.line_size = 64 /* assume same as in Exynos M1/M2 cores */,
			};
			*l3 = (struct cpuinfo_cache) {
				.size = 4 * 1024 * 1024,
				.associativity = 16 /* assume DynamIQ cache */,
				.line_size = 64 /* assume DynamIQ cache */,
			};
			break;
#if CPUINFO_ARCH_ARM64 && !defined(__ANDROID__)
		case cpuinfo_uarch_thunderx:
			/*
			 * "78K-Icache and 32K-D cache per core, 16 MB shared L2 cache" [1]
			 *
			 * [1] https://www.cavium.com/pdfFiles/ThunderX_CP_PB_Rev1.pdf
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 78 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 64 /* assumption */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 64 /* assumption */
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 16 * 1024 * 1024,
				.associativity = 8 /* assumption */,
				.line_size = 64 /* assumption */
			};
			break;
		case cpuinfo_uarch_taishan_v110:
			/*
			 * It features private 64 KiB L1 instruction and data caches as well as 512 KiB of private L2. [1]
			 *
			 *  +------------------+-------+-----------+-----------+-----------+----------+-----------+
			 *  | Processor model  | Cores | L1D cache | L1I cache | L2 cache  | L3 cache | Reference |
			 *  +------------------+-------+-----------+-----------+-----------+----------+-----------+
			 *  | Kunpeng 920-3226 |  32   |    64K    |    64K    |    512K   |    32M   |     [2]   |
			 *  +------------------+-------+-----------+-----------+-----------+----------+-----------+
			 *  | Kunpeng 920-4826 |  48   |    64K    |    64K    |    512K   |    48M   |     [3]   |
			 *  +------------------+-------+-----------+-----------+-----------+----------+-----------+
			 *  | Kunpeng 920-6426 |  64   |    64K    |    64K    |    512K   |    64M   |     [4]   |
			 *  +------------------+-------+-----------+-----------+-----------+----------+-----------+
			 *
			 * [1] https://en.wikichip.org/wiki/hisilicon/microarchitectures/taishan_v110
			 * [2] https://en.wikichip.org/wiki/hisilicon/kunpeng/920-3226
			 * [3] https://en.wikichip.org/wiki/hisilicon/kunpeng/920-4826
			 * [4] https://en.wikichip.org/wiki/hisilicon/kunpeng/920-6426
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 128 /* assumption */,
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 128 /* assumption */,
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 512 * 1024,
				.associativity = 8 /* assumption */,
				.line_size = 128 /* assumption */,
				.flags = CPUINFO_CACHE_INCLUSIVE /* assumption */,
			};
			*l3 = (struct cpuinfo_cache) {
				.size = cluster_cores * 1024 * 1024,
				.associativity = 16 /* assumption */,
				.line_size = 128 /* assumption */,
			};
			break;
#endif
		case cpuinfo_uarch_cortex_a12:
		case cpuinfo_uarch_cortex_a32:
		default:
			cpuinfo_log_warning("target uarch not recognized; using generic cache parameters");
			/* Follow OpenBLAS */
            *l1i = (struct cpuinfo_cache) {
                .size = 32 * 1024,
                .associativity = 4,
                .line_size = 64
            };
            *l1d = (struct cpuinfo_cache) {
                .size = 32 * 1024,
                .associativity = 4,
                .line_size = 64
            };
            *l2 = (struct cpuinfo_cache) {
                .size = cluster_cores * 256 * 1024,
                .associativity = 8,
                .line_size = 64
            };
			break;
	}
	l1i->sets = l1i->size / (l1i->associativity * l1i->line_size);
	l1i->partitions = 1;
	l1d->sets = l1d->size / (l1d->associativity * l1d->line_size);
	l1d->partitions = 1;
	if (l2->size != 0) {
		l2->sets = l2->size / (l2->associativity * l2->line_size);
		l2->partitions = 1;
		if (l3->size != 0) {
			l3->sets = l3->size / (l3->associativity * l3->line_size);
			l3->partitions = 1;
		}
	}
}

uint32_t cpuinfo_arm_compute_max_cache_size(const struct cpuinfo_processor* processor) {
	/*
	 * There is no precise way to detect cache size on ARM/ARM64, and cache size reported by cpuinfo
	 * may underestimate the actual cache size. Thus, we use microarchitecture-specific maximum.
	 */
	switch (processor->core->uarch) {
		case cpuinfo_uarch_xscale:
		case cpuinfo_uarch_arm11:
		case cpuinfo_uarch_scorpion:
		case cpuinfo_uarch_krait:
		case cpuinfo_uarch_kryo:
		case cpuinfo_uarch_exynos_m1:
		case cpuinfo_uarch_exynos_m2:
		case cpuinfo_uarch_exynos_m3:
			/* cpuinfo-detected cache size always correct */
			return cpuinfo_compute_max_cache_size(processor);
		case cpuinfo_uarch_cortex_a5:
			/* Max observed (NXP Vybrid SoC) */
			return 512 * 1024;
		case cpuinfo_uarch_cortex_a7:
			/*
			 * Cortex-A7 MPCore Technical Reference Manual:
			 * 7.1. About the L2 Memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, and 1MB.
			 */
			return 1024 * 1024;
		case cpuinfo_uarch_cortex_a8:
			/*
			 * Cortex-A8 Technical Reference Manual:
			 * 8.1. About the L2 memory system
			 *   The key features of the L2 memory system include:
			 *    - configurable cache size of 0KB, 128KB, 256KB, 512KB, and 1MB
			 */
			return 1024 * 1024;
		case cpuinfo_uarch_cortex_a9:
			/* Max observed (e.g. Exynos 4212) */
			return 1024 * 1024;
		case cpuinfo_uarch_cortex_a12:
		case cpuinfo_uarch_cortex_a17:
			/*
			 * ARM Cortex-A17 MPCore Processor Technical Reference Manual:
			 * 7.1. About the L2 Memory system
			 *   The key features of the L2 memory system include:
			 *    - An integrated L2 cache:
			 *      - The cache size is implemented as either 256KB, 512KB, 1MB, 2MB, 4MB or 8MB.
			 */
			return 8 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a15:
			/*
			 * ARM Cortex-A15 MPCore Processor Technical Reference Manual:
			 * 7.1. About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, 2MB and 4MB.
			 */
			return 4 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a35:
			/*
			 * ARM Cortex‑A35 Processor Technical Reference Manual:
			 * 7.1 About the L2 memory system
			 *   L2 cache
			 *    - Further features of the L2 cache are:
			 *      - Configurable size of 128KB, 256KB, 512KB, and 1MB.
			 */
			return 1024 * 1024;
		case cpuinfo_uarch_cortex_a53:
			/*
			 * ARM Cortex-A53 MPCore Processor Technical Reference Manual:
			 * 7.1. About the L2 memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, 1MB and 2MB.
			 */
			return 2 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a57:
			/*
			 * ARM Cortex-A57 MPCore Processor Technical Reference Manual:
			 * 7.1 About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, and 2MB.
			 */
			return 2 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a72:
			/*
			 * ARM Cortex-A72 MPCore Processor Technical Reference Manual:
			 * 7.1 About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, 2MB and 4MB.
			 */
			return 4 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a73:
			/*
			 * ARM Cortex‑A73 MPCore Processor Technical Reference Manual
			 * 7.1 About the L2 memory system
			 *   The L2 memory system consists of:
			 *    - A tightly-integrated L2 cache with:
			 *       - A configurable size of 256KB, 512KB, 1MB, 2MB, 4MB, or 8MB.
			 */
			return 8 * 1024 * 1024;
		case cpuinfo_uarch_cortex_a55:
		case cpuinfo_uarch_cortex_a75:
		case cpuinfo_uarch_cortex_a76:
		case cpuinfo_uarch_exynos_m4:
		default:
			/*
			 * ARM DynamIQ Shared Unit Technical Reference Manual
			 * 1.3 Implementation options
			 *   L3_CACHE_SIZE
			 *    - 256KB
			 *    - 512KB
			 *    - 1024KB
			 *    - 1536KB
			 *    - 2048KB
			 *    - 3072KB
			 *    - 4096KB
			 */
			return 4 * 1024 * 1024;
	}
}

#endif
