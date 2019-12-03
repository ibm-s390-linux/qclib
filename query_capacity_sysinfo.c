/* Copyright IBM Corp. 2013, 2019 */

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string.h>

#include "query_capacity_data.h"


static const char *qc_sysinfo_delim = "\n";


static void qc_sysinfo_dump(struct qc_handle *hdl, char *sysinfo) {
	char *path;
	FILE *f;

	qc_debug(hdl, "Dump sysinfo\n");
	qc_debug_indent_inc();
	if (!sysinfo) {
		// /proc/sysinfo is guaranteed to exist - if not, something went wrong
		qc_debug(hdl, "Error: Failed to dump sysinfo, as sysinfo == NULL\n");
		qc_mark_dump_incomplete(hdl, "sysinfo");
		qc_debug_indent_dec();
		return;
	}
	if (asprintf(&path, "%s/sysinfo", qc_dbg_dump_dir) == -1) {
		qc_debug(hdl, "Error: Mem alloc failure, cannot dump sysinfo\n");
		qc_mark_dump_incomplete(hdl, "sysinfo");
		qc_debug_indent_dec();
		return;
	}
	f = fopen(path, "w");
	if (f) {
		if (fprintf(f, "%s", sysinfo) >= 0) {
			qc_debug(hdl, "sysinfo dumped to '%s'\n", path);
		} else {
			qc_debug(hdl, "Error: Failed to write dump to '%s'\n", path);
			qc_mark_dump_incomplete(hdl, "sysinfo");
		}
		fclose(f);
	} else {
		qc_debug(hdl, "Error: Failed to open %s to write sysinfo dump\n", path);
		qc_mark_dump_incomplete(hdl, "sysinfo");
	}
	free(path);
	qc_debug_indent_dec();

	return;
}

static int qc_sysinfo_open(struct qc_handle *hdl, char **sysinfo) {
	char *fname = NULL;
	ssize_t lrc = 1, sysinfo_sz;
	struct stat buf;
	int fd;

	qc_debug(hdl, "Retrieve sysinfo\n");
	qc_debug_indent_inc();
	*sysinfo = NULL;
	if (qc_dbg_use_dump) {
		qc_debug(hdl, "Read sysinfo from dump\n");
		if (asprintf(&fname, "%s/sysinfo", qc_dbg_use_dump) == -1) {
			qc_debug(hdl, "Error: Mem alloc failed, cannot open dump\n");
			goto out_early;
		}
		if (stat(fname, &buf)) {
			qc_debug(hdl, "Error: Failed to stat file '%s'\n", fname);
			goto out_early;
		}
		sysinfo_sz = buf.st_size + 1;
	} else {
		qc_debug(hdl, "Read sysinfo from /proc/sysinfo\n");
		sysinfo_sz = 4096;
	}

	for (lrc = sysinfo_sz; lrc >= sysinfo_sz; sysinfo_sz *= 2, lrc *= 2) {
		fd = open(fname ? fname : "/proc/sysinfo", O_RDONLY);
		if (!fd) {
			qc_debug(hdl, "Error: Failed to open file '%s': %s\n",
				 fname ? fname : "/proc/sysinfo", strerror(errno));
			goto out;
		}

		free(*sysinfo);
		qc_debug(hdl, "Read sysinfo using buffer size %zu\n", sysinfo_sz);
		*sysinfo = malloc(sysinfo_sz);
		if (!*sysinfo) {
			qc_debug(hdl, "Error: Failed to alloc buffer for sysinfo file\n");
			goto out;
		}
		lrc = read(fd, *sysinfo, sysinfo_sz);
		if (lrc == -1) {
			qc_debug(hdl, "Error: Failed to read %s file: %s\n",
				 fname ? fname : "/proc/sysinfo", strerror(errno));
			free(*sysinfo);
			*sysinfo = NULL;
			goto out;
		}
		(*sysinfo)[lrc] = '\0';
		close(fd);
	}
	goto out_early;

out:
	close(fd);

out_early:
	free(fname);
	qc_debug(hdl, "Done reading sysinfo, sysinfo=%p\n", *sysinfo);
	qc_debug_indent_dec();

	return *sysinfo == NULL;
}

