/* Copyright 2025 Cix Technology Group Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _CIX_NOE_API_H_
#define _CIX_NOE_API_H_

#include <stdint.h>
#include <stdbool.h>

#define NPU_DEVFREQ_DT_PATH   "/sys/class/devfreq/14260000.aipu/userspace/set_freq"
#define NPU_DEVFREQ_ACPI_PATH "/sys/class/devfreq/CIXH4000:00/userspace/set_freq"

typedef struct noe_context context_handler_t;  // Opaque context bridge typedef.

// ctx points to memory allocated by the application where UMD stores the opaque context handle.
typedef struct noe_context* noe_context_t;

typedef uint64_t noe_graph_id_t;
typedef uint64_t noe_job_id_t;

typedef uint64_t env_type_t;
typedef uint64_t job_attr_type_t;

/**
 * @struct noe_driver_version
 *
 * @brief Get version numbers of UMD and KMD.
 *
 * @note The version numbers are determined at compile time. Currently both
 *       versions are identical. The maximum buffer size is 16 bytes.
 */
typedef struct noe_driver_version
{
    char umd_version[16]; /**< the buffer for storing UMD version string */
    char kmd_version[16]; /**< the buffer for storing KMD version string */
} noe_driver_version_t;

/**
 * @struct noe_cix_buildversion
 *
 * @brief Get build version number of the model binary.
 *
 * @note The version number is determined by the NN compiler.
 */
typedef struct noe_cix_buildversion
{
    uint64_t graph_id; /**< the graph id to be searched */
    uint32_t noe_graph_buildversion; /**< the build version of searched graph */
} noe_cix_buildversion_t;

/**
 * @brief Result codes returned by NOE APIs.
 */
typedef enum {
    NOE_STATUS_SUCCESS                    = 0x0,
    NOE_STATUS_ERROR_NULL_PTR             = 0x1,
    NOE_STATUS_ERROR_INVALID_CTX          = 0x2,
    NOE_STATUS_ERROR_OPEN_DEV_FAIL        = 0x3,
    NOE_STATUS_ERROR_DEV_ABNORMAL         = 0x4,
    NOE_STATUS_ERROR_DEINIT_FAIL          = 0x5,
    NOE_STATUS_ERROR_INVALID_CONFIG       = 0x6,
    NOE_STATUS_ERROR_UNKNOWN_BIN          = 0x7,
    NOE_STATUS_ERROR_GVERSION_UNSUPPORTED = 0x8,
    NOE_STATUS_ERROR_INVALID_GBIN         = 0x9,
    NOE_STATUS_ERROR_TARGET_NOT_FOUND     = 0xA,
    NOE_STATUS_ERROR_INVALID_GRAPH_ID     = 0xB,
    NOE_STATUS_ERROR_OPEN_FILE_FAIL       = 0xC,
    NOE_STATUS_ERROR_MAP_FILE_FAIL        = 0xD,
    NOE_STATUS_ERROR_READ_FILE_FAIL       = 0xE,
    NOE_STATUS_ERROR_WRITE_FILE_FAIL      = 0xF,
    NOE_STATUS_ERROR_INVALID_JOB_ID       = 0x10,
    NOE_STATUS_ERROR_JOB_EXCEPTION        = 0x11,
    NOE_STATUS_ERROR_JOB_TIMEOUT          = 0x12,
    NOE_STATUS_ERROR_OP_NOT_SUPPORTED     = 0x13,
    NOE_STATUS_ERROR_INVALID_OP           = 0x14,
    NOE_STATUS_ERROR_INVALID_SIZE         = 0x15,
    NOE_STATUS_ERROR_BUF_ALLOC_FAIL       = 0x16,
    NOE_STATUS_ERROR_BUF_FREE_FAIL        = 0x17,
    NOE_STATUS_ERROR_INVALID_CORE_ID      = 0x18,
    NOE_STATUS_ERROR_RESERVE_SRAM_FAIL    = 0x19,
    NOE_STATUS_ERROR_INVALID_TENSOR_ID    = 0x1A,
    NOE_STATUS_ERROR_INVALID_CLUSTER_ID   = 0x1B,
    NOE_STATUS_ERROR_INVALID_PARTITION_ID = 0x1C,
    NOE_STATUS_ERROR_PRINTF_FAIL          = 0x1D,
    NOE_STATUS_ERROR_INVALID_TENSOR_TYPE  = 0x1E,
    NOE_STATUS_ERROR_INVALID_GM           = 0x1F,
    NOE_STATUS_ERROR_INVALID_SEGMMU       = 0x20,
    NOE_STATUS_ERROR_INVALID_QOS          = 0x21,
    NOE_STATUS_ERROR_INVALID_TENSOR_CNT   = 0x22,
    NOE_STATUS_ERROR_TIMEOUT              = 0x23,
    NOE_STATUS_ERROR_NO_BATCH_QUEUE       = 0x24,
    NOE_STATUS_ERROR_MARK_SHARED_TENSOR   = 0x25,
    NOE_STATUS_ERROR_SET_SHARED_TENSOR    = 0x26,
    NOE_STATUS_ERROR_DMABUF_SHARED_IO     = 0x27,
    NOE_STATUS_ERROR_GET_SHAPE_FAILED     = 0x28,
    NOE_STATUS_ERROR_SET_SHAPE_FAILED     = 0x29,
    NOE_STATUS_ERROR_NOT_CONFIG_SHAPE     = 0x30,
    NOE_STATUS_ERROR_UNMATCH_OUT_SHAPE    = 0x31,
    NOE_STATUS_ERROR_ZERO_TENSOR_SIZE     = 0x32,
    NOE_STATUS_ERROR_ALLOC_GRIP_ID        = 0x33,
    NOE_STATUS_ERROR_ALLOC_GROUP_ID       = 0x34,
    NOE_STATUS_ERROR_NOT_FOUND_IN_HASHTABLE = 0x35,
    NOE_STATUS_ERROR_INVALID_COREDUMP     = 0x36,
    NOE_STATUS_ERROR_JOB_DISPATCH_FAIL    = 0x37,
    NOE_STATUS_MAX                        = 0x38,
    NOE_STATUS_INVALID_ARGUMENT           = 0x40,//andy added
    NOE_STATUS_OUT_OF_MEMORY              = 0x41,//andy added
    NOE_STATUS_DEVICE_ERROR               = 0x42,//andy added
    NOE_STATUS_NOT_SUPPORTED              = 0x43,//andy added
    NOE_STATUS_TIMEOUT                    = 0x44,//andy added
    NOE_STATUS_ERROR                      = 0x45,//andy added
    NOE_STATUS_ERROR_UNKNOWN_ERROR        = 0x200,
    NOE_STATUS_ERROR_KEYBOARD_INTERRUPT   = 0x300,
    NOE_STATUS_ERROR_SYSTEM_ERR           = 0x301,
    NOE_STATUS_ERROR_OUT_OF_SPEC          = 0x380,
    NOE_STATUS_ERROR_OUT_OF_AIFF_SPEC     = 0x381,
    NOE_STATUS_ERROR_OUT_OF_TPC_SPEC      = 0x382,
    NOE_STATUS_ERROR_OUT_OF_DMA_SPEC      = 0x383,
    NOE_STATUS_ERROR_OUT_OF_MEM_RANGE     = 0x400,
    NOE_STATUS_ERROR_OUT_OF_SRAM_RANGE    = 0x401,
    NOE_STATUS_ERROR_OUT_OF_LSRAM0_RANGE  = 0x402,
    NOE_STATUS_ERROR_OUT_OF_LSRAM1_RANGE  = 0x403,
    NOE_STATUS_ERROR_OUT_OF_LSRAM_RANGE   = 0x404,
    NOE_STATUS_ERROR_OUT_OF_GSRAM0_RANGE  = 0x405,
    NOE_STATUS_ERROR_OUT_OF_GSRAM1_RANGE  = 0x406,
    NOE_STATUS_ERROR_OUT_OF_GSRAM_RANGE   = 0x407,
    NOE_STATUS_ERROR_ARITHMETIC_ERR       = 0x480,
    NOE_STATUS_ERROR_FLOAT_POINT_ERR      = 0x481,
    NOE_STATUS_ERROR_UNDERFLOW_ERR        = 0x482,
    NOE_STATUS_ERROR_OVERFLOW_ERR         = 0x483,
    NOE_STATUS_ERROR_NOT_A_NUMBER_ERR     = 0x484,
    NOE_STATUS_ERROR_INFINITY_ERR         = 0x485,
    NOE_STATUS_ERROR_STRING_LENGTH_ERR    = 0x486,
    NOE_STATUS_ERROR_ZERO_DIVISION_ERR    = 0x487,
} noe_status_t;

