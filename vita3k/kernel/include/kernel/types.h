// Vita3K emulator project
// Copyright (C) 2021 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once
#include <mem/ptr.h>
#include <util/types.h>

#define SCE_KERNEL_DEFAULT_PRIORITY static_cast<SceInt32>(0x10000100)
#define SCE_KERNEL_DEFAULT_PRIORITY_USER SCE_KERNEL_DEFAULT_PRIORITY
#define SCE_KERNEL_DEFAULT_PRIORITY_USER_INTERNAL SCE_KERNEL_DEFAULT_PRIORITY
#define SCE_KERNEL_HIGHEST_DEFAULT_PRIORITY (SCE_KERNEL_DEFAULT_PRIORITY - 32)
#define SCE_KERNEL_LOWEST_DEFAULT_PRIORITY (SCE_KERNEL_DEFAULT_PRIORITY + 31)

#define SCE_KERNEL_GAME_DEFAULT_PRIORITY_ACTUAL 160

#define SCE_KERNEL_HIGHEST_PRIORITY_USER 64
#define SCE_KERNEL_LOWEST_PRIORITY_USER 191

#define SCE_KERNEL_STACK_SIZE_USER_MAIN KB(256)
#define SCE_KERNEL_STACK_SIZE_USER_DEFAULT KB(4)

#define SCE_KERNEL_ATTR_TH_FIFO 0x00000000U
#define SCE_KERNEL_ATTR_TH_PRIO 0x00002000U
#define SCE_KERNEL_MUTEX_ATTR_RECURSIVE 0x2U

#define SCE_KERNEL_MSG_PIPE_MODE_ASAP 0x00000000U
#define SCE_KERNEL_MSG_PIPE_MODE_FULL 0x00000001U

#define SCE_KERNEL_MSG_PIPE_MODE_WAIT 0x00000000U
#define SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT 0x00000010U

#define SCE_KERNEL_MSG_PIPE_MODE_DONT_REMOVE 0x00000100U

#define SCE_KERNEL_LW_MUTEX_ATTR_TH_FIFO SCE_KERNEL_ATTR_TH_FIFO
#define SCE_KERNEL_LW_MUTEX_ATTR_TH_PRIO SCE_KERNEL_ATTR_TH_PRIO

#define KERNELOBJECT_MAX_NAME_LENGTH 31

#define SCE_UID_INVALID_UID (SceUID)(0xFFFFFFFF)

#define SCE_ERROR_ERRNO_EINVAL 0x80010016

constexpr size_t MODULE_INFO_NUM_SEGMENTS = 4;

enum SceKernelModel {
    SCE_KERNEL_MODEL_VITA,
    SCE_KERNEL_MODEL_VITATV
};