static int qc_sysinfo_lgm_check(struct qc_handle *hdl, const char *sysinfo) {
	char *lsysinfo = NULL;
	int rc = 0;

	// Live Guest Migration check: If we were migrated, /proc/sysinfo will have changed
	qc_debug(hdl, "Run LGM check\n");
	qc_debug_indent_inc();
	if (qc_sysinfo_open(hdl, &lsysinfo)) {
		qc_debug(hdl, "Error: Failed to open /proc/sysinfo\n");
		rc = -1;
		goto out;
	}
	if (strcmp(lsysinfo, sysinfo)) {
		qc_debug(hdl, "/proc/sysinfo content changed, LGM took place!\n");
		rc = 1;
		goto out;
	}
	qc_debug(hdl, "/proc/sysinfo still consistent, no LGM detected\n");
	rc = 0;

out:
	qc_debug_indent_dec();
	free(lsysinfo);

	return rc;
}

static void qc_sysinfo_close(struct qc_handle *hdl, char *sysinfo) {
	free(sysinfo);
}

/* Whenever we're using strtok_r() to parse sysinfo, we're messing up the string, since
   strtok_r() will insert '\0's, so this function will create a fresh copy to work on. */
static char *qc_copy_sysinfo(struct qc_handle *hdl, char *sysinfo) {
	char *rc;

	if ((rc = strdup(sysinfo)) == NULL)
		qc_debug(hdl, "Error: Failed to create copy of qc_sysconfig\n");

	return rc;
}

#define QC_SYSINFO_PARSE_LINE_STR_NOCONT(hdl, str, strlen, id) \
	if (sscanf(*line, str, str_buf) > 0 && qc_set_attr_string(hdl, id, str_buf, ATTR_SRC_SYSINFO)) \
		goto out_err;
#define QC_SYSINFO_PARSE_LINE_STR(hdl, str, strlen, id) \
	if (sscanf(*line, str, str_buf) > 0) { \
		if (qc_set_attr_string(hdl, id, str_buf, ATTR_SRC_SYSINFO)) \
			goto out_err; \
		continue; \
	}
#define QC_SYSINFO_PARSE_LINE_INT_inc(hdl, str, id, inc) \
	if (sscanf(*line, str, &int_buf) > 0) { \
		if (qc_set_attr_int(hdl, id, int_buf + inc, ATTR_SRC_SYSINFO)) \
			goto out_err; \
		continue; \
	}
#define QC_SYSINFO_PARSE_LINE_INT(hdl, str, id) QC_SYSINFO_PARSE_LINE_INT_inc(hdl, str, id, 0)
#define QC_SYSINFO_PARSE_LINE_FLOAT(hdl, str, id) \
	if (sscanf(*line, str, &float_buf)) { \
		if (qc_set_attr_float(hdl, id, float_buf, ATTR_SRC_SYSINFO)) \
			goto out_err; \
		continue; \
	}