typedef enum {
    NOE_DEVICE_AIPU,
    NOE_DEVICE_VSI
} noe_device_t;

typedef enum {
    cpu = 0,
    npu = 1,
    gpu = 2,
}device_type_t;

typedef enum {
    NOE_DEV_IDLE = 0,
    NOE_DEV_BUSY,
    NOE_DEV_EXCEPTION
} noe_device_status_t;

/**
 * @struct noe_dynshape_param
 *
 * @brief Dynamic shape information to be configured for one graph.
 *
 * @note 'ds_data' is a uint32 array that describes a shape, e.g.:
 *        N*H*W*C => [u32_N, u32_H, u32_W, u32_C], total 4 words.
 */
 typedef struct noe_dynshape_item
 {
     uint32_t ds_idx;      /**< the dynamic shape index */
     uint32_t *ds_data;    /**< the dynamic shape information of configured */
 } noe_dynshape_item_t;

 /**
  * @struct noe_dynshape_param
  *
  * @brief Dynamic shape information to be configured for one graph.
  *
  * @note input_shape_cnt can be less than the number of input tensors, but providing
  *       full input shapes is recommended. Otherwise, unspecified shapes are set
  *       to the minimum shape from build time.
  */
 typedef struct noe_dynshape_param
 {
     uint32_t input_shape_cnt; /**< input shape counter provided */
     noe_dynshape_item_t *shape_items; /**< configured input shape info */
 } noe_dynshape_param_t;

typedef struct {
    /**
     * dump_dir is used as file dump-path
     */
    const char* dump_dir;
    /**
     * name prefix of dump files
     */
    const char* prefix;
    /**
     * name prefix of output dump files
     */
    const char *output_prefix;
    /**
     * name prefix of profile/printf data files
     */
    const char *misc_prefix;
} job_config_dump_t;

/**
 * @note fm_mem_region
 *       If configuring the feature buffer region, ignore partition_id and qos_level,
 *       set them to 0, and only set fm_mem_region. The buffer is allocated only if
 *       there is enough space in the region marked by fm_mem_region.
 *       region marked by `fm_mem_region`.
 *
 * @note fm_idxes
 *       The indices of feature map tensors. These buffers first try to allocate from
 *       the region specified by fm_mem_region.
 *
 *
 * @note dbg_dispatch and dbg_core_id
 *       These fields can dispatch a job to a specific core for debugging. Leave them
 *       unset for normal use.
 */
typedef struct job_config_npu {
    union {
#ifdef __cplusplus
        uint32_t misc = 0;
#else
        uint32_t misc;
#endif
        struct {
            uint8_t partition_id:4;  /**< default 0, in partition-0, only for CIX NPU */
            uint8_t dbg_dispatch:1;  /**< debug dispatch flag, set 1 to indicate specify job
                                          to debug core to run */
            uint8_t dbg_core_id:3;   /**< specify debug core id, [0, max_core_id in cluster] */
            uint8_t qos_level:4;     /**< default 0, low priority, only for CIX NPU */
            uint8_t fm_mem_region:4; /**< default 0, feature map buffer memory region */
            uint8_t wt_mem_region:4; /**< default 0, weight buffer memory region */
        };
    };

    bool bind_enable; /**< enable bind dispatch */
    uint8_t bind_cluster_ids; /**< bitx represents clusterx, e.g. 0x02 binds to cluster1.
                                   Currently only one cluster binding is supported. */
    uint8_t bind_core_ids; /**< bitx represents corex, e.g. 0x03 binds to core0 and core1.
                                Multi-core binding only supports > v3. */

    uint32_t reserved_iova_size; /**< only for > v3. Default is graph IO size. Dynamic ASID0 IOVA is reserved
                                    for dmabuf. */

    int32_t* fm_idxes; /**< specify feature maps allocated from fm_mem_region. If SRAM or DTCM, ensure they are in ASID0
                          region. */
    int32_t fm_idxes_cnt; /**< number of elements in fm_idxes */

    noe_dynshape_param_t* dynshape; /**< [[deprecated]] replaced by dynshape_params below */
    noe_dynshape_param_t dynshape_params; /**< dynamic shape parameters; you can also set or update
                                                them via noe_config_job */
} job_config_npu_t;