enum SceKernelErrorCode {
    SCE_KERNEL_OK = 0x0,
    SCE_KERNEL_ERROR_ERROR = 0x80020001,
    SCE_KERNEL_ERROR_NOT_IMPLEMENTED = 0x80020002,
    SCE_KERNEL_ERROR_NOSYS = 0x80020003,
    SCE_KERNEL_ERROR_UNSUP = 0x80020004,
    SCE_KERNEL_ERROR_INVALID_ARGUMENT = 0x80020005,
    SCE_KERNEL_ERROR_ILLEGAL_ADDR = 0x80020006,
    SCE_KERNEL_ERROR_ILLEGAL_ALIGNMENT = 0x80020007,
    SCE_KERNEL_ERROR_ILLEGAL_PERMISSION = 0x80020008,
    SCE_KERNEL_ERROR_INVALID_ARGUMENT_SIZE = 0x80020009,
    SCE_KERNEL_ERROR_INVALID_FLAGS = 0x8002000A,
    SCE_KERNEL_ERROR_ILLEGAL_SIZE = 0x8002000B,
    SCE_KERNEL_ERROR_ILLEGAL_TYPE = 0x8002000C,
    SCE_KERNEL_ERROR_ILLEGAL_PATTERN = 0x8002000D,
    SCE_KERNEL_ERROR_ILLEGAL_ATTR = 0x8002000E,
    SCE_KERNEL_ERROR_ILLEGAL_COUNT = 0x8002000F,
    SCE_KERNEL_ERROR_ILLEGAL_MODE = 0x80020010,
    SCE_KERNEL_ERROR_ILLEGAL_OPEN_LIMIT = 0x80020011,
    SCE_KERNEL_ERROR_ONLY_DEVELOPMENT_MODE = 0x80020012,
    SCE_KERNEL_ERROR_DEBUG_ERROR = 0x80021000,
    SCE_KERNEL_ERROR_ILLEGAL_DIPSW_NUMBER = 0x80021001,
    SCE_KERNEL_ERROR_PA_ERROR = 0x80021100,
    SCE_KERNEL_ERROR_PA_NOT_AVAILABLE = 0x80021101,
    SCE_KERNEL_ERROR_PA_INVALID_KEY = 0x80021102,
    SCE_KERNEL_ERROR_PA_KEY_IS_NOT_SHARED = 0x80021103,
    SCE_KERNEL_ERROR_PA_INVALID_SIGNATURE = 0x80021104,
    SCE_KERNEL_ERROR_CPU_ERROR = 0x80022000,
    SCE_KERNEL_ERROR_MMU_ILLEGAL_L1_TYPE = 0x80022001,
    SCE_KERNEL_ERROR_MMU_L2_INDEX_OVERFLOW = 0x80022002,
    SCE_KERNEL_ERROR_MMU_L2_SIZE_OVERFLOW = 0x80022003,
    SCE_KERNEL_ERROR_INVALID_CPU_AFFINITY = 0x80022004,
    SCE_KERNEL_ERROR_INVALID_MEMORY_ACCESS = 0x80022005,
    SCE_KERNEL_ERROR_INVALID_MEMORY_ACCESS_PERMISSION = 0x80022006,
    SCE_KERNEL_ERROR_VA2PA_FAULT = 0x80022007,
    SCE_KERNEL_ERROR_VA2PA_MAPPED = 0x80022008,
    SCE_KERNEL_ERROR_VALIDATION_CHECK_FAILED = 0x80022009,
    SCE_KERNEL_ERROR_SYSMEM_ERROR = 0x80024000,
    SCE_KERNEL_ERROR_INVALID_PROCESS_CONTEXT = 0x80024001,
    SCE_KERNEL_ERROR_UID_NAME_TOO_LONG = 0x80024002,
    SCE_KERNEL_ERROR_VARANGE_IS_NOT_PHYSICAL_CONTINUOUS = 0x80024003,
    SCE_KERNEL_ERROR_PHYADDR_ERROR = 0x80024100,
    SCE_KERNEL_ERROR_NO_PHYADDR = 0x80024101,
    SCE_KERNEL_ERROR_PHYADDR_USED = 0x80024102,
    SCE_KERNEL_ERROR_PHYADDR_NOT_USED = 0x80024103,
    SCE_KERNEL_ERROR_NO_IOADDR = 0x80024104,
    SCE_KERNEL_ERROR_PHYMEM_ERROR = 0x80024300,
    SCE_KERNEL_ERROR_ILLEGAL_PHYPAGE_STATUS = 0x80024301,
    SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE = 0x80024302,
    SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE_UNIT = 0x80024303,
    SCE_KERNEL_ERROR_PHYMEMPART_NOT_EMPTY = 0x80024304,
    SCE_KERNEL_ERROR_NO_PHYMEMPART_LPDDR2 = 0x80024305,
    SCE_KERNEL_ERROR_NO_PHYMEMPART_CDRAM = 0x80024306,
    SCE_KERNEL_ERROR_PHYMEMPART_OUT_OF_INDEX = 0x80024307,
    SCE_KERNEL_ERROR_CANNOT_GROW_PHYMEMPART = 0x80024308,
    SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE_CDRAM = 0x80024309,
    SCE_KERNEL_ERROR_INVALID_SUBBUDGET_ID = 0x8002430A,
    SCE_KERNEL_ERROR_FIXEDHEAP_ERROR = 0x80024400,
    SCE_KERNEL_ERROR_FIXEDHEAP_ILLEGAL_SIZE = 0x80024401,
    SCE_KERNEL_ERROR_FIXEDHEAP_ILLEGAL_INDEX = 0x80024402,
    SCE_KERNEL_ERROR_FIXEDHEAP_INDEX_OVERFLOW = 0x80024403,
    SCE_KERNEL_ERROR_FIXEDHEAP_NO_CHUNK = 0x80024404,
    SCE_KERNEL_ERROR_UID_ERROR = 0x80024500,
    SCE_KERNEL_ERROR_INVALID_UID = 0x80024501,
    SCE_KERNEL_ERROR_SYSMEM_UID_INVALID_ARGUMENT = 0x80024502,
    SCE_KERNEL_ERROR_SYSMEM_INVALID_UID_RANGE = 0x80024503,
    SCE_KERNEL_ERROR_SYSMEM_NO_VALID_UID = 0x80024504,
    SCE_KERNEL_ERROR_SYSMEM_CANNOT_ALLOCATE_UIDENTRY = 0x80024505,
    SCE_KERNEL_ERROR_NOT_PROCESS_UID = 0x80024506,
    SCE_KERNEL_ERROR_NOT_KERNEL_UID = 0x80024507,
    SCE_KERNEL_ERROR_INVALID_UID_CLASS = 0x80024508,
    SCE_KERNEL_ERROR_INVALID_UID_SUBCLASS = 0x80024509,
    SCE_KERNEL_ERROR_UID_CANNOT_FIND_BY_NAME = 0x8002450A,
    SCE_KERNEL_ERROR_UID_NOT_VISIBLE = 0x8002450B,
    SCE_KERNEL_ERROR_UID_MAX_OPEN = 0x8002450C,
    SCE_KERNEL_ERROR_UID_RL_OVERFLOW = 0x8002450D,
    SCE_KERNEL_ERROR_VIRPAGE_ERROR = 0x80024600,
    SCE_KERNEL_ERROR_ILLEGAL_VIRPAGE_TYPE = 0x80024601,
    SCE_KERNEL_ERROR_BLOCK_ERROR = 0x80024700,
    SCE_KERNEL_ERROR_ILLEGAL_BLOCK_ID = 0x80024701,
    SCE_KERNEL_ERROR_ILLEGAL_BLOCK_TYPE = 0x80024702,
    SCE_KERNEL_ERROR_BLOCK_IN_USE = 0x80024703,
    SCE_KERNEL_ERROR_PARTITION_ERROR = 0x80024800,
    SCE_KERNEL_ERROR_ILLEGAL_PARTITION_ID = 0x80024801,
    SCE_KERNEL_ERROR_ILLEGAL_PARTITION_INDEX = 0x80024802,
    SCE_KERNEL_ERROR_NO_L2PAGETABLE = 0x80024803,
    SCE_KERNEL_ERROR_HEAPLIB_ERROR = 0x80024900,
    SCE_KERNEL_ERROR_ILLEGAL_HEAP_ID = 0x80024901,
    SCE_KERNEL_ERROR_OUT_OF_RANG = 0x80024902,
    SCE_KERNEL_ERROR_HEAPLIB_NOMEM = 0x80024903,
    SCE_KERNEL_ERROR_HEAPLIB_VERIFY_ERROR = 0x80024904,
    SCE_KERNEL_ERROR_SYSMEM_ADDRESS_SPACE_ERROR = 0x80024A00,
    SCE_KERNEL_ERROR_INVALID_ADDRESS_SPACE_ID = 0x80024A01,
    SCE_KERNEL_ERROR_INVALID_PARTITION_INDEX = 0x80024A02,
    SCE_KERNEL_ERROR_ADDRESS_SPACE_CANNOT_FIND_PARTITION_BY_ADDR = 0x80024A03,
    SCE_KERNEL_ERROR_SYSMEM_MEMBLOCK_ERROR = 0x80024B00,
    SCE_KERNEL_ERROR_ILLEGAL_MEMBLOCK_TYPE = 0x80024B01,
    SCE_KERNEL_ERROR_ILLEGAL_MEMBLOCK_REMAP_TYPE = 0x80024B02,
    SCE_KERNEL_ERROR_NOT_PHY_CONT_MEMBLOCK = 0x80024B03,
    SCE_KERNEL_ERROR_ILLEGAL_MEMBLOCK_CODE = 0x80024B04,
    SCE_KERNEL_ERROR_ILLEGAL_MEMBLOCK_SIZE = 0x80024B05,
    SCE_KERNEL_ERROR_ILLEGAL_USERMAP_SIZE = 0x80024B06,
    SCE_KERNEL_ERROR_MEMBLOCK_TYPE_FOR_KERNEL_PROCESS = 0x80024B07,
    SCE_KERNEL_ERROR_PROCESS_CANNOT_REMAP_MEMBLOCK = 0x80024B08,
    SCE_KERNEL_ERROR_MEMBLOCK_RANGE_ERROR = 0x80024B09,
    SCE_KERNEL_ERROR_MEMBLOCK_TYPE_FOR_UPDATER_OR_SAFEMODE = 0x80024B0A,
    SCE_KERNEL_ERROR_MEMBLOCK_OVERFLOW = 0x80024B0B,
    SCE_KERNEL_ERROR_SYSMEM_PHYMEMLOW_ERROR = 0x80024C00,
    SCE_KERNEL_ERROR_CANNOT_ALLOC_PHYMEMLOW = 0x80024C01,
    SCE_KERNEL_ERROR_UNKNOWN_PHYMEMLOW_TYPE = 0x80024C02,
    SCE_KERNEL_ERROR_SYSMEM_BITHEAP_ERROR = 0x80024D00,
    SCE_KERNEL_ERROR_CANNOT_ALLOC_BITHEAP = 0x80024D01,
    SCE_KERNEL_ERROR_SYSMEM_NAMEHEAP_ERROR = 0x80024E00,
    SCE_KERNEL_ERROR_NO_SUCH_NAME = 0x80024E01,
    SCE_KERNEL_ERROR_DUPLICATE_NAME = 0x80024E02,
    SCE_KERNEL_ERROR_LOADCORE_ERROR = 0x80025000,
    SCE_KERNEL_ERROR_ILLEGAL_ELF_HEADER = 0x80025001,
    SCE_KERNEL_ERROR_ILLEGAL_SELF_HEADER = 0x80025002,
    SCE_KERNEL_ERROR_EXCPMGR_ERROR = 0x80027000,
    SCE_KERNEL_ERROR_ILLEGAL_EXCPCODE = 0x80027001,
    SCE_KERNEL_ERROR_ILLEGAL_EXCPHANDLER = 0x80027002,
    SCE_KERNEL_ERROR_NOTFOUND_EXCPHANDLER = 0x80027003,
    SCE_KERNEL_ERROR_CANNOT_RELEASE_EXCPHANDLER = 0x80027004,
    SCE_KERNEL_ERROR_INTRMGR_ERROR = 0x80027100,
    SCE_KERNEL_ERROR_ILLEGAL_CONTEXT = 0x80027101,
    SCE_KERNEL_ERROR_ILLEGAL_INTRCODE = 0x80027102,
    SCE_KERNEL_ERROR_ILLEGAL_INTRPARAM = 0x80027103,
    SCE_KERNEL_ERROR_ILLEGAL_INTRPRIORITY = 0x80027104,
    SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU = 0x80027105,
    SCE_KERNEL_ERROR_ILLEGAL_INTRFILTER = 0x80027106,
    SCE_KERNEL_ERROR_ILLEGAL_INTRTYPE = 0x80027107,
    SCE_KERNEL_ERROR_ILLEGAL_HANDLER = 0x80027108,
    SCE_KERNEL_ERROR_FOUND_HANDLER = 0x80027109,
    SCE_KERNEL_ERROR_NOTFOUND_HANDLER = 0x8002710A,
    SCE_KERNEL_ERROR_NO_MEMORY = 0x8002710B,
    SCE_KERNEL_ERROR_DMACMGR_ERROR = 0x80027200,
    SCE_KERNEL_ERROR_ALREADY_QUEUED = 0x80027201,
    SCE_KERNEL_ERROR_NOT_QUEUED = 0x80027202,
    SCE_KERNEL_ERROR_NOT_SETUP = 0x80027203,
    SCE_KERNEL_ERROR_ON_TRANSFERRING = 0x80027204,
    SCE_KERNEL_ERROR_NOT_INITIALIZED = 0x80027205,
    SCE_KERNEL_ERROR_TRANSFERRED = 0x80027206,
    SCE_KERNEL_ERROR_NOT_UNDER_CONTROL = 0x80027207,
    SCE_KERNEL_ERROR_CANCELING = 0x80027208,
    SCE_KERNEL_ERROR_SYSTIMER_ERROR = 0x80027300,
    SCE_KERNEL_ERROR_NO_FREE_TIMER = 0x80027301,
    SCE_KERNEL_ERROR_TIMER_NOT_ALLOCATED = 0x80027302,
    SCE_KERNEL_ERROR_TIMER_COUNTING = 0x80027303,
    SCE_KERNEL_ERROR_TIMER_STOPPED = 0x80027304,
    SCE_KERNEL_ERROR_THREADMGR_ERROR = 0x80028000,
    SCE_KERNEL_ERROR_UNKNOWN_UID = 0x80028001,
    SCE_KERNEL_ERROR_DIFFERENT_UID_CLASS = 0x80028002,
    SCE_KERNEL_ERROR_ALREADY_REGISTERED = 0x80028003,
    SCE_KERNEL_ERROR_CAN_NOT_WAIT = 0x80028004,
    SCE_KERNEL_ERROR_WAIT_TIMEOUT = 0x80028005,
    SCE_KERNEL_ERROR_WAIT_DELETE = 0x80028006,
    SCE_KERNEL_ERROR_WAIT_CANCEL = 0x80028007,
    SCE_KERNEL_ERROR_THREAD_ERROR = 0x80028020,
    SCE_KERNEL_ERROR_UNKNOWN_THREAD_ID = 0x80028021,
    SCE_KERNEL_ERROR_ILLEGAL_THREAD_ID = 0x80028022,
    SCE_KERNEL_ERROR_ILLEGAL_PRIORITY = 0x80028023,
    SCE_KERNEL_ERROR_ILLEGAL_STACK_SIZE = 0x80028024,
    SCE_KERNEL_ERROR_ILLEGAL_CPU_AFFINITY_MASK = 0x80028025,
    SCE_KERNEL_ERROR_ILLEGAL_THREAD_PARAM_COMBINATION = 0x80028026,
    SCE_KERNEL_ERROR_DORMANT = 0x80028027,
    SCE_KERNEL_ERROR_NOT_DORMANT = 0x80028028,
    SCE_KERNEL_ERROR_RUNNING = 0x80028029,
    SCE_KERNEL_ERROR_DELETED = 0x8002802A,
    SCE_KERNEL_ERROR_CAN_NOT_SUSPEND = 0x8002802B,
    SCE_KERNEL_ERROR_THREAD_STOPPED = 0x8002802C,
    SCE_KERNEL_ERROR_THREAD_SUSPENDED = 0x8002802D,
    SCE_KERNEL_ERROR_NOT_SUSPENDED = 0x8002802E,
    SCE_KERNEL_ERROR_ALREADY_DEBUG_SUSPENDED = 0x8002802F,
    SCE_KERNEL_ERROR_NOT_DEBUG_SUSPENDED = 0x80028030,
    SCE_KERNEL_ERROR_CAN_NOT_USE_VFP = 0x80028031,
    SCE_KERNEL_ERROR_THREAD_EVENT_ERROR = 0x80028060,
    SCE_KERNEL_ERROR_UNKNOWN_THREAD_EVENT_ID = 0x80028061,
    SCE_KERNEL_ERROR_KERNEL_TLS_ERROR = 0x80028080,
    SCE_KERNEL_ERROR_KERNEL_TLS_FULL = 0x80028081,
    SCE_KERNEL_ERROR_ILLEGAL_KERNEL_TLS_INDEX = 0x80028082,
    SCE_KERNEL_ERROR_KERNEL_TLS_BUSY = 0x80028083,
    SCE_KERNEL_ERROR_CALLBACK_ERROR = 0x800280A0,
    SCE_KERNEL_ERROR_UNKNOWN_CALLBACK_ID = 0x800280A1,
    SCE_KERNEL_ERROR_NOTIFY_CALLBACK = 0x800280A2,
    SCE_KERNEL_ERROR_CALLBACK_NOT_REGISTERED = 0x800280A3,
    SCE_KERNEL_ERROR_ALARM_ERROR = 0x800280C0,
    SCE_KERNEL_ERROR_UNKNOWN_ALARM_ID = 0x800280C1,
    SCE_KERNEL_ERROR_ALARM_CAN_NOT_CANCEL = 0x800280C2,
    SCE_KERNEL_ERROR_EVF_ERROR = 0x800280E0,
    SCE_KERNEL_ERROR_UNKNOWN_EVF_ID = 0x800280E1,
    SCE_KERNEL_ERROR_EVF_MULTI = 0x800280E2,
    SCE_KERNEL_ERROR_EVF_COND = 0x800280E3,
    SCE_KERNEL_ERROR_SEMA_ERROR = 0x80028100,
    SCE_KERNEL_ERROR_UNKNOWN_SEMA_ID = 0x80028101,
    SCE_KERNEL_ERROR_SEMA_ZERO = 0x80028102,
    SCE_KERNEL_ERROR_SEMA_OVF = 0x80028103,
    SCE_KERNEL_ERROR_SIGNAL_ERROR = 0x80028120,
    SCE_KERNEL_ERROR_ALREADY_SENT = 0x80028121,
    SCE_KERNEL_ERROR_MUTEX_ERROR = 0x80028140,
    SCE_KERNEL_ERROR_UNKNOWN_MUTEX_ID = 0x80028141,
    SCE_KERNEL_ERROR_MUTEX_RECURSIVE = 0x80028142,
    SCE_KERNEL_ERROR_MUTEX_LOCK_OVF = 0x80028143,
    SCE_KERNEL_ERROR_MUTEX_UNLOCK_UDF = 0x80028144,
    SCE_KERNEL_ERROR_MUTEX_FAILED_TO_OWN = 0x80028145,
    SCE_KERNEL_ERROR_MUTEX_NOT_OWNED = 0x80028146,
    SCE_KERNEL_ERROR_FAST_MUTEX_ERROR = 0x80028160,
    SCE_KERNEL_ERROR_UNKNOWN_FAST_MUTEX_ID = 0x80028161,
    SCE_KERNEL_ERROR_FAST_MUTEX_RECURSIVE = 0x80028162,
    SCE_KERNEL_ERROR_FAST_MUTEX_LOCK_OVF = 0x80028163,
    SCE_KERNEL_ERROR_FAST_MUTEX_FAILED_TO_OWN = 0x80028164,
    SCE_KERNEL_ERROR_FAST_MUTEX_NOT_OWNED = 0x80028165,
    SCE_KERNEL_ERROR_FAST_MUTEX_OWNED = 0x80028166,
    SCE_KERNEL_ERROR_FAST_MUTEX_ALREADY_INITIALIZED = 0x80028167,
    SCE_KERNEL_ERROR_FAST_MUTEX_NOT_INITIALIZED = 0x80028168,
    SCE_KERNEL_ERROR_LW_MUTEX_ERROR = 0x80028180,
    SCE_KERNEL_ERROR_UNKNOWN_LW_MUTEX_ID = 0x80028181,
    SCE_KERNEL_ERROR_LW_MUTEX_RECURSIVE = 0x80028182,
    SCE_KERNEL_ERROR_LW_MUTEX_LOCK_OVF = 0x80028183,
    SCE_KERNEL_ERROR_LW_MUTEX_UNLOCK_UDF = 0x80028184,
    SCE_KERNEL_ERROR_LW_MUTEX_FAILED_TO_OWN = 0x80028185,
    SCE_KERNEL_ERROR_LW_MUTEX_NOT_OWNED = 0x80028186,
    SCE_KERNEL_ERROR_COND_ERROR = 0x800281A0,
    SCE_KERNEL_ERROR_UNKNOWN_COND_ID = 0x800281A1,
    SCE_KERNEL_ERROR_WAIT_DELETE_MUTEX = 0x800281A2,
    SCE_KERNEL_ERROR_WAIT_CANCEL_MUTEX = 0x800281A3,
    SCE_KERNEL_ERROR_WAIT_DELETE_COND = 0x800281A4,
    SCE_KERNEL_ERROR_WAIT_CANCEL_COND = 0x800281A5,
    SCE_KERNEL_ERROR_LW_COND_ERROR = 0x800281C0,
    SCE_KERNEL_ERROR_UNKNOWN_LW_COND_ID = 0x800281C1,
    SCE_KERNEL_ERROR_WAIT_DELETE_LW_MUTEX = 0x800281C2,
    SCE_KERNEL_ERROR_WAIT_DELETE_LW_COND = 0x800281C3,
    SCE_KERNEL_ERROR_RW_LOCK_ERROR = 0x800281E0,
    SCE_KERNEL_ERROR_UNKNOWN_RW_LOCK_ID = 0x800281E1,
    SCE_KERNEL_ERROR_RW_LOCK_RECURSIVE = 0x800281E2,
    SCE_KERNEL_ERROR_RW_LOCK_LOCK_OVF = 0x800281E3,
    SCE_KERNEL_ERROR_RW_LOCK_NOT_OWNED = 0x800281E4,
    SCE_KERNEL_ERROR_RW_LOCK_UNLOCK_UDF = 0x800281E5,
    SCE_KERNEL_ERROR_RW_LOCK_FAILED_TO_LOCK = 0x800281E6,
    SCE_KERNEL_ERROR_RW_LOCK_FAILED_TO_UNLOCK = 0x800281E7,
    SCE_KERNEL_ERROR_EVENT_ERROR = 0x80028200,
    SCE_KERNEL_ERROR_UNKNOWN_EVENT_ID = 0x80028201,
    SCE_KERNEL_ERROR_EVENT_COND = 0x80028202,
    SCE_KERNEL_ERROR_MSG_PIPE_ERROR = 0x80028220,
    SCE_KERNEL_ERROR_UNKNOWN_MSG_PIPE_ID = 0x80028221,
    SCE_KERNEL_ERROR_MSG_PIPE_FULL = 0x80028222,
    SCE_KERNEL_ERROR_MSG_PIPE_EMPTY = 0x80028223,
    SCE_KERNEL_ERROR_MSG_PIPE_DELETED = 0x80028224,
    SCE_KERNEL_ERROR_TIMER_ERROR = 0x80028240,
    SCE_KERNEL_ERROR_UNKNOWN_TIMER_ID = 0x80028241,
    SCE_KERNEL_ERROR_EVENT_NOT_SET = 0x80028242,
    SCE_KERNEL_ERROR_SIMPLE_EVENT_ERROR = 0x80028260,
    SCE_KERNEL_ERROR_UNKNOWN_SIMPLE_EVENT_ID = 0x80028261,
    SCE_KERNEL_ERROR_PMON_ERROR = 0x80028280,
    SCE_KERNEL_ERROR_PMON_NOT_THREAD_MODE = 0x80028281,
    SCE_KERNEL_ERROR_PMON_NOT_CPU_MODE = 0x80028282,
    SCE_KERNEL_ERROR_WORK_QUEUE = 0x80028300,
    SCE_KERNEL_ERROR_UNKNOWN_WORK_QUEUE_ID = 0x80028301,
    SCE_KERNEL_ERROR_UNKNOWN_WORK_TASK_ID = 0x80028302,
    SCE_KERNEL_ERROR_PROCESSMGR_ERROR = 0x80029000,
    SCE_KERNEL_ERROR_INVALID_PID = 0x80029001,
    SCE_KERNEL_ERROR_INVALID_PROCESS_TYPE = 0x80029002,
    SCE_KERNEL_ERROR_PLS_FULL = 0x80029003,
    SCE_KERNEL_ERROR_INVALID_PROCESS_STATUS = 0x80029004,
    SCE_KERNEL_ERROR_PROCESS_CALLBACK_NOTFOUND = 0x80029005,
    SCE_KERNEL_ERROR_INVALID_BUDGET_ID = 0x80029006,
    SCE_KERNEL_ERROR_INVALID_BUDGET_SIZE = 0x80029007,
    SCE_KERNEL_ERROR_CP14_DISABLED = 0x80029008,
    SCE_KERNEL_ERROR_EXCEEDED_MAX_PROCESSES = 0x80029009,
    SCE_KERNEL_ERROR_PROCESS_REMAINING = 0x8002900A,
    SCE_KERNEL_ERROR_NO_PROCESS_DATA = 0x8002900B,
    SCE_KERNEL_ERROR_PROCESS_EVENT_INHIBITED = 0x8002900C,
    SCE_KERNEL_ERROR_IOFILEMGR_ERROR = 0x8002A000,
    SCE_KERNEL_ERROR_IO_NAME_TOO_LONG = 0x8002A001,
    SCE_KERNEL_ERROR_IO_REG_DEV = 0x8002A002,
    SCE_KERNEL_ERROR_IO_ALIAS_USED = 0x8002A003,
    SCE_KERNEL_ERROR_IO_DEL_DEV = 0x8002A004,
    SCE_KERNEL_ERROR_IO_WOULD_BLOCK = 0x8002A005,
    SCE_KERNEL_ERROR_MODULEMGR_START_FAILED = 0x8002D000,
    SCE_KERNEL_ERROR_MODULEMGR_STOP_FAIL = 0x8002D001,
    SCE_KERNEL_ERROR_MODULEMGR_IN_USE = 0x8002D002,
    SCE_KERNEL_ERROR_MODULEMGR_NO_LIB = 0x8002D003,
    SCE_KERNEL_ERROR_MODULEMGR_SYSCALL_REG = 0x8002D004,
    SCE_KERNEL_ERROR_MODULEMGR_NOMEM_LIB = 0x8002D005,
    SCE_KERNEL_ERROR_MODULEMGR_NOMEM_STUB = 0x8002D006,
    SCE_KERNEL_ERROR_MODULEMGR_NOMEM_SELF = 0x8002D007,
    SCE_KERNEL_ERROR_MODULEMGR_NOMEM = 0x8002D008,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_LIB = 0x8002D009,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_STUB = 0x8002D00A,
    SCE_KERNEL_ERROR_MODULEMGR_NO_FUNC_NID = 0x8002D00B,
    SCE_KERNEL_ERROR_MODULEMGR_NO_VAR_NID = 0x8002D00C,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_TYPE = 0x8002D00D,
    SCE_KERNEL_ERROR_MODULEMGR_NO_MOD_ENTRY = 0x8002D00E,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_PROC_PARAM = 0x8002D00F,
    SCE_KERNEL_ERROR_MODULEMGR_NO_MODOBJ = 0x8002D010,
    SCE_KERNEL_ERROR_MODULEMGR_NO_MOD = 0x8002D011,
    SCE_KERNEL_ERROR_MODULEMGR_NO_PROCESS = 0x8002D012,
    SCE_KERNEL_ERROR_MODULEMGR_OLD_LIB = 0x8002D013,
    SCE_KERNEL_ERROR_MODULEMGR_STARTED = 0x8002D014,
    SCE_KERNEL_ERROR_MODULEMGR_NOT_STARTED = 0x8002D015,
    SCE_KERNEL_ERROR_MODULEMGR_NOT_STOPPED = 0x8002D016,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_PROCESS_UID = 0x8002D017,
    SCE_KERNEL_ERROR_MODULEMGR_CANNOT_EXPORT_LIB_TO_SHARED = 0x8002D018,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_REL_INFO = 0x8002D019,
    SCE_KERNEL_ERROR_MODULEMGR_INVALID_REF_INFO = 0x8002D01A,
    SCE_KERNEL_ERROR_MODULEMGR_ELINK = 0x8002D01B,
    SCE_KERNEL_ERROR_MODULEMGR_NOENT = 0x8002D01C,
    SCE_KERNEL_ERROR_MODULEMGR_BUSY = 0x8002D01D,
    SCE_KERNEL_ERROR_MODULEMGR_NOEXEC = 0x8002D01E,
    SCE_KERNEL_ERROR_MODULEMGR_NAMETOOLONG = 0x8002D01F,
    SCE_KERNEL_ERROR_LIBRARYDB_NOENT = 0x8002D080,
    SCE_KERNEL_ERROR_LIBRARYDB_NO_LIB = 0x8002D081,
    SCE_KERNEL_ERROR_LIBRARYDB_NO_MOD = 0x8002D082,
    SCE_KERNEL_ERROR_PRELOAD_FAILED = 0x8002D0F0,
    SCE_KERNEL_ERROR_PRELOAD_LIBC_FAILED = 0x8002D0F1,
    SCE_KERNEL_ERROR_PRELOAD_FIOS2_FAILED = 0x8002D0F2,
    SCE_KERNEL_ERROR_AUTHFAIL = 0x8002F000,
    SCE_KERNEL_ERROR_NO_AUTH = 0x8002F001,
    SCE_KERNEL_ERROR_MPP_EMPTY = 0x800201b4
};

