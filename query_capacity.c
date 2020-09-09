/* Copyright IBM Corp. 2013, 2020 */

#define _GNU_SOURCE

#include <sys/stat.h>

#include "query_capacity_data.h"


long  qc_dbg_level;
FILE *qc_dbg_file;
char *qc_dbg_dump_dir;
int   qc_dbg_indent;
char *qc_dbg_use_dump;
int   qc_consistency_check_requested;
static char	    *qc_dbg_file_name;
static long	     qc_dbg_autodump;
static unsigned int  qc_dbg_dump_idx;
static iconv_t	     qc_cd = (iconv_t)-1;

struct qc_reg_hdl {
	struct qc_handle	*hdl;
	struct qc_reg_hdl	*next;
};

static struct qc_reg_hdl *qc_hdls = NULL;

static void __attribute__((destructor)) qc_destructor(void) {
	if (qc_cd != (iconv_t)-1)
		iconv_close(qc_cd);
}

struct qc_handle *qc_get_cec_handle(struct qc_handle *hdl) {
	return hdl ? hdl->root : hdl;
}

struct qc_handle *qc_get_lpar_handle(struct qc_handle *hdl) {
	for (hdl = hdl->root; hdl != NULL && *(int *)(hdl->layer) != QC_LAYER_TYPE_LPAR; hdl = hdl->next);

	return hdl;
}

/* Update dbg_level from environment variable */
static void qc_update_dbg_level(void) {
	char *s, *end;

	s = getenv("QC_DEBUG");
	if (s) {
		qc_dbg_level = strtol(s, &end, 10);
		if (end == s || qc_dbg_level < 0)
			qc_dbg_level = 0;
	}
#ifdef CONFIG_DUMP_READING
	s = getenv("QC_USE_DUMP");
	// if qc_dbg_use_dump is NULL, then there's nothing we can do about it
	if (s) {
		free(qc_dbg_use_dump);
		qc_dbg_use_dump = strdup(s);
	}
#endif
	s = getenv("QC_AUTODUMP");
	if (s) {
		qc_dbg_autodump = strtol(s, &end, 10);
		if (end == s || qc_dbg_autodump < 0)
			qc_dbg_autodump = 0;
	}
}

static void qc_debug_deinit(void *hdl) {
	qc_update_dbg_level();
	if (qc_dbg_level <= 0 && qc_dbg_autodump <= 0 && qc_dbg_file) {
		qc_dbg_level = 1;	// temporarily set, or qc_debug won't print anything
		qc_debug(hdl, "Log level set to %ld, closing\n", qc_dbg_level);
		qc_dbg_level = 0;
		fclose(qc_dbg_file);
		qc_dbg_file = NULL;
		free(qc_dbg_dump_dir);
		qc_dbg_dump_dir = NULL;
		free(qc_dbg_file_name);
		qc_dbg_file_name = NULL;
		qc_dbg_dump_idx = 0;
		qc_dbg_autodump = 0;
	}
	free(qc_dbg_use_dump);
	qc_dbg_use_dump = NULL;
}

#define QC_DBGFILE		"/tmp/qclib-XXXXXX"
static int qc_debug_file_init(void) {
	int fd;

	if (!qc_dbg_file_name) {
		char *s = getenv("QC_DEBUG_FILE");
		if (s) {
			qc_dbg_file_name = strdup(s);
			if (!qc_dbg_file_name)
				goto out_err;
			qc_dbg_file = fopen(qc_dbg_file_name, "w");
			if (!qc_dbg_file)
				goto out_err;
		} else {
			qc_dbg_file_name = strdup(QC_DBGFILE);
			if (!qc_dbg_file_name)
				goto out_err;
			fd = mkstemp(qc_dbg_file_name);
			if (fd == -1)
				goto out_err;
			qc_dbg_file = fdopen(fd, "w");
			if (!qc_dbg_file) {
				close(fd);
				goto out_err;
			}
		}
		qc_debug(NULL, "This is qclib v2.2.0, level 33725a4f, date 2020-09-09 17:55:45 +0200\n");
	}

	return 0;

out_err:
	free(qc_dbg_file_name);
	qc_dbg_file_name = NULL;
	qc_dbg_level = 0;

	return -1;
}

static int qc_debug_open_dump_dir(struct qc_handle *hdl) {
	int i;

	if (!qc_dbg_file_name && qc_debug_file_init())
		return -1;
	for (i = 0, ++qc_dbg_dump_idx; i < 100; ++i, ++qc_dbg_dump_idx) {
		free(qc_dbg_dump_dir);
		qc_dbg_dump_dir = NULL;
		if (asprintf(&qc_dbg_dump_dir, "%s.dump-%u", qc_dbg_file_name,
				qc_dbg_dump_idx) == -1) {
			qc_debug(hdl, "Error: Mem alloc error\n");
			goto out_err;
		}
		if (mkdir(qc_dbg_dump_dir, S_IRWXU) == 0)
			break;
		qc_debug(hdl, "Warning: Could not create dir '%s': %s\n", qc_dbg_dump_dir,
									strerror(errno));
	}
	if (i == 100)
		goto out_err;
	qc_debug(hdl, "Created directory '%s' for all dumps\n", qc_dbg_dump_dir);

	return 0;

out_err:
	qc_debug(hdl, "Error: Could not create directory for dump, better luck maybe next time...\n");
	free(qc_dbg_dump_dir);
	qc_dbg_dump_dir = NULL;

	return -1;
}

static void qc_debug_close_dump_dir(struct qc_handle *hdl) {
	free(qc_dbg_dump_dir);
	qc_dbg_dump_dir = NULL;
}

