/* Copyright IBM Corp. 2013, 2016 */

#ifndef QUERY_CAPACITY_INT
#define QUERY_CAPACITY_INT

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <iconv.h>
#include <inttypes.h>
#include <linux/types.h>
#include <unistd.h>

#include "query_capacity.h"


/* Miscellaneous structures and constants */
#define STR_BUF_SIZE		257

#define ATTR_SRC_SYSINFO	'S'
#define ATTR_SRC_SYSFS		'F'
#define ATTR_SRC_HYPFS		'H'
#define ATTR_SRC_STHYI		'V'
#define ATTR_SRC_POSTPROC	'P'	// Note: Post-processed attributes can have multiple origins - would be
					//       complicated to figure out accurately. We leave it at 'P' for now
#define ATTR_SRC_UNDEF		'_'

#ifndef htobe16	// fallbacks for systems with a glibc < 2.9
#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define htobe16(x)	bswap_16(x)
	#define htobe32(x)	bswap_32(x)
	#define htobe64(x)	bswap_64(x)
#else
	#define htobe16(x)	x
	#define htobe32(x)	x
	#define htobe64(x)	x
#endif // __BYTE_ORDER
#endif // htobe32

struct qc_handle {
	void		 *layer;	// holds a copy of the respective *_values struct
					// and is filled by looking up the offset via the respective *_attrs table
	struct qc_attr	 *attr_list;
	int 		  layer_no;
	int 		 *attr_present;	// array indicating whether attributes are set
	char		 *src;		// array indicating the source of the attribute's value, see ATTR_SRC_*
	struct qc_handle *next;
	struct qc_handle *root;		// points to top handle
};

struct qc_data_src {
	int  (*open)(struct qc_handle *, char **);
	int  (*process)(struct qc_handle *, char *);
	void (*dump)(struct qc_handle *, char *);
	void (*close)(struct qc_handle *, char *);
	int  (*lgm_check)(struct qc_handle *, const char *);
	char *priv;
};

extern struct qc_data_src sysinfo, sysfs, hypfs, sthyi;

/* Utility functions */
int qc_ebcdic_to_ascii(struct qc_handle *hdl, char *inbuf, size_t insz);
int qc_is_nonempty_ebcdic(__u64 *str);
int qc_new_handle(struct qc_handle *hdl, struct qc_handle **tgthdl, int layer_no, int layer_type);
// Insert new layer 'inserted_hdl' of type 'type' before 'hdl'. Won't support inserting a new root
int qc_insert_handle(struct qc_handle *hdl, struct qc_handle **inserted_hdl, int type);
// Insert new layer 'appended_hdl' of type 'type' after 'hdl'
int qc_append_handle(struct qc_handle *hdl, struct qc_handle **appended_hdl, int type);
struct qc_handle *qc_get_cec_handle(struct qc_handle *hdl);
struct qc_handle *qc_get_lpar_handle(struct qc_handle *hdl);
struct qc_handle *qc_get_root_handle(struct qc_handle *hdl);
struct qc_handle *qc_get_prev_handle(struct qc_handle *hdl);
struct qc_handle *qc_get_top_handle(struct qc_handle *hdl);

/* Debugging-related functions and variables */
extern long  qc_dbg_level;
extern FILE *qc_dbg_file;
extern char *qc_dbg_dump_dir;
extern char *qc_dbg_use_dump;
extern int   qc_dbg_indent;
extern int   qc_consistency_check_requested;
void qc_debug_indent_inc();
void qc_debug_indent_dec();
void qc_mark_dump_incomplete(struct qc_handle *hdl, char *missing_component);


#ifdef CONFIG_DEBUG_TIMESTAMPS
#define qc_debug(hdl, arg, ...)	do {if (qc_dbg_level > 0) { \
					time_t t; \
					struct tm *tm; \
					time(&t); \
					tm = localtime(&t); \
					fprintf(qc_dbg_file, "%02d/%02d,%02d:%02d:%02d,%-10p: %*s" arg, \
					tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, qc_get_root_handle(hdl), qc_dbg_indent, "", ##__VA_ARGS__); \
				} }while(0);
#else
#define qc_debug(hdl, arg, ...)	do {if (qc_dbg_level > 0) { \
					fprintf(qc_dbg_file, "%-10p: %*s" arg, qc_get_root_handle(hdl), qc_dbg_indent, "", ##__VA_ARGS__); \
				} } while(0);
#endif
#endif