/**
 * @note config_job_npu
 *       Configure job partition, QoS (priority), and feature/weight buffer regions
 *       for CIX NPU when creating a job.
 */
typedef struct job_config {
    job_config_npu_t* conf_j_npu;
}job_config_t;

typedef struct {
    uint64_t instr_base;
    void* simulation_noe;
    void* simulation_mem_engine;
} job_info_npu_t;

typedef struct {
    job_info_npu_t* j_info;
} job_info_t;

/**
 * @struct aipu_share_buf_t
 *
 * @brief Request a common shared buffer. The 'size' field must be set. Mainly for noe_ioctl.
 *
 */
typedef struct noe_share_buf
{
    uint64_t pa; /**< buffer pa: filled by UMD */
    uint64_t va; /**< buffer va: filled by UMD */
    uint32_t size; /**< buffer size: filled by USER */
    uint32_t mem_type; /**< memory region type: AIPU_MEM_REGION_DEFAULT, AIPU_MEM_REGION_SRAM */
} noe_share_buf_t;

/**
 * @brief Job status.
 */
typedef enum {
    NOE_JOB_STATUS_NO_STATUS, /**< no status */
    NOE_JOB_STATUS_DONE,      /**< job completed successfully */
    NOE_JOB_STATUS_EXCEPTION,  /**< job failed with an exception */
    NOE_JOB_COREDUMP /**< job triggers coredump */
} job_status_t;

/**
 * @brief Job priority.
 */
typedef enum {
    NOE_HIGH_PRIOR,
    NOE_LOW_PRIOR
}job_prior_t;

typedef enum {
    /**
     * NOE_GLOBAL_CONFIG_TYPE_[*]: for noe_config_global() only;
     * NOE_JOB_CONFIG_TYPE_[*]: for noe_config_job() only;
     * NOE_CONFIG_TYPE_[*]: for noe_config_global/noe_config_job();
     */
    /* 1~16 */
    NOE_JOB_CONFIG_TYPE_DUMP_TEXT = 0x1,
    NOE_JOB_CONFIG_TYPE_DUMP_WEIGHT = 0x2,
    NOE_JOB_CONFIG_TYPE_DUMP_RODATA = 0x4,
    NOE_JOB_CONFIG_TYPE_DUMP_DESCRIPTOR = 0x8,
    NOE_JOB_CONFIG_TYPE_DUMP_INPUT = 0x10,
    NOE_JOB_CONFIG_TYPE_DUMP_OUTPUT = 0x20,
    NOE_JOB_CONFIG_TYPE_DUMP_REUSE = 0x40,
    NOE_JOB_CONFIG_TYPE_DUMP_TCB_CHAIN = 0x80,
    NOE_JOB_CONFIG_TYPE_DUMP_EMULATION = 0x100,
    NOE_JOB_CONFIG_TYPE_DUMP_PROFILE = 0x200,
    NOE_JOB_CONFIG_TYPE_DYNAMIC_PARAMS = 0x400,

    /* 16~24 */
    NOE_CONFIG_TYPE_SIMULATION = 0x10000,
    NOE_CONFIG_TYPE_HW = 0x20000,

    /* 24~32 */
    NOE_GLOBAL_CONFIG_TYPE_DISABLE_VER_CHECK = 0x1000000,
    NOE_GLOBAL_CONFIG_TYPE_ENABLE_VER_CHECK = 0x2000000,
} noe_config_type_t;

/**
 * @note wt_mem_region
 *       If configuring the weight buffer region for CIX NPU, ignore partition_id and qos_level,
 *       set them to 0, and only set fm_mem_region. The buffer is allocated only if there is
 *       enough space in the region marked by fm_mem_region.
 *       region marked by `fm_mem_region`.
 *
 * @note wt_idxes
 *       The indices of weight tensors. These buffers first try to allocate from
 *       the region specified by wt_mem_region.
 */
typedef struct graph_config_npu {
    union {
#ifdef __cplusplus
        uint32_t misc = 0;
#else
        uint32_t misc;
#endif
        struct {
            uint8_t wt_mem_region:4; /**< default 0, weight buffer memory region */
        };
    };

    int32_t* wt_idxes; /**< specify weights allocated from 'wt_mem_region' */
    int32_t wt_idxes_cnt; /**< the emement number in wt_idxes */
    const char* extra_weight_path; /**< the extra weight files path */
    bool put_weight_gm;
    bool put_desc_gm;
    bool put_ws_gm;
} graph_config_npu_t;

/**
 * @union noe_load_model
 *
 * @brief Support configuration during graph loading.
 *
 * @note config_graph_npu
 *       Configure weight buffer region for CIX NPU.
 */
typedef struct graph_config{
    graph_config_npu_t* conf_g_npu;
}graph_config_t;

typedef struct
{
    /**
     * set false if job polling thread isn't commit thread;
     * set true if job polling thread is commit thread.
     *
     * default true (no config via this structure): the polling thread is identical to the commit thread.
     *
     * eg: poll job in non-commit thread
     *   hw_config->poll_in_commit_thread = false;
     *   aipu_config_global(ctx, AIPU_CONFIG_TYPE_HW, hw_config);
     *
     * note: this configuration is effective only within a single process context.
     */
    bool poll_in_commit_thread;

    /**
     * only for x2. Default true (no config via this structure). We suggest leaving it true because
     * some signal IRQs are coupled with the TEC done interrupt (e.g., operator printf).
     * Disabling TEC done IRQ may hang when operator printf is enabled.
     */
    bool enable_tec_done_irq;
} noe_global_config_hw_t;


typedef enum {
    NOE_TENSOR_TYPE_INPUT         = 0,
    NOE_TENSOR_TYPE_OUTPUT        = 1,
    NOE_TENSOR_TYPE_INTER_DUMP    = 2,
    NOE_TENSOR_TYPE_PRINTF        = 3,
    NOE_TENSOR_TYPE_PROFILER      = 4,
    NOE_TENSOR_TYPE_LAYER_COUNTER = 5,
    NOE_TENSOR_TYPE_ERROR_CODE    = 6, /* only for v1/v2 */
    NOE_TENSOR_TYPE_OUT_TENSOR_SHAPE = 7,
} tensor_type_t;