#define QC_DUMP_INCOMPLETE	"INCOMPLETE_DUMP.txt"
/* Opens a log file for debug messages if env var QC_DEBUG is >0. Note that the file is only
   closed in qc_close_configuration() when qc_dbg_level is <=0, so that it's left up to the user
   to decide whether a single file is used all the time or individual files created for each
   invocation of the library. */
static int qc_debug_init(void) {
	static int init = 0;
	char *path = NULL;
	int rc = 0;

	if (!init) {
		// use a static initializer, as a shared library's init won't work for static libs
		qc_dbg_indent = 0;
		qc_dbg_file = NULL;
		qc_dbg_file_name = NULL;
		qc_dbg_level = 0;
		qc_dbg_dump_dir = NULL;
		qc_dbg_use_dump = NULL;
		qc_dbg_dump_idx = 0;
		qc_dbg_autodump = 0;
		init = 1;
	}
	qc_update_dbg_level();
	if (qc_dbg_level > 0 && !qc_dbg_file) {
		if (qc_debug_file_init()) {
			rc = 1;
			goto out_err;
		}
		qc_debug(NULL, "Log level set to %ld\n", qc_dbg_level);
	}
	if (qc_dbg_use_dump) {
		// usage of dump file requested - any error in here is fatal
		if (access(qc_dbg_use_dump, R_OK | X_OK) == -1) {
			qc_debug(NULL, "Error: Dump usage requested, but path '%s' "
					"not accessible: %s\n",	qc_dbg_use_dump, strerror(errno));
			rc = 2;
			goto out_err;
		}
		// Check for marker indicating incomplete dump
		if (asprintf(&path, "%s/%s", qc_dbg_use_dump, QC_DUMP_INCOMPLETE) == -1) {
			qc_debug(NULL, "Error: Mem alloc failed");
			rc = 3;
			goto out_err;
		}
		if (!access(path, R_OK)) {
			qc_debug(NULL, "Error: Dump at %s is incomplete, cannot use\n", qc_dbg_dump_dir);
			qc_debug(NULL, "       See content of %s for list of missing components\n",
											path);
			rc = 4;
			goto out_err;
		}
		free(path);
		path = NULL;
		qc_debug(NULL, "Running with dump in '%s'\n", qc_dbg_use_dump);
	}

	return 0;


out_err:
	// Nothing we can do about this except to disable debug messages to prevent further damage
	free(qc_dbg_dump_dir);
	qc_dbg_dump_dir = NULL;
	free(qc_dbg_use_dump);
	qc_dbg_use_dump = NULL;
	free(path);

	return rc;
}

void qc_debug_indent_inc(void) {
	qc_dbg_indent += 2;
}

void qc_debug_indent_dec(void) {
	qc_dbg_indent -= 2;
}

void qc_mark_dump_incomplete(struct qc_handle *hdl, char *missing_component) {
	int rc;
	char *cmd;

	if (asprintf(&cmd, "/bin/echo %s >> %s/%s", missing_component, qc_dbg_dump_dir,
								QC_DUMP_INCOMPLETE) == -1) {
		qc_debug(hdl, "Error: Failed to alloc mem to indicate dump as incomplete\n");
		return;
	}
	if ((rc = system(cmd)) != 0)
		qc_debug(hdl, "Error: Failed to exec command to indicate dump as incomplete, "
										"rc=%d\n", rc);
	free(cmd);
}

/* Convert EBCDIC input to ASCII in place, removing trailing whitespace */
int qc_ebcdic_to_ascii(struct qc_handle *hdl, char *inbuf, size_t insz) {
	char *outbuf, *outbuf_start, *inbuf_start = inbuf;
	size_t len, outsz = insz, insz_orig, outsz_orig;
	int rc = 0;

	if (!(outbuf_start = malloc(outsz))) {
		qc_debug(hdl, "Error: Failed to alloc tmp buffer of size %zd for iconv\n", outsz);
		rc = -1;
		goto out;
	}
	outbuf = outbuf_start;
	insz_orig = insz;
	outsz_orig = outsz;
	len = iconv(qc_cd, &inbuf, &insz, &outbuf, &outsz);
	if (len == (size_t)(-1)) {
		qc_debug(hdl, "Error: iconv conversion failed: %s\n", strerror(errno));
		rc = -2;
		goto out;
	}

	// remove trailing whitespace
	for (len = 0; len < outsz_orig - outsz; ++len)
		if (outbuf_start[len] == ' ') {
			outbuf_start[len] = '\0';
			len++;
			break;
		}

	if (len > insz_orig) {
		qc_debug(hdl, "Error: iconv result exceeds target buffer (%zd > %zd)\n",
				len, insz);
		rc = -3;
		goto out;
	}
	memcpy(inbuf_start, outbuf_start, len);

out:
       	free(outbuf_start);

	return rc;
}

static int qc_hdl_register(struct qc_handle *hdl) {
	struct qc_reg_hdl *entry;

	entry = malloc(sizeof(struct qc_reg_hdl));
	if (!entry) {
		qc_debug(hdl, "Error: Failed register hdl\n");
		return -1;
	}
	entry->hdl = hdl;
	if (qc_hdls)
		entry->next = qc_hdls;
	else
		entry->next = NULL;
	qc_hdls = entry;

	return 0;
}

static void qc_hdl_unregister(struct qc_handle *hdl) {
	struct qc_reg_hdl *entry, *prev = NULL;

	for (entry = qc_hdls; entry != NULL; prev = entry, entry = entry->next) {
		if (entry->hdl == hdl) {
			if (prev && entry->next)
				prev->next = entry->next;
			else if (!prev)
				qc_hdls = entry->next;
			else
				prev->next = NULL;
			free(entry);
			break;
		}
	}
	return;
}