enum SceEventFlagWaitTypes {
    SCE_EVENT_WAITAND = 0,
    SCE_EVENT_WAITOR = 1,
    SCE_EVENT_WAITCLEAR = 2,
    SCE_EVENT_WAITCLEAR_PAT = 4
};

enum SceKernelMemBlockType {
    SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE = 0x0C208060,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_RX = 0x0C20D050,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_RW = 0x0C20D060,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_RW = 0x0C80D060,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW = 0x0D808060,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW = 0x09408060
};

enum SceSysmoduleModuleId {
    SCE_SYSMODULE_INVALID = 0x0000,
    SCE_SYSMODULE_NET = 0x0001,
    SCE_SYSMODULE_HTTP = 0x0002,
    SCE_SYSMODULE_SSL = 0x0003,
    SCE_SYSMODULE_HTTPS = 0x0004,
    SCE_SYSMODULE_PERF = 0x0005,
    SCE_SYSMODULE_FIBER = 0x0006,
    SCE_SYSMODULE_ULT = 0x0007,
    SCE_SYSMODULE_DBG = 0x0008,
    SCE_SYSMODULE_RAZOR_CAPTURE = 0x0009,
    SCE_SYSMODULE_RAZOR_HUD = 0x000A,
    SCE_SYSMODULE_NGS = 0x000B,
    SCE_SYSMODULE_SULPHA = 0x000C,
    SCE_SYSMODULE_SAS = 0x000D,
    SCE_SYSMODULE_PGF = 0x000E,
    SCE_SYSMODULE_APPUTIL = 0x000F,
    SCE_SYSMODULE_FIOS2 = 0x0010,
    SCE_SYSMODULE_IME = 0x0011,
    SCE_SYSMODULE_NP_BASIC = 0x0012,
    SCE_SYSMODULE_SYSTEM_GESTURE = 0x0013,
    SCE_SYSMODULE_LOCATION = 0x0014,
    SCE_SYSMODULE_NP = 0x0015,
    SCE_SYSMODULE_PHOTO_EXPORT = 0x0016,
    SCE_SYSMODULE_XML = 0x0017,
    SCE_SYSMODULE_NP_COMMERCE2 = 0x0018,
    SCE_SYSMODULE_NP_UTILITY = 0x0019,
    SCE_SYSMODULE_VOICE = 0x001A,
    SCE_SYSMODULE_VOICEQOS = 0x001B,
    SCE_SYSMODULE_NP_MATCHING2 = 0x001C,
    SCE_SYSMODULE_SCREEN_SHOT = 0x001D,
    SCE_SYSMODULE_NP_SCORE_RANKING = 0x001E,
    SCE_SYSMODULE_SQLITE = 0x001F,
    SCE_SYSMODULE_TRIGGER_UTIL = 0x0020,
    SCE_SYSMODULE_RUDP = 0x0021,
    SCE_SYSMODULE_CODECENGINE_PERF = 0x0022,
    SCE_SYSMODULE_LIVEAREA = 0x0023,
    SCE_SYSMODULE_NP_ACTIVITY = 0x0024,
    SCE_SYSMODULE_NP_TROPHY = 0x0025,
    SCE_SYSMODULE_NP_MESSAGE = 0x0026,
    SCE_SYSMODULE_SHUTTER_SOUND = 0x0027,
    SCE_SYSMODULE_CLIPBOARD = 0x0028,
    SCE_SYSMODULE_NP_PARTY = 0x0029,
    SCE_SYSMODULE_NET_ADHOC_MATCHING = 0x002A,
    SCE_SYSMODULE_NEAR_UTIL = 0x002B,
    SCE_SYSMODULE_NP_TUS = 0x002C,
    SCE_SYSMODULE_MP4 = 0x002D,
    SCE_SYSMODULE_AACENC = 0x002E,
    SCE_SYSMODULE_HANDWRITING = 0x002F,
    SCE_SYSMODULE_ATRAC = 0x0030,
    SCE_SYSMODULE_NP_SNS_FACEBOOK = 0x0031,
    SCE_SYSMODULE_VIDEO_EXPORT = 0x0032,
    SCE_SYSMODULE_NOTIFICATION_UTIL = 0x0033,
    SCE_SYSMODULE_BG_APP_UTIL = 0x0034,
    SCE_SYSMODULE_INCOMING_DIALOG = 0x0035,
    SCE_SYSMODULE_IPMI = 0x0036,
    SCE_SYSMODULE_AUDIOCODEC = 0x0037,
    SCE_SYSMODULE_FACE = 0x0038,
    SCE_SYSMODULE_SMART = 0x0039,
    SCE_SYSMODULE_MARLIN = 0x003A,
    SCE_SYSMODULE_MARLIN_DOWNLOADER = 0x003B,
    SCE_SYSMODULE_MARLIN_APP_LIB = 0x003C,
    SCE_SYSMODULE_TELEPHONY_UTIL = 0x003D,
    SCE_SYSMODULE_PSPNET_ADHOC = 0x0043,
    SCE_SYSMODULE_DTCP_IP = 0x0044,
    SCE_SYSMODULE_VIDEO_SEARCH_EMPR = 0x0045,
    SCE_SYSMODULE_NP_SIGNALING = 0x0046,
    SCE_SYSMODULE_BEISOBMF = 0x0047,
    SCE_SYSMODULE_BEMP2SYS = 0x0048,
    SCE_SYSMODULE_MUSIC_EXPORT = 0x0049,
    SCE_SYSMODULE_NEAR_DIALOG_UTIL = 0x004A,
    SCE_SYSMODULE_LOCATION_EXTENSION = 0x004B,
    SCE_SYSMODULE_AVPLAYER = 0x004C,
    SCE_SYSMODULE_GAME_UPDATE = 0x004D,
    SCE_SYSMODULE_MAIL_API = 0x004E,
    SCE_SYSMODULE_TELEPORT_CLIENT = 0x004F,
    SCE_SYSMODULE_TELEPORT_SERVER = 0x0050,
    SCE_SYSMODULE_MP4_RECORDER = 0x0051,
    SCE_SYSMODULE_APPUTIL_EXT = 0x0052,
    SCE_SYSMODULE_NP_WEBAPI = 0x0053,
    SCE_SYSMODULE_AVCDEC = 0x0054,
    SCE_SYSMODULE_JSON = 0x0055
};

