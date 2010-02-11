#ifndef _INCLUDE_LIBR_BP_H_
#define _INCLUDE_LIBR_BP_H_

#include <r_types.h>
#include <r_io.h>
#include "list.h"

#define R_BP_MAXPIDS 10
#define R_BP_CONT_NORMAL 0
#define R_BP_CONT_NORMAL 0

typedef struct r_bp_arch_t {
	int length;
	int endian;
	const ut8 *bytes;
} RBreakpointArch;

enum {
	R_BP_TYPE_SW,
	R_BP_TYPE_HW,
	R_BP_TYPE_COND,
	R_BP_TYPE_FAULT,
	R_BP_TYPE_DELETE,
};

typedef struct r_bp_handle_t {
	char *name;
	char *arch;
	int type; // R_BP_TYPE_SW
	int nbps;
	struct r_bp_arch_t *bps;
	struct list_head list;
} RBreakpointHandler;

typedef struct r_bp_item_t {
	ut64 addr;
	int size; /* size of breakpoint area */
	int recoil; /* recoil */
	int rwx;
	int hw;
	int trace;
	int enabled;
	int hits;
	ut8 *obytes; /* original bytes */
	ut8 *bbytes; /* breakpoint bytes */
	int pids[R_BP_MAXPIDS];
	struct list_head list;
} RBreakpointItem;

typedef int (*RBreakpointCallback)(void *user, int type, ut64 addr, int hw, int rwx);

typedef struct r_bp_t {
	int trace_all;
	ut64 trace_bp;
	int nbps;
	int stepcont;
	RBreakpointCallback breakpoint;
	struct r_io_bind_t iob; // compile time dependency
	struct r_bp_handle_t *cur;
	struct list_head plugins;
	struct list_head bps;
} RBreakpoint;

enum {
	R_BP_PROT_READ = 1,
	R_BP_PROT_WRITE = 2,
	R_BP_PROT_EXEC = 4,
};

#ifdef R_API
R_API RBreakpoint *r_bp_init(RBreakpoint *bp);
R_API RBreakpoint *r_bp_new();
R_API RBreakpoint *r_bp_free(RBreakpoint *bp);

R_API int r_bp_del(RBreakpoint *bp, ut64 addr);

R_API int r_bp_handle_add(RBreakpoint *bp, struct r_bp_handle_t *foo);
R_API int r_bp_use(RBreakpoint *bp, const char *name);
R_API int r_bp_handle_del(RBreakpoint *bp, const char *name);
R_API void r_bp_handle_list(RBreakpoint *bp);

R_API int r_bp_in(RBreakpoint *bp, ut64 addr, int rwx);
R_API int r_bp_list(RBreakpoint *bp, int rad);
R_API int r_bp_get_bytes(RBreakpoint *bp, ut8 *buf, int len, int endian, int idx);
R_API int r_bp_set_trace(RBreakpoint *bp, ut64 addr, int set);
//R_API int r_bp_set_trace_bp(RBreakpoint *bp, ut64 addr, int set);
R_API RBreakpointItem *r_bp_enable(RBreakpoint *bp, ut64 addr, int set);

R_API int r_bp_add_cond(RBreakpoint *bp, const char *cond);
R_API int r_bp_del_cond(RBreakpoint *bp, int idx);
R_API int r_bp_add_fault(RBreakpoint *bp, ut64 addr, int size, int rwx);

R_API RBreakpointItem *r_bp_add_sw(RBreakpoint *bp, ut64 addr, int size, int rwx);
R_API RBreakpointItem *r_bp_add_hw(RBreakpoint *bp, ut64 addr, int size, int rwx);
R_API RBreakpointItem *r_bp_at_addr(RBreakpoint *bp, ut64 addr, int rwx);
R_API int r_bp_restore(RBreakpoint *bp, int set);
R_API int r_bp_recoil(RBreakpoint *bp, ut64 addr);
#endif

/* plugin pointers */
extern struct r_bp_handle_t r_bp_plugin_x86;
extern struct r_bp_handle_t r_bp_plugin_arm;
#if 0
extern struct r_bp_handle_t r_bp_plugin_powerpc;
extern struct r_bp_handle_t r_bp_plugin_mips;
extern struct r_bp_handle_t r_bp_plugin_sparc;
#endif

#endif