static int qc_hdl_verify(struct qc_handle *hdl, const char *func) {
	struct qc_reg_hdl *entry;

	if (!hdl)
		return -1;
	for (entry = qc_hdls; entry != NULL; entry = entry->next) {
		if (entry->hdl == hdl)
			return 0;
	}
	qc_debug(NULL, "Error: %s() called with unknown handle %p\n", func, hdl);

	return -1;
}

// De-alloc hdl, leaving out the actual handle
static void qc_hdl_reinit(struct qc_handle *hdl) {
	struct qc_handle *ptr = hdl, *arg = hdl;

	while (ptr) {
		free(ptr->layer);
		free(ptr->attr_present);
		free(ptr->src);
		hdl = ptr->next;
		if (ptr == arg) {
			memset(ptr, 0, sizeof(struct qc_handle));
			ptr->root = ptr;
		} else
			free(ptr);
		ptr = hdl;
	}
	qc_hdl_unregister(arg);
}

/** Verifies that either a and (b or c), or none are set. I.e. if only one of the attributes is set, then that's an error */
static int qc_verify_capped_capacity(struct qc_handle *hdl, enum qc_attr_id a, enum qc_attr_id b, enum qc_attr_id c) {
	int *val_a, *val_b, *val_c;

	// We assume that non-presence of a value is due to...non-presence, as opposed to an error (since that would have been reported previously)
	val_a = qc_get_attr_value_int(hdl, a);
	val_b = qc_get_attr_value_int(hdl, b);
	val_c = qc_get_attr_value_int(hdl, c);
	if (!val_a && !val_b && !val_c)
		return 0;

	if ((*val_a && !*val_b && !*val_c) || (!*val_a && (*val_b || *val_c))) {
		qc_debug(hdl, "Warning: Consistency check (\"capped capacity\") for '%s && (%s || %s)' failed at layer %d (%s/%s): %d && (%d || %d)\n",
			qc_attr_id_to_char(hdl, a), qc_attr_id_to_char(hdl, b), qc_attr_id_to_char(hdl, c),
			hdl->layer_no, qc_get_attr_value_string(hdl, qc_layer_type), qc_get_attr_value_string(hdl, qc_layer_category),
			*val_a, *val_b, *val_c);
		return 1;
	}

	return 0;
}

#define ATTR_UNDEF	qc_layer_name
/** Verifies that a + (b (+ c)) <= d (b and c are optional, where b being unset (==ATTR_UNDEF) implies c being unset, too)
 *  for the respective int-attributes holds true.
 * @param equals If set, we verify using '=', not '<='
 */
static int qc_verify(struct qc_handle *hdl, enum qc_attr_id a, enum qc_attr_id b, enum qc_attr_id c, enum qc_attr_id d, int equals) {
	int *val_a, *val_b = NULL, *val_c = NULL, *val_d;

	// We assume that non-presence of a value is due to...non-presence, as opposed to an error (since that would have been reported previously)
	if ((val_a = qc_get_attr_value_int(hdl, a)) == NULL ||
	    (val_d = qc_get_attr_value_int(hdl, d)) == NULL)
		return 0;
	if (b != ATTR_UNDEF && (val_b = qc_get_attr_value_int(hdl, b)) == NULL)
		return 0;
	if (c != ATTR_UNDEF && (val_c = qc_get_attr_value_int(hdl, c)) == NULL)
		return 0;

	if (b == ATTR_UNDEF) {
		if ((equals && *val_a != *val_d) || (!equals && *val_a > *val_d)) {
			qc_debug(hdl, "Warning: Consistency check '%s %s %s' failed at layer %d (%s/%s): %d %s %d\n",
				qc_attr_id_to_char(hdl, a), (equals ? "=" : "<="), qc_attr_id_to_char(hdl, d),
				hdl->layer_no, qc_get_attr_value_string(hdl, qc_layer_type), qc_get_attr_value_string(hdl, qc_layer_category),
				*val_a, (equals ? "!=" : ">"), *val_d);
			return 1;
		}
	} else if (c == ATTR_UNDEF) {
		if ((equals && *val_a + *val_b != *val_d) || (!equals && *val_a + *val_b > *val_d)) {
			qc_debug(hdl, "Warning: Consistency check '%s + %s %s %s' failed at layer %d (%s/%s): %d + %d %s %d\n",
				qc_attr_id_to_char(hdl, a), qc_attr_id_to_char(hdl, b), (equals ? "=" : "<="), qc_attr_id_to_char(hdl, d),
				hdl->layer_no, qc_get_attr_value_string(hdl, qc_layer_type), qc_get_attr_value_string(hdl, qc_layer_category),
				*val_a, *val_b, (equals ? "!=" : ">"), *val_d);
			return 2;
		}
	} else {
		if ((equals && *val_a + *val_b + *val_c != *val_d) || (!equals && *val_a + *val_b + *val_c > *val_d)) {
			qc_debug(hdl, "Warning: Consistency check '%s + %s + %s %s %s' failed at layer %d (%s/%s): %d + %d + %d %s %d\n",
				qc_attr_id_to_char(hdl, a), qc_attr_id_to_char(hdl, b),	qc_attr_id_to_char(hdl, c), (equals ? "=" : "<="),
				qc_attr_id_to_char(hdl, d), hdl->layer_no, qc_get_attr_value_string(hdl, qc_layer_type),
				qc_get_attr_value_string(hdl, qc_layer_category), *val_a, *val_b, *val_c, (equals ? "!=" : ">"), *val_d);
			return 1;
		}
	}

	return 0;
}