typedef enum {
    NOE_DATA_TYPE_NONE = 0,
    NOE_DATA_TYPE_BOOL = 1,
    NOE_DATA_TYPE_U8   = 2,
    NOE_DATA_TYPE_S8   = 3,
    NOE_DATA_TYPE_U16  = 4,
    NOE_DATA_TYPE_S16  = 5,
    NOE_DATA_TYPE_U32  = 6,
    NOE_DATA_TYPE_S32  = 7,
    NOE_DATA_TYPE_U64  = 8,
    NOE_DATA_TYPE_S64  = 9,
    NOE_DATA_TYPE_F16  = 0xa,
    NOE_DATA_TYPE_F32  = 0xb,
    NOE_DATA_TYPE_F64  = 0xc,
    NOE_DATA_TYPE_BF16 = 0xd,

    // byte-aligned u/int4
    NOE_DATA_TYPE_ALIGNED_U4  = 0x12,
    NOE_DATA_TYPE_ALIGNED_S4  = 0x13,

    // byte-aligned u/int12
    NOE_DATA_TYPE_ALIGNED_U12 = 0x14,
    NOE_DATA_TYPE_ALIGNED_S12 = 0x15,

    NOE_DATA_TYPE_COMPACT_U4  = 0x20,
    NOE_DATA_TYPE_COMPACT_S4  = 0x21,
    NOE_DATA_TYPE_COMPACT_U12 = 0x22,
    NOE_DATA_TYPE_COMPACT_S12 = 0x23,
} noe_data_type_t;

typedef struct
{
    uint32_t id;
    uint32_t size;
    float    scale;
    int32_t    zero_point;
    noe_data_type_t data_type;
} tensor_desc_t;

/**
 * @brief case1: mark one tensor buffer of one graph as shared with other graphs.
 *               (in one process context)
 *
 * @brief case2: describe a shared buffer based on dma_buf mechanism.
 *               this dma_buf is managed by NPU KMD. (among multiple processes)
 *
 * @brief case3: specify a shared buffer allocated in external memory manager.
 *               (not original NPU driver)
 *
 * @brief case4: describe a shared buffer based on dma_buf mechanism.
 *               this dma_buf is managed by other modules(not KMD). (among multiple processes)
 *
 */
typedef struct shared_tensor_info_npu {
    /* the common fields */
    tensor_type_t type;     /**< the shared tensor's type: input/output */
    uint32_t tensor_idx;         /**< the shared tensor's index */

    /* fields for case1 and case3, not recommended */
    uint64_t id;                 /**< pass job ID for marking one io buffer as shared
                                      ignored for dma_buf share */
    uint64_t pa;                 /**< the physical address of shared tensor, ignored for dma_buf share */

    /* fields for case2, recommended */
    int dmabuf_fd;               /**< the fd corresponding to shared buffer from dma_buf allocator */
    uint32_t offset_in_dmabuf;   /**< the shared address offset in dma_buf which is specified by 'fd' */

    /**
     * it has to set this type, indicates sharing is for which case.
     */
    int shared_case_type;
} shared_tensor_info_t;

typedef struct noe_dma_buf_req
{
    int fd; /**< [out] dma-buf fd provided by KMD */
    uint64_t bytes;
} noe_dma_buf_req_t;

typedef struct noe_dma_buf_desc
{
    int fd;
    uint64_t pa; /**< [out] pa of dma-buf fd */
    uint64_t bytes; /**< [out] bytes of dma-buf fd */
} noe_dma_buf_desc_t;

// /**
//  * @struct noe_dynshape_param
//  *
//  * @brief dynamic shape information to be confirgured for one graph.
//  *
//  * @note 'ds_data' is uint32 array indicates a shape, eg:
//  *        N*H*W*C => [u32_N, u32_H, u32_W, u32_C], total 4 words.
//  */
// typedef struct noe_dynshape_item
// {
//     uint32_t ds_idx;      /**< the dynamic shape index */
//     uint32_t *ds_data;    /**< the dynamic shape information of configured */
// } noe_dynshape_item_t;

// /**
//  * @struct noe_dynshape_param
//  *
//  * @brief dynamic shape information to be confirgured for one graph.
//  *
//  * @note input_shape_cnt can be less than input tensors number of graph, but you are suggested to providing
//  *       full input shapes. Otherwise, not provided shape will be set as minimum shape which is same with building time
//  */
// typedef struct noe_dynshape_param
// {
//     uint32_t input_shape_cnt; /**< input shape counter provided */
//     noe_dynshape_item_t *shape_items; /**< configured input shape info */
// } noe_dynshape_param_t;
typedef enum {
    NOE_MEM_REGION_DEFAULT = 0,/**< DDR */
    NOE_MEM_REGION_SRAM    = 1,/**< On-Chip Memory */
    NOE_MEM_REGION_DTCM    = 2,/**< Data Tightly Couped Memory, only for v2 */
} noe_mem_region_t;

/**
 * @brief ioctl commands to operate shared tensor buffers for KMD
 */
typedef enum {
    NOE_IOCTL_SET_PROFILE = 255,
    NOE_IOCTL_GET_NOEBIN_BUILDVERSION,
    NOE_IOCTL_GET_DS_NUM, /* [[deprecate]] */
    NOE_IOCTL_GET_DS_DIM_NUM, /* [[deprecate]] */
    NOE_IOCTL_GET_DS_INFO, /* [[deprecate]] */
    NOE_IOCTL_GET_DS_RANK,
    NOE_IOCTL_GET_DS_DIM_CONSTRAINT,
    NOE_IOCTL_SET_DYNAMIC_ASID1, /* only v3 need, for core bind and multi-model parallel, default is false */
    NOE_IOCTL_ALLOC_SHARE_BUF,
    NOE_IOCTL_FREE_SHARE_BUF,

    /* map to KMD's ioctl */
    NOE_IOCTL_ALLOC_DMABUF,
    NOE_IOCTL_FREE_DMABUF,
    NOE_IOCTL_GET_DMABUF_INFO,
    NOE_IOCTL_WRITE_DMABUF,
    NOE_IOCTL_READ_DMABUF,
    NOE_IOCTL_ATTACH_DMABUF,
    NOE_IOCTL_DETACH_DMABUF,
    NOE_IOCTL_GET_VERSION,
    NOE_IOCTL_ABORT_CMDPOOL,
    NOE_IOCTL_ENABLE_TICKCOUNTER,
    NOE_IOCTL_DISABLE_TICKCOUNTER,

    /* register or memory rw */
    NOE_IOCTL_RW_REGISTER,
    NOE_IOCTL_RW_MEMORY,
    NOE_IOCTL_UMD_MAX,
} ioctl_cmd_t;

/**
 * @brief Function prototype for a job callback handler.
 *
 * @param[in] job_id    Done job's id
 * @param[in] job_state Done job's state
 *
 * @retval 0 for successfully calling, other value for abnormally calling
 */