enum SceSysmoduleInternalModuleId {
    SCE_SYSMODULE_INTERNAL_JPEG_ENC_ARM = 0x80000001, //!< sceJpegEncArm module
    SCE_SYSMODULE_INTERNAL_AUDIOCODEC = 0x80000002, //!< AudioCodec module
    SCE_SYSMODULE_INTERNAL_JPEG_ARM = 0x80000003, //!< sceJpegArm module
    SCE_SYSMODULE_INTERNAL_G729 = 0x80000004, //!< G729 module
    SCE_SYSMODULE_INTERNAL_BXCE = 0x80000005, //!< BXCE module
    SCE_SYSMODULE_INTERNAL_INI_FILE_PROCESSOR = 0x80000006, //!< INI File Processor module
    SCE_SYSMODULE_INTERNAL_NP_ACTIVITY_NET = 0x80000007, //!< NpActivityNet module
    SCE_SYSMODULE_INTERNAL_PAF = 0x80000008, //!< Paf module
    SCE_SYSMODULE_INTERNAL_SQLITE_VSH = 0x80000009, //!< sceSqliteVsh module
    SCE_SYSMODULE_INTERNAL_DBUTIL = 0x8000000A, //!< DBUtil module
    SCE_SYSMODULE_INTERNAL_ACTIVITY_DB = 0x8000000B, //!< ActivityDB module
    SCE_SYSMODULE_INTERNAL_COMMON_GUI_DIALOG = 0x8000000C, //!< Common Gui Dialog module
    SCE_SYSMODULE_INTERNAL_STORE_CHECKOUT = 0x8000000D, //!< Store Checkout module
    SCE_SYSMODULE_INTERNAL_IME_DIALOG = 0x8000000E, //!< Ime Dialog module
    SCE_SYSMODULE_INTERNAL_PHOTO_IMPORT_DIALOG = 0x8000000F, //!< Photo Import Dialog module
    SCE_SYSMODULE_INTERNAL_PHOTO_REVIEW_DIALOG = 0x80000010, //!< Photo Review Dialog module
    SCE_SYSMODULE_INTERNAL_CHECKOUT_DIALOG = 0x80000011, //!< Checkout Dialog module
    SCE_SYSMODULE_INTERNAL_COMMON_DIALOG_MAIN = 0x80000012, //!< Common Dialog Main module
    SCE_SYSMODULE_INTERNAL_MSG_DIALOG = 0x80000013, //!< sceMsgDialog module
    SCE_SYSMODULE_INTERNAL_NET_CHECK_DIALOG = 0x80000014, //!< NetCheck Dialog module
    SCE_SYSMODULE_INTERNAL_SAVEDATA_DIALOG = 0x80000015, //!< SaveData Dialog module
    SCE_SYSMODULE_INTERNAL_NP_MESSAGE_DIALOG = 0x80000016, //!< NpMessage Dialog module
    SCE_SYSMODULE_INTERNAL_TROPHY_SETUP_DIALOG = 0x80000017, //!< Trophy Setup Dialog module
    SCE_SYSMODULE_INTERNAL_FRIEND_LIST_DIALOG = 0x80000018, //!< FriendList Dialog module
    SCE_SYSMODULE_INTERNAL_NEAR_PROFILE = 0x8000001A, //!< Near Profile module
    SCE_SYSMODULE_INTERNAL_NP_FRIEND_PRIVACY_LEVEL = 0x8000001B, //!< NpFriend Privacy Level module
    SCE_SYSMODULE_INTERNAL_NP_COMMERCE2 = 0x8000001D, //!< NpCommerce2 module
    SCE_SYSMODULE_INTERNAL_NP_KDC = 0x8000001E, //!< NpKdc module
    SCE_SYSMODULE_INTERNAL_MUSIC_EXPORT = 0x8000001F, //!< Music Export module
    SCE_SYSMODULE_INTERNAL_VIDEO_EXPORT = 0x80000020, //!< Video Export module
    SCE_SYSMODULE_INTERNAL_NP_MESSAGE_DIALOG_IMPL = 0x80000021, //!< NpMessageDlg module
    SCE_SYSMODULE_INTERNAL_NP_MESSAGE_CONTACTS = 0x80000022, //!< NpMessageContacts module
    SCE_SYSMODULE_INTERNAL_DB_RECOVERY_UTILITY = 0x80000023, //!< DBRecoveryUtil module
    SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL = 0x80000024, //!< scePromoterUtil module
    SCE_SYSMODULE_INTERNAL_PARTY_MEMBER_LIST = 0x80000026, //!< Party Member List module
    SCE_SYSMODULE_INTERNAL_ULT = 0x80000025, //!< Ult module
    SCE_SYSMODULE_INTERNAL_DRM_PSM_KDC = 0x80000027, //!< DRM PSM KDC module
    SCE_SYSMODULE_INTERNAL_LOCATION_INTERNAL = 0x80000028, //!< sceLocation module
    SCE_SYSMODULE_INTERNAL_LOCATION_FACTORY = 0x80000029 //!< Location Factory module
};