// Check consistency of data across data sources, as well as consistency of data within each data source.
// Returns 0 in case of success, <0 for errors, and >0 in case the data is inconsistent.
static int qc_consistency_check(struct qc_handle *hdl) {
	int *etype, rc = 0;

	if (!qc_consistency_check_requested)
		return 0;
	qc_debug(hdl, "Run consistency check\n");
	qc_debug_indent_inc();
	for (; hdl; hdl = hdl->next) {
		if ((etype = qc_get_attr_value_int(hdl, qc_layer_type_num)) == NULL) {
			rc = -1;
			goto out;
		}

		switch (*etype) {
		case QC_LAYER_TYPE_CEC:
			if ((rc = qc_verify(hdl, qc_num_core_dedicated, qc_num_core_shared,	ATTR_UNDEF,	      	qc_num_core_total, 0)) ||
			    (rc = qc_verify(hdl, qc_num_core_configured, qc_num_core_standby,	qc_num_core_reserved, 	qc_num_core_total, 0)) ||
			    (rc = qc_verify(hdl, qc_num_cp_total,	 qc_num_ifl_total,	ATTR_UNDEF,     	qc_num_core_total, 0)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,	 qc_num_cp_dedicated,	ATTR_UNDEF,		qc_num_core_dedicated, 1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_shared,	 qc_num_cp_shared,	ATTR_UNDEF,		qc_num_core_shared, 1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,    qc_num_cp_shared,	ATTR_UNDEF,	      	qc_num_cp_total,  1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,   qc_num_ifl_shared,	ATTR_UNDEF,	      	qc_num_ifl_total, 1)) ||
			    (rc = qc_verify(hdl, qc_num_ziip_dedicated,  qc_num_ziip_shared,	ATTR_UNDEF,	      	qc_num_ziip_total, 1)))
				goto out;
			break;
		case QC_LAYER_TYPE_LPAR:
			if ((rc = qc_verify(hdl, qc_num_core_dedicated,  qc_num_core_shared,	ATTR_UNDEF,	     qc_num_core_total, 0)) ||
			    (rc = qc_verify(hdl, qc_num_core_configured, qc_num_core_reserved,	qc_num_core_standby, qc_num_core_total, 0)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,	 qc_num_cp_shared,	ATTR_UNDEF,	     qc_num_cp_total,  1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,   qc_num_ifl_shared,	ATTR_UNDEF,	     qc_num_ifl_total, 1)) ||
			    (rc = qc_verify(hdl, qc_num_ziip_dedicated,  qc_num_ziip_shared,	ATTR_UNDEF,	     qc_num_ziip_total, 1)))
				goto out;
			break;
		case QC_LAYER_TYPE_ZVM_HYPERVISOR:
			if ((rc = qc_verify(hdl, qc_num_core_dedicated, qc_num_core_shared,	ATTR_UNDEF,		qc_num_core_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_total,       qc_num_ifl_total,	ATTR_UNDEF,		qc_num_core_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,   qc_num_ifl_dedicated,	ATTR_UNDEF,		qc_num_core_dedicated,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_shared,      qc_num_ifl_shared,	ATTR_UNDEF,		qc_num_core_shared,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,   qc_num_cp_shared,	ATTR_UNDEF, qc_num_cp_total,	  1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,  qc_num_ifl_shared,	ATTR_UNDEF, qc_num_ifl_total,	  1)) ||
			    (rc = qc_verify(hdl, qc_num_ziip_dedicated,	qc_num_ziip_shared,	ATTR_UNDEF, qc_num_ziip_total,	  1)))
				goto out;
			break;
		case QC_LAYER_TYPE_ZVM_CPU_POOL:
		case QC_LAYER_TYPE_ZOS_TENANT_RESOURCE_GROUP:
			if ((rc = qc_verify_capped_capacity(hdl, qc_cp_capped_capacity, qc_cp_capacity_cap, qc_cp_limithard_cap)) ||
			    (rc = qc_verify_capped_capacity(hdl, qc_ifl_capped_capacity, qc_ifl_capacity_cap, qc_ifl_limithard_cap)) ||
			    (rc = qc_verify_capped_capacity(hdl, qc_ziip_capped_capacity, qc_ziip_capacity_cap, qc_ziip_limithard_cap)))
				goto out;
			break;
		case QC_LAYER_TYPE_ZVM_GUEST:
			if ((rc = qc_verify(hdl, qc_num_cpu_dedicated,  qc_num_cpu_shared,	ATTR_UNDEF,		qc_num_cpu_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cpu_configured, qc_num_cpu_reserved,	qc_num_cpu_standby,	qc_num_cpu_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_total,       qc_num_ifl_total,	ATTR_UNDEF,		qc_num_cpu_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,   qc_num_ifl_dedicated,	ATTR_UNDEF,		qc_num_cpu_dedicated,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_shared,      qc_num_ifl_shared,	ATTR_UNDEF,		qc_num_cpu_shared,	1)) ||
			    (rc = qc_verify(hdl, qc_num_cp_dedicated,   qc_num_cp_shared,	ATTR_UNDEF,		qc_num_cp_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,  qc_num_ifl_shared,	ATTR_UNDEF,		qc_num_ifl_total,	1)) ||
			    (rc = qc_verify(hdl, qc_num_ziip_dedicated,	qc_num_ziip_shared,	ATTR_UNDEF,		qc_num_ziip_total,	1)))
				goto out;
			break;
		case QC_LAYER_TYPE_KVM_HYPERVISOR:
			if ((rc = qc_verify(hdl, qc_num_core_shared, 	qc_num_core_dedicated,	ATTR_UNDEF,	    qc_num_core_total, 1)) ||
			(rc = qc_verify(hdl, qc_num_cp_dedicated,	qc_num_cp_shared,	ATTR_UNDEF,	    qc_num_cp_total, 1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,	qc_num_ifl_shared,	ATTR_UNDEF,	    qc_num_ifl_total, 1)) ||
			    (rc = qc_verify(hdl, qc_num_ziip_dedicated,	qc_num_ziip_shared,	ATTR_UNDEF,	    qc_num_ziip_total, 1)))
				goto out;
			break;
		case QC_LAYER_TYPE_KVM_GUEST:
			if ((rc = qc_verify(hdl, qc_num_cpu_configured, qc_num_cpu_reserved,	qc_num_cpu_standby, qc_num_cpu_total,	   1)) ||
			    (rc = qc_verify(hdl, qc_num_cpu_shared, 	qc_num_cpu_dedicated,	ATTR_UNDEF,	    qc_num_cpu_total,	   1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,	qc_num_ifl_shared,	ATTR_UNDEF,	    qc_num_ifl_total,	   1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_dedicated,	ATTR_UNDEF,		ATTR_UNDEF,	    0,			   1)) ||
			    (rc = qc_verify(hdl, qc_num_ifl_shared,	ATTR_UNDEF,		ATTR_UNDEF,	    qc_num_cpu_configured, 1)))
				goto out;
			break;
		default: break;
		}
	}