#if defined(BUILD_PYTHON_API)
#include <functional>
typedef std::function<int(uint64_t, job_status_t)> job_callback_func_t;
#else
typedef int (*job_callback_func_t)(uint64_t job_id, job_status_t job_state);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Initialize CIX NOE UMD context.
 *
 * @param[out] ctx Pointer to a memory location allocated by application where UMD stores the
 *                     opaque context handle struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_OPEN_DEV_FAIL
 * @retval NOE_STATUS_ERROR_DEV_ABNORMAL
 *
 * @note Before invoking any other UMD API calls, any UMD application must initialize a context first.
 */
#ifdef __cplusplus
noe_status_t noe_init_context(noe_context_t* ctx, noe_device_t device = NOE_DEVICE_AIPU);
#else
noe_status_t noe_init_context(noe_context_t* ctx, noe_device_t device);
#endif


/**
 * @brief Destroy CIX UMD context.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_DEINIT_FAIL
 */
noe_status_t noe_deinit_context(noe_context_t ctx);

/**
 * @brief Get a readable error message for a returned status code.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  status Status returned by UMD standard API
 * @param[out] msg    Pointer to a memory location allocated by application where UMD stores the
 *                        message string pointer
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 */
noe_status_t noe_get_error_message(const noe_context_t ctx, noe_status_t status, const char** msg);

/**
 * @brief This API is used to configure a specified global option.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  types  Configuration type(s)
 * @param[out] config Pointer to application-owned configuration data
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_CONFIG
 *
 */
noe_status_t noe_config_global(const noe_context_t ctx, env_type_t types, void* config);

/**
 * @brief Load an offline-built CIX executable graph binary from the file system.
 *
 * @param[in]  ctx   Pointer to a context handle struct returned by noe_init_context
 * @param[in]  graph_file Executable graph binary file path
 * @param[out] id    Pointer to a memory location allocated by application where UMD stores the
 *                   opaque graph handle
 * @param[in]  config Pointer to specific configuration struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval NOE_STATUS_ERROR_MAP_FILE_FAIL
 * @retval NOE_STATUS_ERROR_UNKNOWN_BIN
 * @retval NOE_STATUS_ERROR_GVERSION_UNSUPPORTED
 * @retval NOE_STATUS_ERROR_INVALID_GBIN
 * @retval NOE_STATUS_ERROR_TARGET_NOT_FOUND
 * @retval NOE_STATUS_ERROR_BUF_ALLOC_FAIL
 * @retval NOE_STATUS_ERROR_RESERVE_SRAM_FAIL
 * @retval NOE_STATUS_ERROR_INVALID_GM
 */
#ifdef __cplusplus
noe_status_t noe_load_graph(const noe_context_t ctx, const char* graph_file,
    uint64_t* id, graph_config_t *config = nullptr);
#else
noe_status_t noe_load_graph(const noe_context_t ctx, const char* graph_file,
    uint64_t* id, graph_config_t *config);
#endif

/**
 * @brief Load a graph from an in-memory executable graph binary buffer.
 *
 * @param[in]  ctx   Pointer to a context handle struct returned by noe_init_context
 * @param[in]  graph_buf The start address of buffer which stores graph binary data
 * @param[in]  graph_size The byte size of graph binary data in 'graph_buf'
 * @param[out] id    Pointer to a graph handle struct
 * @param[in]  config Pointer to specific configuration struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_SIZE
 * @retval NOE_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval NOE_STATUS_ERROR_MAP_FILE_FAIL
 * @retval NOE_STATUS_ERROR_UNKNOWN_BIN
 * @retval NOE_STATUS_ERROR_GVERSION_UNSUPPORTED
 * @retval NOE_STATUS_ERROR_INVALID_GBIN
 * @retval NOE_STATUS_ERROR_TARGET_NOT_FOUND
 * @retval NOE_STATUS_ERROR_BUF_ALLOC_FAIL
 * @retval NOE_STATUS_ERROR_RESERVE_SRAM_FAIL
 * @retval NOE_STATUS_ERROR_INVALID_GM
 */
#ifdef __cplusplus
noe_status_t noe_load_graph_helper(const noe_context_t ctx, const char* graph_buf,
    uint32_t graph_size, uint64_t* id, graph_config_t *config = nullptr);
#else
noe_status_t noe_load_graph_helper(const noe_context_t ctx, const char* graph_buf,
    uint32_t graph_size, uint64_t* id, graph_config_t *config);
#endif

/**
 * @brief This API is used to unload a loaded graph
 *
 * @param[in] ctx   Pointer to a context handle struct returned by noe_init_context
 * @param[in] id Pointer to a graph handle struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 */
noe_status_t noe_unload_graph(const noe_context_t ctx, uint64_t id);

/**
 * @brief Create a new job for a graph with a provided buffer handle.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  graph  Pointer to a graph handle struct
 * @param[out] job    Pointer to a job handle struct
 * @param[in]  config Specify job partition id, QoS level, and memory regions for
 *                    feature map and weight buffers
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_BUF_ALLOC_FAIL
 *
 * @note The application can create one or multiple jobs by calling this API one or multiple times.
 * @note Through 'config' parameter, the feature map and weight buffer can be allocated from
 *           specific memory regions. The allocate order is: DTCM->SRAM->DDR.
 *           For example: if you intend to allocate feature map buffer from DTCM, it will try to
 *           allocate from DTCM, if there's no enough free space, it tries to check SRAM, then
 *           DDR until fail.
 */
#ifdef __cplusplus
noe_status_t noe_create_job(const noe_context_t ctx, uint64_t graph,
    uint64_t* job, job_config_t *config = nullptr);
#else
noe_status_t noe_create_job(const noe_context_t ctx, uint64_t graph,
    uint64_t* job, job_config_t *config);
#endif

/**
 * @brief Flush a new computation job (blocking).
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] job      Job handle struct returned by noe_create_job
 * @param[in] time_out Time out (in millisecond) specified by application for this job
 *                     (A timeout of value <= 0 means an infinite timeout.)
 * @param[in] cb_func  Callback function
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 * @retval NOE_STATUS_ERROR_JOB_EXCEPTION
 * @retval NOE_STATUS_ERROR_JOB_TIMEOUT
 */
noe_status_t noe_job_infer_sync(const noe_context_t ctx, uint64_t job, int32_t time_out);

/**
 * @brief Flush a new computation job (non-blocking).
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] job      Pointer to a job handle struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 *
 * @note A flushed job cannot be flushed again before the previous scheduled one is done.
 */