enum SceSysmoduleErrorCode {
    SCE_SYSMODULE_LOADED = 0,
    SCE_SYSMODULE_ERROR_INVALID_VALUE = 0x805A1000,
    SCE_SYSMODULE_ERROR_UNLOADED = 0x805A1001,
    SCE_SYSMODULE_ERROR_FATAL = 0x805A10FF
};

typedef SceUInt64 SceKernelSysClock;
typedef Ptr<int(SceSize args, Ptr<void> argp)> SceKernelThreadEntry;

struct SceKernelSegmentInfo {
    SceSize size; //!< this structure size (0x18)
    SceUInt perms; //!< probably rwx in low bits
    Ptr<const void> vaddr; //!< address in memory
    SceSize memsz; //!< size in memory
    SceSize filesz; //!< original size of memsz
    SceUInt res; //!< unused
};

static_assert(sizeof(SceKernelSegmentInfo) == 0x18);

struct SceKernelModuleInfo {
    SceSize size; //!< 0x1B8 for Vita 1.x
    SceUID modid;
    uint16_t modattr;
    uint8_t modver[2];
    char module_name[28];
    SceUInt unk28;
    Ptr<const void> start_entry;
    Ptr<const void> stop_entry;
    Ptr<const void> exit_entry;
    Ptr<const void> exidx_top;
    Ptr<const void> exidx_btm;
    Ptr<const void> extab_top;
    Ptr<const void> extab_btm;
    Ptr<const void> tlsInit;
    SceSize tlsInitSize;
    SceSize tlsAreaSize;
    char path[256];
    SceKernelSegmentInfo segments[MODULE_INFO_NUM_SEGMENTS];
    SceUInt state; //!< see:SceKernelModuleState
};