out:
	if (rc)
		qc_debug(hdl, "Warning: Consistency check failed\n");
	qc_debug_indent_dec();

	return rc;
}

static int qc_copy_attr_value(struct qc_handle *tgt, struct qc_handle *src, enum qc_attr_id id) {
	int *i = qc_get_attr_value_int(src, id);

	return i ? qc_set_attr_int(tgt, id, *i, ATTR_SRC_POSTPROC) : 0;
}

// src layer can have either qc_num_core_* (in case of LPAR) or qc_num_cpu_* (in case of e.g. KVM guest) attributes!
static int qc_copy_attr_value_rename(struct qc_handle *tgt, enum qc_attr_id tgtid, struct qc_handle *src, enum qc_attr_id altsrcid) {
	int *i = qc_get_attr_value_int(src, tgtid);

	if (!i)
		i = qc_get_attr_value_int(src, altsrcid);

	return i ? qc_set_attr_int(tgt, tgtid, *i, ATTR_SRC_POSTPROC) : 0;
}

struct qc_mtype {
	int	type;
	char   *zname;	// IBM Z
	char   *lname;  // LinuxONE
};

static struct qc_mtype mtypes[] = {
	//     IBM Z				LinuxONE
	{4381, "IBM 4381",			NULL},
	{3090, "IBM 3090",			NULL},
	{9221, "IBM S/390 9221",		NULL},
	{9021, "IBM ES/9000 9021",		NULL},
	{2003, "IBM S/390 Multiprise 2000",	NULL},
	{3000, "IBM S/390 StarterPak 3000",	NULL},
	{9672, "IBM S/390 9672",		NULL},
	{2066, "IBM zSeries 800",		NULL},
	{2064, "IBM zSeries 900",		NULL},
	{2086, "IBM zSeries 890",		NULL},
	{2084, "IBM zSeries 990",		NULL},
	{2096, "IBM System z9 BC",		NULL},
	{2094, "IBM System z9 EC",		NULL},
	{2098, "IBM System z10 BC",		NULL},
	{2097, "IBM System z10 EC",		NULL},
	{2818, "IBM zEnterprise 114",		NULL},
	{2817, "IBM zEnterprise 196",		NULL},
	{2827, "IBM zEnterprise EC12",		NULL},
	{2828, "IBM zEnterprise BC12",		NULL},
	{2965, "IBM z13s",			"IBM LinuxONE Rockhopper"},
	{2964, "IBM z13",			"IBM LinuxONE Emperor"},
	{3907, "IBM z14 ZR1",			"IBM LinuxONE Rockhopper II"},
	{3906, "IBM z14",			"IBM LinuxONE Emperor II"},
	{8561, "IBM z15",			"IBM LinuxONE III"},
	{8562, "IBM z15 Model T02",		"IBM LinuxONE III Model LT2"},
	{0,    NULL,				NULL}
};

static int qc_post_process_ziip_thrds(struct qc_handle *hdl) {
	int *thrds, *ziips;

	if ((ziips = qc_get_attr_value_int(hdl, qc_num_ziip_total)) && *ziips) {
		if ((thrds = qc_get_attr_value_int(hdl, qc_num_ifl_threads)) &&
		    qc_set_attr_int(hdl, qc_num_ziip_threads, *thrds, ATTR_SRC_POSTPROC))
			return -1;
	}

	return 0;
}

static int qc_post_process_CEC(struct qc_handle *hdl) {
	int cpuid, rc = -1, family = QC_TYPE_FAMILY_IBMZ;
	struct qc_mtype *type;
	char *str;

	qc_debug(hdl, "Fill CEC layer\n");
	qc_debug_indent_inc();
	if ((str = qc_get_attr_value_string(hdl, qc_type)) == NULL)
		goto out;
	cpuid = atoi(str);
	for (type = mtypes; type->type; ++type) {
		if (cpuid == type->type) {
			if (type->lname &&
			    (str = qc_get_attr_value_string(hdl, qc_model)) != NULL &&
			    *str == 'L') {
				str = type->lname;
				family = QC_TYPE_FAMILY_LINUXONE;
			} else
				str = type->zname;
			if (qc_set_attr_string(hdl, qc_type_name, str, ATTR_SRC_POSTPROC) ||
			    qc_set_attr_int(hdl, qc_type_family, family, ATTR_SRC_POSTPROC))
				goto out;
			break;
		}
	}
	if (qc_post_process_ziip_thrds(hdl))
		goto out;
	rc = 0;

out:
	qc_debug_indent_dec();

	return rc;
}