#ifdef __cplusplus
noe_status_t noe_job_infer_async(const noe_context_t ctx, uint64_t job, job_callback_func_t cb_func=nullptr);
#else
noe_status_t noe_job_infer_async(const noe_context_t ctx, uint64_t job, job_callback_func_t cb_func);
#endif

/**
 * @brief Get the execution status of a flushed job (non-blocking).
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Pointer to a job handle
 * @param[out] status Pointer to a memory location allocated by the application where UMD stores the job status
 *                    NOE_JOB_STATUS_DONE: job is normally done
 *                    NOE_JOB_STATUS_EXCEPTION: exception occurring on this job
 *                    NOE_JOB_STATUS_NO_STATUS: job is in handling
 * @param[in]  timeout timeout value(ms) to poll job's status
 *                     timeout > 0: the max polling time window is 'timeout'
 *                     timeout = 0: non-blocking and return job's status immediately.
 *                     timeout = -1: blocking until job is really done or exception.
 *
 * @param[in] cb_func  Callback function
 *
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_TIMEOUT
 *
 * @note This API should be used after noe_job_infer_async returns successfully.
 */
#ifdef __cplusplus
// TODO(umd): Not implemented in src yet.
noe_status_t noe_job_infer_wait(const noe_context_t ctx, uint64_t job, job_status_t* status, job_callback_func_t* cb_func=nullptr, int32_t time_out = 0);
#else
noe_status_t noe_job_infer_wait(const noe_context_t ctx, uint64_t job, job_status_t* status, job_callback_func_t* cb_func, int32_t time_out);
#endif

/**
 * @brief Cancel a scheduled job.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] job Pointer to a job handle
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_job_concel(const noe_context_t ctx, uint64_t job);

/**
 * @brief Clean up a finished job object.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] job Pointer to a job handle
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 */
noe_status_t noe_clean_job(const noe_context_t ctx, uint64_t job);

/**
 * @brief Set a job scheduling priority.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] job Pointer to a job handle struct
 * @param[in] p_type
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_job_set_prior(const noe_context_t ctx, uint64_t job, job_prior_t p_type);

/**
 * @brief Get the execution status of an async job (non-blocking).
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] job Pointer to a job handle struct
 * @param[out] status Pointer to a memory location allocated by the application where UMD stores the job status
 *                    NOE_JOB_STATUS_DONE: job is normally done
 *                    NOE_JOB_STATUS_EXCEPTION: exception occurring on this job
 *                    NOE_JOB_STATUS_NO_STATUS: job is in handling
 * @param[in]  timeout timeout value(ms) to poll job's status
 *                    timeout > 0: the max polling time window is 'timeout'
 *                    timeout = 0: non-blocking and return job's status immediately.
 *                    timeout = -1: blocking until job is really done or exception.
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_TIMEOUT
 *
 * @note This API should be used after noe_job_infer_async returns successfully.
 */
#ifdef __cplusplus
noe_status_t noe_get_job_status(const noe_context_t ctx, uint64_t job, job_status_t* status, int32_t timeout = 0);
#else
noe_status_t noe_get_job_status(const noe_context_t ctx, uint64_t job, job_status_t* status, int32_t timeout);
#endif

/**
 * @brief This API is used to load input tensor data
 *
 * @param[in] ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in] job    Job ID returned by noe_create_job
 * @param[in] tensor Input tensor ID
 * @param[in] data   Data of input tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
noe_status_t noe_load_tensor(const noe_context_t ctx, uint64_t job, uint32_t tensor, const void* data);

/**
 * @brief This API is used to get tensor data of specified type
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Job ID returned by noe_create_job
 * @param[in]  type   Tensor type
 * @param[in]  tensor Input tensor ID
 * @param[out] data   Data of output tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
noe_status_t noe_get_tensor(const noe_context_t ctx, uint64_t job,
    tensor_type_t type, uint32_t tensor, void* data);

/**
 * @brief Bind a created job to an idle NPU core for execution later.
 *        External registers of the specified AIPU core are written after this API returns,
 *        but the start PC register is not triggered to run.
 *
 * @param[in] ctx     Pointer to a context handle struct returned by noe_init_context
 * @param[in] job_id  Job ID returned by noe_create_job
 * @param[in] id      ID of an NPU core or ID of a NPU partition
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_CORE_ID
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP

 * @note For the same core or job, this should be bound only once unless the job is done.
 * @note The core or partition to be bound should be idle; otherwise UMD returns an error code.
 */
// noe_status_t noe_debugger_bind_job(const noe_context_t ctx, uint32_t id, uint64_t job);

/**
 * @brief Trigger a previously bound job to run on a target NPU core.
 *        This is a blocking API that returns after the job ends on hardware.
 *
 * @param[in] ctx     Pointer to a context handle struct returned by noe_init_context
 * @param[in] job_id  Job ID returned by noe_create_job and bind by noe_debugger_bind_job
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_JOB_EXCEPTION
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// noe_status_t noe_debugger_run_job(const noe_context_t ctx, uint64_t job);

/**
 * @brief This API is used to configure a specified option of a job.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Job ID returned by noe_create_job
 * @param[in]  types  Configuration type(s)
 * @param[out] config Pointer to a memory location allocated by application where application stores the
 *                        configuration data struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_CONFIG
 *
 */
noe_status_t noe_config_job(const noe_context_t ctx, uint64_t job, uint64_t types, void* config);

/**
 * @brief Specify a shared buffer as a job's IO buffer.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Job ID returned by noe_create_job
 * @param[in]  shared_buf Pointer to a shared buffer allocated through the dma_buf system
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_DMABUF_SHARED_IO
 *
 * @note this API is just used on HW
 */
noe_status_t noe_specify_iobuf(const noe_context_t ctx, uint64_t job,
    shared_tensor_info_t *shared_buf);

/**
 * @brief This API is used to get tensor count of specified type
 *
 * @param[in]  ctx  Pointer to a context handle struct returned by noe_init_context
 * @param[in]  id   Pointer to a graph handle struct
 * @param[in]  type Tensor type
 * @param[out] cnt  Pointer to a memory location allocated by application where UMD stores the count
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 */
noe_status_t noe_get_tensor_count(const noe_context_t ctx, uint64_t id, tensor_type_t type, uint32_t* cnt);