struct SceKernelStartModuleOpt {
    SceSize size;
    SceUInt32 flags;
    SceUInt32 prologue;
    SceUInt32 start;
};

static_assert(sizeof(SceKernelModuleInfo) == 0x1B8);

struct SceKernelMemBlockInfo {
    SceSize size;
    Ptr<void> mappedBase;
    SceSize mappedSize;
    int memoryType;
    SceUInt32 access;
    SceKernelMemBlockType type;
};

struct SceKernelSimpleEventOptParam {
    SceSize size;
};

struct SceKernelEventFlagOptParam {
    SceSize size;
};

struct SceKernelCondOptParam {
    SceSize size;
};

struct SceKernelLwCondOptParam {
    SceSize size;
};

struct SceKernelLwMutexOptParam {
    SceSize size;
};

struct SceKernelMutexOptParam {
    /** Size of the ::SceKernelMutexOptParam structure. */
    SceSize size;
    int ceilingPriority;
};

struct SceKernelSemaOptParam {
    /** Size of the ::SceKernelSemaOptParam structure. */
    SceSize size;
};

struct SceKernelLMOption {
    SceSize size;
};

// We only use workarea for uid
struct SceKernelLwMutexWork {
    std::uint32_t owner;
    std::uint32_t unknown0;
    std::uint32_t lockCount;
    std::uint32_t attr;
    SceUID uid;
    std::uint32_t unknown1[3];
};