static int qc_post_process_LPAR(struct qc_handle *hdl) {
	return qc_post_process_ziip_thrds(hdl);
}

static int qc_post_process_KVM_host(struct qc_handle *hdl) {
	struct qc_handle *parent = qc_get_prev_handle(hdl);
	int *num_conf, rc, *cps, *ifls;

	qc_debug(hdl, "Fill KVM host layer\n");
	qc_debug_indent_inc();
	// We can copy most information from the parent with few exceptions
	if (*(int *)(parent->layer) == QC_LAYER_TYPE_KVM_GUEST)
		// KVM guests ain't got no CPs
		rc = qc_set_attr_int(hdl, qc_num_cp_total, 0, ATTR_SRC_POSTPROC) ||
		     qc_set_attr_int(hdl, qc_num_cp_dedicated, 0, ATTR_SRC_POSTPROC) ||
		     qc_set_attr_int(hdl, qc_num_cp_shared, 0, ATTR_SRC_POSTPROC);
	else
		rc = qc_copy_attr_value(hdl, parent, qc_num_cp_total) ||
		     qc_copy_attr_value(hdl, parent, qc_num_cp_dedicated) ||
		     qc_copy_attr_value(hdl, parent, qc_num_cp_shared);
	// only parent layer's configured CPUs are available to the host
	num_conf = qc_get_attr_value_int(parent, qc_num_core_configured);
	if (!num_conf)
		num_conf = qc_get_attr_value_int(parent, qc_num_cpu_configured);
	if (!num_conf)
		rc = 1;
	if (!rc) {
		rc |= qc_set_attr_int(hdl, qc_num_core_total, *num_conf, ATTR_SRC_POSTPROC) ||
		      qc_copy_attr_value_rename(hdl, qc_num_core_dedicated, parent, qc_num_cpu_dedicated) ||
		      qc_copy_attr_value_rename(hdl, qc_num_core_shared, parent, qc_num_cpu_shared);
		cps = qc_get_attr_value_int(parent, qc_num_cp_total);
		ifls = qc_get_attr_value_int(parent, qc_num_cp_total);
		if (cps && ifls && *cps && *ifls) {
			// mixed-mode LPARs use CPs only!
			rc |= qc_set_attr_int(hdl, qc_num_ifl_total, 0, ATTR_SRC_POSTPROC) ||
			      qc_set_attr_int(hdl, qc_num_ifl_dedicated, 0, ATTR_SRC_POSTPROC) ||
			      qc_set_attr_int(hdl, qc_num_ifl_shared, 0, ATTR_SRC_POSTPROC);
		} else {
			rc |= qc_copy_attr_value(hdl, parent, qc_num_ifl_total) ||
			      qc_copy_attr_value(hdl, parent, qc_num_ifl_dedicated) ||
			      qc_copy_attr_value(hdl, parent, qc_num_ifl_shared);
		}
	}

	qc_debug_indent_dec();

	return rc;
}