/**
 * @brief Get a tensor descriptor for the specified type.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  id     Pointer to a graph handle struct
 * @param[in]  type   Tensor type
 * @param[in]  index Tensor ID
 * @param[out] desc   Pointer to a memory location allocated by application where UMD stores the tensor descriptor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 */
noe_status_t noe_get_tensor_descriptor(const noe_context_t ctx, uint64_t id, tensor_type_t type, uint32_t index, tensor_desc_t* desc);

/**
 * @brief Get input/output tensor name and shape from a CIX model file.
 *
 * @param[in]  cix_model Path to CIX model file
 * @param[in]  type      Tensor type: NOE_TENSOR_TYPE_INPUT or NOE_TENSOR_TYPE_OUTPUT
 * @param[in]  index     Tensor index
 * @param[out] name      Pointer to a memory location allocated by application where UMD stores name (null-terminated)
 * @param[in,out] name_len In: capacity of name buffer; Out: actual length (excluding null)
 * @param[out] shape     Pointer to a memory location allocated by application where UMD stores shape data
 * @param[in,out] dim_num In: capacity of shape array; Out: actual dim number
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_TYPE
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_SIZE
 * @retval NOE_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval NOE_STATUS_ERROR_INVALID_GBIN
 * @retval NOE_STATUS_ERROR_GVERSION_UNSUPPORTED
 */
noe_status_t noe_get_tensor_shape(const char* cix_model, tensor_type_t type, uint32_t index,
    char* name, uint32_t* name_len, int32_t* shape, uint32_t* dim_num);

/**
 * @brief Get a tensor descriptor for the specified type.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  id     Pointer to a graph handle struct
 * @param[in]  type   Tensor type
 * @param[in]  name   Tensor name
 * @param[out] desc   Pointer to a memory location allocated by application where UMD stores the tensor descriptor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_tensor_attr_by_name(const noe_context_t ctx, uint64_t* id, tensor_type_t type, const char* name, tensor_desc_t* desc);

/**
 * @brief Load input tensor data.
 *
 * @param[in] ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in] job    Pointer to a job handle struct
 * @param[in] index  Input tensor ID
 * @param[in] data   Data of input tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_set_input_tensor_data_by_index(const noe_context_t ctx, uint64_t job,
    uint32_t index, void* data);

/**
 * @brief Load input tensor data.
 *
 * @param[in] ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in] job    Pointer to a job handle struct
 * @param[in] name   Input tensor name
 * @param[in] data   Data of input tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_set_input_tensor_data_by_name(const noe_context_t ctx, uint64_t job,
    const char* name, void* data);

/**
 * @brief Get tensor data for the specified type.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Pointer to a job handle struct
 * @param[in]  type   Tensor type
 * @param[in]  index  Input tensor ID
 * @param[out] data   Data of output tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_tensor_data_by_index(const noe_context_t ctx, uint64_t job,
    tensor_type_t type, uint32_t index, void* data);

/**
 * @brief Get tensor data for the specified type.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Pointer to a job handle struct
 * @param[in]  type   Tensor type
 * @param[in]  name   Input tensor name
 * @param[out] data   Data of output tensor
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_TENSOR_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_tensor_data_by_name(const noe_context_t ctx, uint64_t job,
    tensor_type_t type, const char* name, void* data);

/**
 * @brief This API is used to configure a specified option of a job.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Pointer to a job handle struct
 * @param[in]  types  Configuration type(s)
 * @param[in] config Pointer to a memory location allocated by application where application
 *             stores the configuration data struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_CONFIG
 *
 */
// noe_status_t noe_set_job_attr(const noe_context_t ctx, uint64_t job, job_attr_type_t types, void* config);

/**
 * @brief Specify a shared buffer as a job's IO buffer.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job    Pointer to a job handle struct
 * @param[in]  shared_buf Pointer to a shared buffer allocated through the dma_buf system
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_DMABUF_SHARED_IO
 *
 * @note this API is just used on HW
 */
// noe_status_t noe_set_sharedbuff(const noe_context_t ctx, uint64_t job, shared_tensor_info_t *shared_buf);

/**
 * @brief This API is used to create a batch queue.
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] graph_id Graph id
 * @param[out] queue_id Pointer to store batch queue id
 *
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 *
 * @note Each thread is subject to call this function to get private batch queue ID.
 *       It isn't allowed to share batch queue ID between threads.
 */
noe_status_t noe_create_batch_queue(const noe_context_t ctx, uint64_t graph_id, uint32_t *queue_id);

/**
 * @brief This API is used to clean a specific batch queue.
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] graph_id Graph id
 * @param[in] queue_id Pointer to store batch queue id
 *
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 */
noe_status_t noe_clean_batch_queue(const noe_context_t ctx, uint64_t graph_id, uint32_t queue_id);

/**
 * @brief Do basic configuration for batch inference on simulator and HW.
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] graph_id Graph id
 * @param[in] queue_id Batch queue id
 * @param[in] types    Dump options for each batch job
 * @param[in] dump_cfg The root path to store dump files of each batch
 *
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_NO_BATCH_QUEUE
 *
 */
noe_status_t noe_config_batch_dump(const noe_context_t ctx, uint64_t graph_id,
    uint32_t queue_id, uint64_t types, job_config_dump_t *dump_cfg);

/**
 * @brief Add a group of buffers for one frame inference to a batch queue.
 *
 * @param[in] ctx        Pointer to a context handle struct returned by noe_init_context
 * @param[in] graph_id   Graph id
 * @param[in] queue_id   Batch queue id
 * @param[in] inputs     Buffer pointers for input tensors
 * @param[in] outputs    Buffer pointers for output tensors
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_NO_BATCH_QUEUE
 */
noe_status_t noe_add_batch(const noe_context_t ctx, uint64_t graph_id,
    uint32_t queue_id, char *inputs[], char *outputs[]);

/**
 * @brief Run multiple batch inferences.
 *
 * @param[in] ctx      Pointer to a context handle struct returned by noe_init_context
 * @param[in] graph_id Graph id
 * @param[in] queue_id Batch queue id
 * @param[in] create_cfg Config for all batches in one queue
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_GRAPH_ID
 * @retval NOE_STATUS_ERROR_NO_BATCH_QUEUE
 */
noe_status_t noe_finish_batch(const noe_context_t ctx, uint64_t graph_id,
    uint32_t queue_id, job_config_t *create_cfg);