static int qc_fill_in_sysinfo_values_vm(struct qc_handle *hdl, char **sptr, char **line) {
	char str_buf[STR_BUF_SIZE], layer_name[STR_BUF_SIZE];
	struct qc_handle *guesthdl = NULL, *hosthdl = NULL;
	int i, j, rc = -1, int_buf, guesttype, hosttype;
	char vmxx[] = "VMxx ";
	char c, *sysi = NULL;

	qc_debug(hdl, "Retrieve /proc/sysinfo information for VM\n");
	qc_debug_indent_inc();
	for (i = 0; *line && i < 100; i++) {	// /proc/sysinfo cannot go beyond 99 layers of VM
		for (j = 2, c = '0' + i/10; j <= 3; ++j, c = '0' + i%10)
			vmxx[j] = c;
		// Parse file till we find control program ID and name (which precedes)
		// Note: strtok_r will skip empty lines - hence we can't start out reading the next line
		//       start of the loop, or we'd skip a line when looping in the big loop
		layer_name[0] = '\0';
		do {
			if (strncmp(*line, vmxx, 5) != 0)
				continue;	// fast-forward till eof
			*line += 5;
			sysi = NULL;
			// Note: %x[^\n] can create trailing blanks, but unavoidable, since some names *can* contain blanks
			if (layer_name[0] == '\0' && (rc = sscanf(*line, "Name: %8[^\n]", layer_name))) {
				if (rc < 0)
					break;
			}
			if ((rc = sscanf(*line, "Control Program: %16[^\001]s", str_buf)) > 0)
				break;
		} while (!rc && (*line = strtok_r(sysi, qc_sysinfo_delim, sptr)) != NULL);
		if (!*line) {	// end of file reached, but no VM layers found
			rc = 0;
			goto out;
		}
		if (!rc) {
			qc_debug(hdl, "Error: Failed to retrieve CP ID from /proc/sysinfo\n");
			goto out;
		}
		rc = -2;
		if (!strncmp(str_buf, "z/VM", strlen("z/VM"))) {
			hosttype = QC_LAYER_TYPE_ZVM_HYPERVISOR;
			guesttype = QC_LAYER_TYPE_ZVM_GUEST;
			qc_debug(hdl, "Layer %2d: z/VM-host\n", hdl->layer_no + 1);
			qc_debug(hdl, "Layer %2d: z/VM-guest\n", hdl->layer_no + 2);
		} else if (!strncmp(str_buf, "z/OS zCX", strlen("z/OS zCX")) ||
			   !strncmp(str_buf, "KVM zCX", strlen("KVM zCX"))) {
			hosttype = QC_LAYER_TYPE_ZOS_HYPERVISOR;
			guesttype = QC_LAYER_TYPE_ZOS_ZCX_SERVER;
			qc_debug(hdl, "Layer %2d: z/OS-host\n", hdl->layer_no + 1);
			qc_debug(hdl, "Layer %2d: z/OS-zCX-Server\n", hdl->layer_no + 2);
		} else if (!strncmp(str_buf, "KVM", strlen("KVM"))) {
			hosttype = QC_LAYER_TYPE_KVM_HYPERVISOR;
			guesttype = QC_LAYER_TYPE_KVM_GUEST;
			qc_debug(hdl, "Layer %2d: KVM-host\n", hdl->layer_no + 1);
			qc_debug(hdl, "Layer %2d: KVM-guest\n", hdl->layer_no + 2);
		} else {
			qc_debug(hdl, "Error: Unsupported virtualization environment "
					"encountered: '%s'\n", str_buf);
			goto out;
		}
		if (!hdl->next)
			rc = qc_append_handle(hdl, &hosthdl, hosttype);
		else
			rc = qc_insert_handle(hdl->next, &hosthdl, hosttype);
		if (rc)
			goto out;
		if (qc_append_handle(hosthdl, &guesthdl, guesttype))
			goto out_err;
		if (qc_set_attr_string(hosthdl, qc_control_program_id, str_buf, ATTR_SRC_SYSINFO) ||
		    qc_set_attr_string(guesthdl, qc_layer_name, layer_name, ATTR_SRC_SYSINFO))
			goto out_err;
		for (sysi = NULL, *line = strtok_r(sysi, qc_sysinfo_delim, sptr); *line;
		     *line = strtok_r(sysi, qc_sysinfo_delim, sptr)) {
			if (strncmp(*line, vmxx, 4) != 0)
				break;
			*line += 5;
			sysi = NULL;
			QC_SYSINFO_PARSE_LINE_INT(hosthdl, "Adjustment: %i", qc_adjustment);
			if (strncmp(*line, "CPUs ", 5) == 0) {
				*line += 5;
				QC_SYSINFO_PARSE_LINE_INT(guesthdl, "Total: %i", qc_num_cpu_total);
				QC_SYSINFO_PARSE_LINE_INT(guesthdl, "Configured: %i", qc_num_cpu_configured);
				QC_SYSINFO_PARSE_LINE_INT(guesthdl, "Standby: %i", qc_num_cpu_standby);
				QC_SYSINFO_PARSE_LINE_INT(guesthdl, "Reserved: %i", qc_num_cpu_reserved);
			}
			if (guesttype == QC_LAYER_TYPE_KVM_GUEST) {
				QC_SYSINFO_PARSE_LINE_STR(guesthdl, "Extended Name: %256[^\n]", 256, qc_layer_extended_name);
				QC_SYSINFO_PARSE_LINE_STR(guesthdl, "UUID: %36s", 36, qc_layer_uuid);
			}
		}
	}
	rc = 0;
	goto out;

out_err:
	rc = -1;
out:
	qc_debug_indent_dec();

	return rc;
}

