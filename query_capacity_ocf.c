/* Copyright IBM Corp. 2016 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "query_capacity_data.h"


#define FILE_CPC_NAME		"/sys/firmware/ocf/cpc_name"


static void qc_ocf_dump(struct qc_handle *hdl, char *data) {
	char *path = NULL;
	FILE *fp;
	int rc;

	qc_debug(hdl, "Dump ocf\n");
	qc_debug_indent_inc();
	if (!data)
		goto out;
	if (asprintf(&path, "%s/ocf", qc_dbg_dump_dir) == -1) {
		qc_debug(hdl, "Error: Mem alloc failure, cannot dump sysinfo\n");
		goto out_err;
	}
	if (mkdir(path, 0700) == -1) {
		qc_debug(hdl, "Error: Could not create directory for ocf dump: %s\n", strerror(errno));
		goto out_err;
	}
	free(path);
	if (asprintf(&path, "%s/ocf/cpc_name", qc_dbg_dump_dir) == -1) {
		qc_debug(hdl, "Error: Mem alloc failure, cannot dump sysinfo\n");
		goto out_err;
	}
	if ((fp = fopen(path, "w")) == NULL) {
		qc_debug(hdl, "Error: Failed to open %s to write ocf dump\n", path);
		goto out_err;
	}
	rc = fprintf(fp, "%s", data);
	fclose(fp);
	if (rc < 0) {
		qc_debug(hdl, "Error: Failed to write dump to '%s'\n", path);
		goto out_err;
	}
	goto out;

out_err:
	qc_mark_dump_incomplete(hdl, "ocf");
out:
	free(path);
	qc_debug_indent_dec();

	return;
}

static int qc_ocf_open(struct qc_handle *hdl, char **data) {
	char *fname = NULL;
	int rc = 0;
	size_t n;
	FILE *fp;

	qc_debug(hdl, "Retrieve ocf data\n");
	qc_debug_indent_inc();
	*data = NULL;
	if (qc_dbg_use_dump) {
		qc_debug(hdl, "Read ocf from dump\n");
		if (asprintf(&fname, "%s/ocf/cpc_name", qc_dbg_use_dump) == -1) {
			qc_debug(hdl, "Error: Mem alloc failed, cannot open dump\n");
			rc = -1;
			goto out;
		}
	} else
		qc_debug(hdl, "Read ocf from " FILE_CPC_NAME "\n");

	if (access(fname ? fname : FILE_CPC_NAME, F_OK)) {
		qc_debug(hdl, "No ocf data available\n");
		goto out;
	}
	fp = fopen(fname ? fname : FILE_CPC_NAME, "r");
	if (!fp) {
		qc_debug(hdl, "Error: Failed to open file '%s': %s\n",
			 fname ? fname : FILE_CPC_NAME, strerror(errno));
		rc = -2;
		goto out;
	}
	rc = getline(data, &n, fp);
	fclose(fp);
	if (rc == -1) {
		qc_debug(hdl, "Error: Failed to read content: %s\n", strerror(errno));
		*data = NULL;
		rc = -3;
		goto out;
	}
	rc = 0;
	if (strcmp(*data, "\n") == 0 || **data == '\0') {
		qc_debug(hdl, FILE_CPC_NAME " contains no data, discarding\n");
		free(*data);
		*data = NULL;
		goto out;
	}

out:
	free(fname);
	qc_debug(hdl, "Done reading ocf data\n");
	qc_debug_indent_dec();

	return rc;
}

static void qc_ocf_close(struct qc_handle *hdl, char *data) {
	free(data);
}

static int qc_ocf_process(struct qc_handle *hdl, char *data) {
	qc_debug(hdl, "Process ocf\n");
	qc_debug_indent_inc();
	if (!data) {
		qc_debug(hdl, "No ocf data, skipping\n");
		goto out;
	}
	if (qc_set_attr_string(hdl->root, qc_layer_name, data, ATTR_SRC_OCF))
		qc_debug(hdl, "Error: Failed to set CEC name to %s\n", data);
out:
	qc_debug_indent_dec();

	return 0;
}

struct qc_data_src ocf = {qc_ocf_open,
			  qc_ocf_process,
			  qc_ocf_dump,
			  qc_ocf_close,
			  NULL,
			  NULL};