/**
 * @brief This API is used by debugger to get information of a job
 *
 * @param[in]  ctx  Pointer to a context handle struct returned by noe_init_context
 * @param[in]  job  Pointer to a job handle struct
 * @param[out] info Pointer to a memory location allocated by application where UMD stores
 *                      a pointer to the job information
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_job_info(const noe_context_t ctx, uint64_t job, job_info_t* info);

/**
 * @brief Bind a created job to an idle core for execution later.
 *
 * @param[in] ctx     Pointer to a context handle struct returned by noe_init_context
 * @param[in] job     Pointer to a job handle struct
 * @param[in] d_type          Device type
 * @param[in] core_index      ID of a core
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_CORE_ID
 * @retval NOE_STATUS_ERROR_INVALID_JOB_ID
 * @retval NOE_STATUS_ERROR_INVALID_OP

 * @note For the same core or job, this should be bound only once unless the job is done.
 * @note The core or partition to be bound should be idle; otherwise UMD returns an error code.
 */
// noe_status_t noe_bind_job_core(const noe_context_t ctx, uint32_t core_index, uint64_t job, device_type_t d_type);

/**
 * @brief This API is used to get device info.
 *
 * @param[in]  ctx Pointer to a context handle struct returned by noe_init_context
 * @param[out] device_info Pointer to a memory location allocated by application where UMD stores the device info
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
// noe_status_t noe_get_device_info(const noe_context_t ctx, device_info_t* device_info);

/**
 * @brief Allocate a buffer.
 *
 * @param[in] ctx     Pointer to a context handle struct returned by noe_init_context
 * @param[in] size    Size of the requested buffer in byte
 * @param[in] dma_buff    dma buffer flag
 * @param[out] va         Pointer to a virtual address stores the base address of the buffer
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_SIZE
 * @retval NOE_STATUS_ERROR_BUF_ALLOC_FAIL
 *
 */
// noe_status_t noe_malloc(const noe_context_t ctx, uint32_t size, void** va, bool dma_buff);

/**
 * @brief Free a buffer allocated by noe_malloc.
 *
 * @param[in] ctx  Pointer to a context handle struct returned by noe_init_context
 * @param[in] dma_buff    dma buffer flag
 * @param[in] va   Virtual address to free
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_BUF_FREE_FAIL
 */
// noe_status_t noe_free(const noe_context_t ctx, void* va, bool dma_buff);

/**
 * @brief Get profiler information.
 *
 * @param[in] ctx  Pointer to a context handle struct returned by noe_init_context
 * @param[in] job  Pointer to a job handle
 * @param[in] profiler_info   Pointer to profiler information struct
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_BUF_FREE_FAIL
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_profiler(const noe_context_t ctx, uint64_t job, void* profiler_info);

/**
 * @brief Save execution log information after the corresponding job ends.
 *
 * @param[in] ctx  Pointer to a context handle struct returned by noe_init_context
 * @param[in] job  Pointer to a job handle
 * @param[in] log_buff   Pointer to a tensor buffer that stores printf log data
 * @param[in] save_name File path to store printf data
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_PRINTF_FAIL
 *
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_save_log(const noe_context_t ctx, uint64_t job, char* log_buff, char* save_name);


/**
 * @brief Send a specific command to the driver.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] cmd cmd_type
 * @param[in] device_type Device type
 * @param[in/out] arg input or output argument according to 'cmd'
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_DEV_ABNORMAL
 *
 */
#ifdef __cplusplus
noe_status_t noe_ioctl(const noe_context_t, device_type_t device_type, ioctl_cmd_t cmd, void *arg = nullptr);
#else
noe_status_t noe_ioctl(const noe_context_t, device_type_t device_type, ioctl_cmd_t cmd, void *arg);
#endif

/**
 * @brief Set log level.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in] mode Log level
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_DEV_ABNORMAL
 *
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_set_log_level(const noe_context_t ctx, int mode);

/**
 * @brief Get log level.
 *
 * @param[in] ctx Pointer to a context handle struct returned by noe_init_context
 * @param[out] mode Log level
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_DEV_ABNORMAL
 *
 */
// TODO(umd): Not implemented in src yet.
noe_status_t noe_get_log_level(const noe_context_t ctx, int* mode);

/**
 * @brief Set NPU frequency.
 *
 * @param[in] idx Index of an available NPU frequency point
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_INVALID_CONFIG
 * @retval NOE_STATUS_ERROR_DEV_ABNORMAL
 *
 * @note Before invoking this API, the NPU must be idle.
 */
noe_status_t noe_set_freq(int idx);

/**
 * @brief Get NPU partition count.
 *
 * @param[in]  ctx Pointer to a context handle struct returned by noe_init_context
 * @param[out] cnt Pointer to a memory location allocated by application where UMD stores the partition count
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
noe_status_t noe_get_partition_count(const noe_context_t ctx, uint32_t* cnt);

/**
 * @brief Get NPU cluster count.
 *
 * @param[in]  ctx Pointer to a context handle struct returned by noe_init_context
 * @param[in]  partition_id Partition to query
 * @param[out] cnt Pointer to a memory location allocated by application where UMD stores the cluster count
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_OP
 */
noe_status_t noe_get_cluster_count(const noe_context_t ctx, uint32_t partition_id, uint32_t* cnt);

/**
 * @brief Get NPU core count in a specific cluster.
 *
 * @param[in]  ctx     Pointer to a context handle struct returned by noe_init_context
 * @param[in]  partition_id Partition ID
 * @param[in]  cluster Cluster ID
 * @param[out] cnt     Pointer to a memory location allocated by application where UMD stores the core count
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 * @retval NOE_STATUS_ERROR_INVALID_OP
 * @retval NOE_STATUS_ERROR_INVALID_CLUSTER_ID
 *
 * @note Cluster ID is numbered within [0, cluster_cnt).
 */
noe_status_t noe_get_core_count(const noe_context_t ctx, uint32_t partition_id, uint32_t cluster, uint32_t* cnt);

/**
 * @brief Get NPU architecture information.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[out] target Pointer to a memory location allocated by application where UMD stores
 *                    HW ARCH information
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 */
noe_status_t noe_get_target(const noe_context_t ctx, char *target);

/**
 * @brief Get NPU status.
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by noe_init_context
 * @param[out] status Pointer to a memory location allocated by application where UMD stores
 *                    NPU status information
 *
 * @retval NOE_STATUS_SUCCESS
 * @retval NOE_STATUS_ERROR_NULL_PTR
 * @retval NOE_STATUS_ERROR_INVALID_CTX
 */
noe_status_t noe_get_device_status(const noe_context_t ctx, noe_device_status_t *status);

// const char* noe_get_last_error(noe_context_t context);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // _CIX_NOE_API_H_