static int qc_derive_part_char_num(struct qc_handle *hdl) {
	char *str = NULL, *p, *sptr;
	int pchars = 0, rc = 0;
	const char *del = " ";

	if (qc_is_attr_set_string(hdl, qc_partition_char)) {
		p = qc_get_attr_value_string(hdl, qc_partition_char);
		if (!p)
			goto out;
		if ((str = strdup(p)) == NULL) {
			rc = 12;
			qc_debug(hdl, "Error: Failed to allocate memory in qc_derive_part_char_num: %s", strerror(errno));
			goto out;
		}
		for (p = strtok_r(str, del, &sptr); p; p = strtok_r(NULL, del, &sptr)) {
			if (!strncmp(p, "Shared", strlen("Shared")))
				pchars |= QC_PART_CHAR_SHARED;
			else if (!strncmp(p, "Dedicated", strlen("Dedicated")))
				pchars |= QC_PART_CHAR_DEDICATED;
			else if (!strncmp(p, "Limited", strlen("Limited")))
				pchars |= QC_PART_CHAR_LIMITED;
			else {
				qc_debug(hdl, "Error: Encountered unknown partition "
					"characteristics '%s' in string '%s'\n", p, str);
				rc = 13;
				goto out;
			}
		}
		qc_debug(hdl, "Derived qc_partition_char_num from '%s' to be %d\n", str, pchars);
		if (qc_set_attr_int(hdl, qc_partition_char_num, pchars, ATTR_SRC_SYSINFO)) {
			rc = 14;
			goto out;
		}
	}

out:
	free(str);

	return rc;
}

static int qc_fill_in_sysinfo_values_lpar(struct qc_handle *hdl, char **sptr, char **line) {
	char *sysi = NULL, str_buf[STR_BUF_SIZE];	// large enough for all sscanf() calls by far
	int int_buf, rc = -1, ps_mtid = -1, *i;

	qc_debug(hdl, "Retrieve /proc/sysinfo information for LPAR\n");
	qc_debug_indent_inc();
	for (; *line && strncmp(*line, "VM", 2); *line = strtok_r(sysi, qc_sysinfo_delim, sptr)) {
		sysi = NULL;
		if (strncmp(*line, "LPAR ", 5) != 0)
			continue;
		*line += 5;
		QC_SYSINFO_PARSE_LINE_INT(hdl, "Number: %i", qc_partition_number);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Characteristics: %25[^\n]s", 25, qc_partition_char);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Name: %8s", 8, qc_layer_name);
		QC_SYSINFO_PARSE_LINE_INT(hdl, "Adjustment: %i", qc_adjustment);
		if (strncmp(*line, "CPUs ", 5) == 0) {
			*line += 5;
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Total: %i", qc_num_core_total);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Configured: %i", qc_num_core_configured);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Standby: %i", qc_num_core_standby);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Reserved: %i", qc_num_core_reserved);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Dedicated: %i", qc_num_core_dedicated);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Shared: %i", qc_num_core_shared);
			QC_SYSINFO_PARSE_LINE_INT_inc(hdl, "G-MTID: %i", qc_num_cp_threads, 1);
			QC_SYSINFO_PARSE_LINE_INT_inc(hdl, "S-MTID: %i", qc_num_ifl_threads, 1);
			if (sscanf(*line, "PS-MTID: %i", &int_buf) > 0)
				ps_mtid = int_buf + 1;
		}
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Extended Name: %256[^\n]", 256, qc_layer_extended_name);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "UUID: %36s", 36, qc_layer_uuid);
	}
	rc = qc_derive_part_char_num(hdl);
	// Apply threshold provided by ps_mtid if set
	if (ps_mtid >= 0) {
		qc_debug(hdl, "Apply PS-MTID limit of %d\n", ps_mtid);
		if ((i = qc_get_attr_value_int(hdl, qc_num_cp_threads)) != NULL)
			*i = MIN(ps_mtid, *i);
		if ((i = qc_get_attr_value_int(hdl, qc_num_ifl_threads)) != NULL)
			*i = MIN(ps_mtid, *i);
	}

	goto out;

out_err:
	rc = -1;
out:
	qc_debug_indent_dec();

	return rc;
}