static_assert(sizeof(SceKernelLwMutexWork) == 32, "Incorrect size");

struct SceKernelLwMutexInfo {
    SceSize size; /**< Size of this structure (sizeof(SceKernelLwMutexInfo)) */
    SceUID uid; /**< Lightweight mutex identifier */
    char name[KERNELOBJECT_MAX_NAME_LENGTH + 1]; /**< Name of the mutex specified by sceKernelCreateLwMutex() */
    SceUInt32 attr; /**< Mutex attributes specified with sceKernelCreateLwMutex() */
    Ptr<SceKernelLwMutexWork> pWork; /**< Mutex work area specified by sceKernelCreateLwMutex() */
    SceInt32 initCount; /**< Initial number of lw mutex locks specified by sceKernelCreateLwMutex() */
    SceInt32 currentCount; /**< Current lock count of lightweight mutex */
    SceUID currentOwnerId; /**< Identifier of the thread that currently owns the lightweight mutex */
    SceUInt32 numWaitThreads; /**< Number of threads waiting for the lightweight mutex */
};

// We only use workarea for uid
struct SceKernelLwCondWork {
    SceUID uid;

    std::uint8_t padding[28];
};

struct SceKernelCreateLwMutex_opt {
    int init_count;
    Ptr<SceKernelLwMutexOptParam> opt_param;
    uint32_t unk_8;
    uint32_t unk_C;
};