static int qc_post_process_KVM_guest(struct qc_handle *hdl) {
	struct qc_handle *parent = qc_get_prev_handle(hdl);
	int rc = 0, *num_conf, *num_CPs, *num_IFLs;

	qc_debug(hdl, "Fill KVM guest layer\n");
	qc_debug_indent_inc();
	// KVM guests support IFLs only - all configured CPUs (comes from sysinfo!) are treated as IFLs
	num_conf = qc_get_attr_value_int(hdl, qc_num_cpu_configured);
	if (qc_set_attr_int(hdl, qc_num_ifl_total, *num_conf, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_int(hdl, qc_num_ifl_dedicated, 0, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_int(hdl, qc_num_ifl_shared, *num_conf, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_int(hdl, qc_num_cpu_dedicated, 0, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_int(hdl, qc_num_cpu_shared, *num_conf, ATTR_SRC_SYSINFO)) {
		rc = -1;
		goto out;
	}

	num_CPs = qc_get_attr_value_int(parent, qc_num_cp_total);
	num_IFLs = qc_get_attr_value_int(parent, qc_num_ifl_total);
	if (num_CPs && num_IFLs) {
		if (*num_CPs > 0 && *num_IFLs > 0) {
			qc_debug(hdl, "Warning: KVM guest running on a mixed-mode host!\n");
			goto out;	// not an error
		}
		if (qc_set_attr_int(hdl, qc_ifl_dispatch_type, *num_IFLs > 0 ? 3 : 0, ATTR_SRC_POSTPROC)) {
			rc = -3;
			goto out;
		}
	}

out:
	qc_debug_indent_dec();

	return rc;
}

static int qc_post_processing(struct qc_handle *hdl) {
	qc_debug(hdl, "Post processing: Fill KVM layers\n");
	qc_debug_indent_inc();
	for (; hdl; hdl = hdl->next) {
		switch(*(int *)(hdl->layer)) {
		case QC_LAYER_TYPE_CEC:
			if (qc_post_process_CEC(hdl))
				goto fail;
			break;
		case QC_LAYER_TYPE_LPAR:
			if (qc_post_process_LPAR(hdl))
				goto fail;
			break;
		case QC_LAYER_TYPE_KVM_HYPERVISOR:
			if (qc_post_process_KVM_host(hdl))
				goto fail;
			break;
		case QC_LAYER_TYPE_KVM_GUEST:
			if (qc_post_process_KVM_guest(hdl))
				goto fail;
			break;
		default:
			break;
		}
	}
	qc_debug_indent_dec();

	return 0;
fail:
	qc_debug_indent_dec();

	return -1;
}

static void *_qc_open(struct qc_handle *hdl, int *rc) {
	// sysinfo needs to be handled first, or our LGM check later on will have loopholes
	// sysfs needs to be handled last, as part of the attributes apply to top-most layer only
	struct qc_data_src *src, *sources[] = {&sysinfo, &hypfs, &sthyi, &sysfs, NULL};
	struct qc_handle *lparhdl;
	int i;

	qc_debug(hdl, "_qc_open()\n");
	qc_debug_indent_inc();
	*rc = 0;
	if (qc_new_handle(NULL, &hdl, 0, QC_LAYER_TYPE_CEC) ||
	    qc_new_handle(hdl, &lparhdl, 1, QC_LAYER_TYPE_LPAR)) {
		*rc = -1;
		goto out;
	}
	hdl->next = lparhdl;
	lparhdl->root = hdl->root;

	// open all data sources
	for (i = 0; (src = sources[i]) != NULL; i++)
		if (src->open(hdl, &src->priv))
			*rc = -2;	// don't exit on error immediately, so we collect all data for a dump later on
	if (*rc)
		goto out;

	// verify that we weren't migrated
	if ((*rc = sysinfo.lgm_check(hdl, sysinfo.priv)) != 0)
		goto out;

	// process data sources
	for (i = 0; (src = sources[i]) != NULL; i++) {
		// Return values >0 will be left as is and passed back to caller
		if ((*rc = src->process(hdl, src->priv)) < 0) {
			*rc = -3;	// match errors to a value that we can identify
			goto out;
		}
		if (*rc)
			goto out;
	}

	if (qc_post_processing(hdl)) {
		*rc = -4;
		goto out;
	}

	if (qc_dbg_level > 0) {
		qc_debug(hdl, "Final layers overview:\n");
		qc_debug_indent_inc();
		for (lparhdl = hdl; lparhdl; lparhdl = lparhdl->next)
			qc_debug(hdl, "Layer %2i: %s %s\n", lparhdl->layer_no, qc_get_attr_value_string(lparhdl, qc_layer_type),
				 qc_get_attr_value_string(lparhdl, qc_layer_category));
		qc_debug_indent_dec();
	}

out:
	// Possibly dump all data sources
	if (qc_dbg_level > 1 || (qc_dbg_autodump && *rc < 0)) {
		qc_debug(hdl, "Create dump\n");
		qc_debug_indent_inc();
		if (qc_debug_open_dump_dir(hdl) == 0) {	// get a new dump directory
			for (i = 0; (src = sources[i]) != NULL; i++)
				src->dump(hdl, src->priv);
			qc_debug_close_dump_dir(hdl);
		} else
			qc_debug(hdl, "Failed, could not open directory\n");
		qc_debug_indent_dec();
	}

	// Close all data sources
	for (i = 0; (src = sources[i]) != NULL; i++)
		src->close(hdl, src->priv);
	if (hdl)
		// nothing else we can do if registration fails
		qc_hdl_register(hdl);
	qc_debug(hdl, "Return rc=%d\n", *rc);
	qc_debug_indent_dec();

	return hdl;
}

void *qc_open(int *rc) {
	struct qc_handle *hdl = NULL;
	char *s, *end;
	int i;

	*rc = 0;
	if (qc_debug_init()) {
		*rc = -1;
		goto out;
	}
	qc_debug(hdl, "qc_open()\n");
	qc_debug_indent_inc();

	if (qc_cd == (iconv_t)-1) {
		qc_debug(hdl, "Initialize iconv\n");
		qc_cd = iconv_open("ISO8859-1", "IBM-1047");
		if (qc_cd == (iconv_t)-1) {
			qc_debug(hdl, "Error: iconv setup failed: %s\n", strerror(errno));
			*rc = -2;
			goto out;
		}
	}

	if ((s = getenv("QC_CHECK_CONSISTENCY")) != NULL) {
		qc_consistency_check_requested = strtol(s, &end, 10);
		if (end == s || qc_consistency_check_requested < 0)
			qc_consistency_check_requested = 0;
	}

	/* Since we retrieve data from multiple sources, CPU hotplugging provides a chance for
	 * inconsistent data. If we detect that, we retry up to a total of 3 times before
	 * giving up. */
	for (i = 0; i < 3; ++i) {
		if (i > 0) {
			qc_debug(hdl, "Warning: Gathering data failed, retry %d\n", i);
			qc_hdl_reinit(hdl);
		}
		hdl = _qc_open(hdl, rc);
		if (*rc > 0)
			continue;
		if (*rc < 0 || ((*rc = qc_consistency_check(hdl)) <= 0))
			break;
	}
	if (*rc > 0)
		qc_debug(hdl, "Error: Unable to retrieve consistent data, giving up\n");

out:
	qc_debug(hdl, "Return %p, rc=%d\n", *rc ? NULL : hdl, *rc);
	qc_debug_indent_dec();
	if (*rc) {
		qc_close(hdl);
		hdl = NULL;
	}

	return hdl;
}

void qc_close(void *hdl) {
	if (qc_hdl_verify(hdl, "qc_close"))
		return;
	qc_debug(hdl, "qc_close()\n");
	qc_debug_indent_inc();

	qc_debug_deinit(hdl);
	qc_hdl_reinit(hdl);
	free(hdl);

	qc_debug_indent_dec();
}

int qc_get_num_layers(void *cfg, int *rc) {
	struct qc_handle *hdl = cfg;

	if (qc_hdl_verify(hdl, "qc_get_num_layers")) {
		*rc = -EFAULT;
		return *rc;
	}
	qc_debug(hdl, "qc_get_num_layers()\n");
	qc_debug_indent_inc();
	while (hdl->next)
		hdl = hdl->next;
	qc_debug(hdl, "Return %d layers\n", hdl->layer_no + 1);
	*rc = 0;
	qc_debug_indent_dec();

	return hdl->layer_no + 1;
}

static struct qc_handle *qc_get_layer_handle(void *config, int layer) {
	struct qc_handle *hdl = config;

	do {
		if (hdl->layer_no == layer)
			return hdl;
		hdl = hdl->next;
	} while (hdl);

	return NULL;
}

static int qc_is_attr_id_valid(enum qc_attr_id id) {
	return id <= qc_secure;
}

int qc_get_attribute_string(void *cfg, enum qc_attr_id id, int layer, const char **value) {
	struct qc_handle *hdl;
	int rc;

	*value = NULL;
	if (qc_hdl_verify(cfg, "qc_get_attribute_string"))
		return -4;
	hdl = qc_get_layer_handle(cfg, layer);
	qc_debug(cfg, "qc_get_attribute_string(attr=%d, layer=%d)\n", id, layer);
	qc_debug_indent_inc();
	if (!hdl) {
		rc = -1;
		goto out;
	}
	if (!qc_is_attr_id_valid(id)) {
		rc = -2;
		goto out;
	}
	if ((*value = qc_get_attr_value_string(hdl, id))) {
		qc_debug(cfg, "Attr '%s' from '%c' res=%s\n", qc_attr_id_to_char(cfg, id), qc_get_attr_value_src_string(hdl, id), *value);
		rc = 1;
		goto out;
	}
	if (qc_is_attr_set_string(hdl, id) <= 0) {
		qc_debug(cfg, "Attr '%s' not defined\n", qc_attr_id_to_char(cfg, id));
		rc = 0;
		goto out;
	}
	rc = -3;

out:
	qc_debug(cfg, "Return value='%s', rc=%d\n", *value, rc);
	qc_debug_indent_dec();
	return rc;
}

int qc_get_attribute_int(void *cfg, enum qc_attr_id id, int layer, int *value) {
	struct qc_handle *hdl;
	void *ptr = NULL;
	int rc;

	*value = -EINVAL;
	if (qc_hdl_verify(cfg, "qc_get_attribute_int"))
		return -4;
	hdl = qc_get_layer_handle(cfg, layer);
	qc_debug(cfg, "qc_get_attribute_int(attr=%d, layer=%d)\n", id, layer);
	qc_debug_indent_inc();
	if (!hdl) {
		rc = -1;
		goto out;
	}
	if (!qc_is_attr_id_valid(id)) {
		rc = -2;
		goto out;
	}
	if ((ptr = qc_get_attr_value_int(hdl, id))) {
		qc_debug(cfg, "Attr '%s' from '%c' res=%d\n", qc_attr_id_to_char(cfg, id), qc_get_attr_value_src_int(hdl, id), *(int *)ptr);
		rc = 1;
		goto out;
	}
	// Attribute value not set - let's figure out why
	if (qc_is_attr_set_int(hdl, id) <= 0) {
		qc_debug(cfg, "Attr '%s' not defined\n", qc_attr_id_to_char(cfg, id));
		rc = 0;
		goto out;
	}
	rc = -3;

out:
	if (ptr)
		*value = *(int *)ptr;
	qc_debug(cfg, "Return value=%d, rc=%d\n", *value, rc);
	qc_debug_indent_dec();

	return rc;
}


int qc_get_attribute_float(void *cfg, enum qc_attr_id id, int layer, float *value) {
	struct qc_handle *hdl;
	void *ptr = NULL;
	int rc;

	*value = -EINVAL;
	if (qc_hdl_verify(cfg, "qc_get_attribute_float"))
		return -4;
	hdl = qc_get_layer_handle(cfg, layer);
	qc_debug(cfg, "qc_get_attribute_float(attr=%d, layer=%d)\n", id, layer);
	qc_debug_indent_inc();
	if (!hdl) {
		rc = -1;
		goto out;
	}
	if (!qc_is_attr_id_valid(id)) {
		rc = -2;
		goto out;
	}
	if ((ptr = qc_get_attr_value_float(hdl, id))) {
		qc_debug(cfg, "Attr '%s' from '%c' res=%f\n", qc_attr_id_to_char(cfg, id), qc_get_attr_value_src_float(hdl, id), *(float *)ptr);
		rc = 1;
		goto out;
	}
	// Attribute value not set - let's figure out why
	if (qc_is_attr_set_float(hdl, id) <= 0) {
		qc_debug(cfg, "Attr '%s' not defined\n", qc_attr_id_to_char(cfg, id));
		rc = 0;
		goto out;
	}
	rc = -3;

out:
	if (ptr)
		*value = *(float *)ptr;
	qc_debug(cfg, "Return value=%f, rc=%d\n", *value, rc);
	qc_debug_indent_dec();

	return rc;
}

static void qc_start_object(int *jindent, int layer) {
	printf("%*s\"Layer %d\": {\n", *jindent, "", layer);
	*jindent += 2;
}
static void qc_end_object(int *jindent, int final) {
	*jindent -= 2;
	printf("%*s}%s\n", *jindent, "", (final ? "" : ","));
}

void qc_export_json(void *cfg) {
	struct qc_handle *hdl = (struct qc_handle *)cfg;
	int jindent = 0;	// indent for json output
	int i;

	if (!hdl)
		return;

	printf("{\n");
	jindent += 2;
	for (hdl = hdl->root, i = 0; hdl != NULL; hdl = hdl->next, i++) {
		qc_start_object(&jindent, i);
		qc_print_attrs_json(hdl, jindent);
		qc_end_object(&jindent, hdl->next == NULL);
	}

	printf("}\n");

	return;
}