static int qc_fill_in_sysinfo_values_cec(struct qc_handle *hdl, char **sptr, char **line) {
	char *sysi = NULL, str_buf[STR_BUF_SIZE];	// large enough for all sscanf() calls by far
	int int_buf, rc = -1;
	float float_buf;

	qc_debug(hdl, "Retrieve /proc/sysinfo information for CEC\n");
	qc_debug_indent_inc();
	if (qc_set_attr_int(hdl, qc_layer_type_num, QC_LAYER_TYPE_CEC, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_int(hdl, qc_layer_category_num, QC_LAYER_CAT_HOST, ATTR_SRC_SYSINFO) ||
	    qc_set_attr_string(hdl, qc_layer_type, "CEC", ATTR_SRC_SYSINFO) ||
	    qc_set_attr_string(hdl, qc_layer_category, "HOST", ATTR_SRC_SYSINFO))
		goto out;
	for (; *line && strncmp(*line, "LPAR", 4); *line = strtok_r(sysi, qc_sysinfo_delim, sptr)) {
		sysi = NULL;
		if (strncmp(*line, "Adj", 3) == 0)
			continue;	// Lots of "Adjustment" lines that we can skip
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Manufacturer: %16s", 16, qc_manufacturer);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Type: %4s", 4, qc_type);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "LIC Identifier: %16s", 16, qc_lic_identifier);
		if (strncmp(*line, "Model: ", 7) == 0) {
			*line += 7;
			QC_SYSINFO_PARSE_LINE_STR_NOCONT(hdl, "%16s", 16, qc_model_capacity);
			QC_SYSINFO_PARSE_LINE_STR_NOCONT(hdl, "%*s %16s", 16, qc_model);
			continue;
		}
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Sequence Code: %16s", 16, qc_sequence_code);
		QC_SYSINFO_PARSE_LINE_STR(hdl, "Plant: %4s", 4, qc_plant);
		if (strncmp(*line, "Capacity ", 9) == 0) {
			*line += 9;
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Adj. Ind.: %i", qc_capacity_adjustment_indication);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Ch. Reason: %i", qc_capacity_change_reason);
		}
		if (strncmp(*line, "CPUs ", 5) == 0) {
			*line += 5;
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Total: %i", qc_num_core_total);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Configured: %i", qc_num_core_configured);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Standby: %i", qc_num_core_standby);
			QC_SYSINFO_PARSE_LINE_INT(hdl, "Reserved: %i", qc_num_core_reserved);
			QC_SYSINFO_PARSE_LINE_INT_inc(hdl, "G-MTID: %i", qc_num_cp_threads, 1);
			QC_SYSINFO_PARSE_LINE_INT_inc(hdl, "S-MTID: %i", qc_num_ifl_threads, 1);
		}
		QC_SYSINFO_PARSE_LINE_FLOAT(hdl, "Capability: %f", qc_capability);
		QC_SYSINFO_PARSE_LINE_FLOAT(hdl, "Secondary Capability: %f", qc_secondary_capability);
	}
	rc = 0;
	goto out;

out_err:
	rc = -1;
out:
	qc_debug_indent_dec();

	return rc;
}

static int qc_sysinfo_process(struct qc_handle *hdl, char *sysinfo) {
	struct qc_handle *lparhdl = qc_get_lpar_handle(hdl);
	char *sysi = NULL, *start, *sptr, *line;
	int rc = -1;

	fflush(stdout);
	qc_debug(hdl, "Process sysinfo\n");
	qc_debug_indent_inc();
	if (!sysinfo) {
		qc_debug(hdl, "qc_sysinfo_process() called with priv==NULL\n");
		goto out;
	}
	// create a global copy which is parsed in one go across the functions. Therefore, pass on
	// the strtok() saveptr and the last line parsed
	if ((start = sysi = qc_copy_sysinfo(hdl, sysinfo)) == NULL)
		goto out;
	line = strtok_r(sysi, qc_sysinfo_delim, &sptr);
	if (qc_fill_in_sysinfo_values_cec(hdl, &sptr, &line))
		goto out;
	if (qc_fill_in_sysinfo_values_lpar(lparhdl, &sptr, &line))
		goto out;
	if (line && qc_fill_in_sysinfo_values_vm(lparhdl, &sptr, &line))
		goto out;
	rc = 0;

out:
	free(sysi);
	qc_debug_indent_dec();

	return rc;
}

struct qc_data_src sysinfo = {qc_sysinfo_open,
			      qc_sysinfo_process,
			      qc_sysinfo_dump,
			      qc_sysinfo_close,
			      qc_sysinfo_lgm_check,
			      NULL};