struct SceKernelCreateLwCond_opt {
    Ptr<SceKernelLwMutexWork> workarea_mutex;
    Ptr<SceKernelLwCondOptParam> opt_param;
    uint32_t unk_8;
    uint32_t unk_C;
};

struct SceKernelCreateSema_opt {
    int maxVal;
    Ptr<SceKernelSemaOptParam> option;
};

struct SceKernelCreateMsgPipeOpt {
    /** Size of the structure */
    SceSize size;
    /** == 0x00000100U if msgpipe uses openLimit */
    uint32_t attr;
    uint32_t reserved[2];
    uint32_t opneLimit;
};

struct SceKernelMsgPipeInfo {
    /** Size of the structure */
    SceSize size;
    SceUID id;
    char name[32];
    /** == 0x00000100U if msgpipe uses openLimit */
    uint32_t attr;
    SceSize bufferSize;
    SceSize freeSize;
    /** Number of waiting threads */
    uint32_t numSenders;
    uint32_t numReceivers;
};

struct SceKernelCondInfo {
    SceSize size;
    SceUID condId;
    char name[KERNELOBJECT_MAX_NAME_LENGTH + 1];
    SceUInt32 attr;
    SceUID mutexId;
    SceUInt32 numWaitThreads;
};

struct SceKernelEventFlagInfo {
    SceSize size;
    SceUID evfId;
    char name[KERNELOBJECT_MAX_NAME_LENGTH + 1];
    SceUInt32 attr;
    SceUInt32 initPattern;
    SceUInt32 currentPattern;
    SceUInt32 numWaitThreads;
};

struct SceKernelSemaInfo {
    SceSize size;
    SceUID semaId;
    char name[KERNELOBJECT_MAX_NAME_LENGTH + 1];
    SceUInt32 attr;
    SceInt32 initCount;
    SceInt32 currentCount;
    SceInt32 maxCount;
    SceUInt32 numWaitThreads;
};

struct SceKernelThreadInfo {
    /** Size of the structure */
    SceSize size;
    /** The UID of the process where the thread belongs */
    SceUID processId;
    /** Nul terminated name of the thread */
    char name[32];
    /** Thread attributes */
    SceUInt32 attr;
    /** Thread status */
    SceUInt32 status;
    /** Thread entry point */
    SceKernelThreadEntry entry;
    /** Thread stack pointer */
    Ptr<void> stack;
    /** Thread stack size */
    SceInt32 stackSize;
    /** Initial priority */
    SceInt32 initPriority;
    /** Current priority */
    SceInt32 currentPriority;
    /** Initial CPU affinity mask */
    SceInt32 initCpuAffinityMask;
    /** Current CPU affinity mask */
    SceInt32 currentCpuAffinityMask;
    /** Current CPU ID */
    SceInt32 currentCpuId;
    /** Last executed CPU ID */
    SceInt32 lastExecutedCpuId;
    /** Wait type */
    SceUInt32 waitType;
    /** Wait id */
    SceUID waitId;
    /** Exit status of the thread */
    SceInt32 exitStatus;
    /** Number of clock cycles run */
    SceKernelSysClock runClocks;
    /** Interrupt preemption count */
    SceUInt32 intrPreemptCount;
    /** Thread preemption count */
    SceUInt32 threadPreemptCount;
    /** Thread release count */
    SceUInt32 threadReleaseCount;
    /** Number of CPUs to which the thread is moved */
    SceInt32 changeCpuCount;
    /** Function notify callback UID */
    SceInt32 fNotifyCallback;
    /** Reserved */
    SceInt32 reserved;
};

struct SceKernelThreadCpuRegisterInfo {
    SceSize size;
    SceUInt32 cpsr;
    SceUInt32 reg[16];
    SceUInt32 tpidrurw;
    SceUInt32 teehbr;
    SceUInt32 sb;
    SceUInt32 st;
};

struct SceKernelThreadVfpRegisterInfo {
    SceSize size;
    SceUInt32 fpscr;
    SceFloat reg[64];
};

struct SceProcessParam {
    SceSize size;
    SceUInt32 magic;
    SceUInt32 version;
    SceUInt32 fw_version;
    Ptr<char> main_thread_name;
    SceInt32 main_thread_priority;
    SceUInt32 main_thread_stacksize;
    SceUInt32 main_thread_attribute;
    Ptr<char> process_name;
    SceUInt32 process_preload_disabled;
    SceUInt32 main_thread_cpu_affinity_mask;
    Ptr<void> sce_libc_param;
    SceUInt32 unk;
};

struct SceKernelThreadOptParam {
    /** Size of the ::SceKernelThreadOptParam structure. */
    SceSize size;
    /** Attributes */
    SceUInt32 attr;
};

struct SceKernelCreateThread_opt {
    SceSize size;
    int stack_size;
    SceUInt attr;
    int cpu_affinity_mask;
    Ptr<SceKernelThreadOptParam> option;
};

typedef SceInt32(SceKernelCallbackFunction)(SceUID notifyId, SceInt32 notifyCount, SceInt32 notifyArg, void *pCommon);

struct SceKernelCallbackInfo {
    SceSize size;
    SceUID callbackId;
    char name[KERNELOBJECT_MAX_NAME_LENGTH + 1];
    SceUInt32 attr;
    SceUID threadId;
    Ptr<SceKernelCallbackFunction> callbackFunc;
    SceUID notifyId;
    SceInt32 notifyCount;
    SceInt32 notifyArg;
    Ptr<void> pCommon;
};
