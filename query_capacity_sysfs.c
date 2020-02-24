/* Copyright IBM Corp. 2020 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "query_capacity_data.h"


#define SYSFS_NA		0
#define SYSFS_AVAILABLE		1

#define FILE_CPC_NAME		"/sys/firmware/ocf/cpc_name"
#define FILE_SEC_IPL_HAS_SEC	"/sys/firmware/ipl/has_secure"
#define FILE_SEC_IPL_SEC	"/sys/firmware/ipl/secure"

static const char *sysfs_dirs[] = {"/sys",
				   "/sys/firmware",
				   "/sys/firmware/ocf",
				   "/sys/firmware/ipl",
				   NULL
				  };

struct sysfs_priv {
	int		avail;
	char	       *cpc_name;	// NULL if n/a
	int 		has_secure;	// <0 if n/a
	int		secure;		// <0 if n/a
};

static int qc_sysfs_mkpath(struct qc_handle *hdl, const char *a, const char *b, char **path) {
	free(*path);
	*path = NULL;

	if (asprintf(path, "%s/%s", a, b) == -1) {
		qc_debug(hdl, "Error: Mem alloc failed\n");
		return -1;
	}

	return 0;
}

/** Create directory structure that we need for our dumps - if we don't have any content later on,
    then there simply won't be any files in there */
static int qc_sysfs_create_dump_dirs(struct qc_handle *hdl) {
	char *path = NULL;
	int rc = -1, i;

	for (i = 0; sysfs_dirs[i]; ++i) {
		if (qc_sysfs_mkpath(hdl, qc_dbg_dump_dir, sysfs_dirs[i], &path))
			goto out;
		if (mkdir(path, 0700) == -1) {
			qc_debug(hdl, "Error: Could not create directory %s for sysfs dump: %s\n", path, strerror(errno));
			goto out;
		}
	}
	rc = 0;

out:
	free(path);

	return rc;
}

static FILE *qc_sysfs_open_dump_file(struct qc_handle *hdl, const char* file) {
	char *path = NULL;
	FILE *fp = NULL;

	if (qc_sysfs_mkpath(hdl, qc_dbg_dump_dir, file, &path))
		goto out;
	if ((fp = fopen(path, "w")) == NULL) {
		qc_debug(hdl, "Error: Failed to open '%s' to write sysfs dump\n", path);
		goto out;
	}

out:
	free(path);

	return fp;
}

static int qc_sysfs_dump_file_char(struct qc_handle *hdl, const char* file, const char *val) {
	FILE *fp;
	int rc;

	if (!val) {
		qc_debug(hdl, "No data for '%s', skipping\n", file);
		return 0;
	}
	fp = qc_sysfs_open_dump_file(hdl, file);
	if (!fp)
		return -1;
	rc = fprintf(fp, "%s", val);
	fclose(fp);
	if (rc < 0) {
		qc_debug(hdl, "Error: Failed to write dump to '%s'\n", file);
		return -1;
	}

	return 0;
}

static int qc_sysfs_dump_file_int(struct qc_handle *hdl, const char* file, int val) {
	FILE *fp;
	int rc;

	if (val < 0) {
		qc_debug(hdl, "No data for '%s', skipping\n", file);
		return 0;
	}
	fp = qc_sysfs_open_dump_file(hdl, file);
	if (!fp)
		return -1;
	rc = fprintf(fp, "%d", val);
	fclose(fp);
	if (rc < 0) {
		qc_debug(hdl, "Error: Failed to write dump to '%s'\n", file);
		return -1;
	}

	return 0;
}

static void qc_sysfs_dump(struct qc_handle *hdl, char *data) {
	struct sysfs_priv *p;

	qc_debug(hdl, "Dump sysfs\n");
	qc_debug_indent_inc();
	if (!data)
		goto out;
	p = (struct sysfs_priv *)data;
	if (qc_sysfs_create_dump_dirs(hdl)) {
		qc_debug(hdl, "Error: Failed to create directory structure\n");
		goto out_err;
	}
	if (qc_sysfs_dump_file_char(hdl, FILE_CPC_NAME, p->cpc_name) ||
	    qc_sysfs_dump_file_int(hdl, FILE_SEC_IPL_HAS_SEC, p->has_secure) ||
	    qc_sysfs_dump_file_int(hdl, FILE_SEC_IPL_SEC, p->secure))
	    	goto out_err;
	qc_debug(hdl, "sysfs data dumped to '%s%s'\n", qc_dbg_dump_dir, *sysfs_dirs);
	goto out;

out_err:
	qc_mark_dump_incomplete(hdl, "sysfs");
out:
	qc_debug_indent_dec();

	return;
}

static int qc_sysfs_is_old_dump_format(struct qc_handle *hdl) {
	char *fname = NULL;
	int rc = -1;

	if (qc_sysfs_mkpath(hdl, qc_dbg_use_dump, "ocf", &fname))
		goto out;
	if (access(fname, F_OK) == 0) {
		qc_debug(hdl, "Old ocf-based dump format\n");
		rc = 1;
		goto out;
	}
	rc = 0;
out:
	free(fname);

	return rc;
}

/** On success, returns 0 on success and filles data with respective file content.
    Returns >0 if file is not available, and <0 on error. */
static int qc_sysfs_get_file_content(struct qc_handle *hdl, char *file, char **content) {
	FILE *fp = NULL;
	size_t n;
	int rc;

	if (access(file, F_OK)) {
		qc_debug(hdl, "File '%s' not available\n", file);
		*content = NULL;
		return 1;
	}
	fp = fopen(file, "r");
	if (!fp) {
		qc_debug(hdl, "Error: Failed to open file '%s': %s\n", file, strerror(errno));
		return -1;
	}
	rc = getline(content, &n, fp);
	fclose(fp);
	if (rc == -1) {
		qc_debug(hdl, "Error: Failed to read content of '%s': %s\n", file, strerror(errno));
		*content = NULL;
		return -2;
	}
	rc = 0;
	if (strcmp(*content, "\n") == 0 || **content == '\0') {
		qc_debug(hdl, "'%s' contains no data, discarding\n", file);
		free(*content);
		*content = NULL;
		return 2;
	}
	qc_debug(hdl, "Read file %s\n", file);

	return 0;
}

/** Handle numeric attributes */
static int qc_sysfs_num_attr(struct qc_handle *hdl, char *file, int *attr) {
	char *content = NULL;
	int rc;

	rc = qc_sysfs_get_file_content(hdl, file, &content);
	if (rc) {
		*attr = -1;
		if (rc > 0)
			rc = 0;
		goto out;
	}
	*attr = atoi(content);
	if (*attr < 0) {
		// we're not prepared to handle negative values (yet)
		qc_debug(hdl, "Negative content for '%s': %s\n", file, content);
		rc = -1;
	}

out:
	free(content);

	return rc;
}

static struct sysfs_priv *qc_sysfs_init_data(struct qc_handle *hdl, char **data) {
	struct sysfs_priv *p;

	if ((*data = malloc(sizeof(struct sysfs_priv))) == NULL) {
		qc_debug(hdl, "Error: Failed to allocate private data for sysfs\n");
		return NULL;
	}
	p = (struct sysfs_priv *)*data;
	memset(p, 0, sizeof(struct sysfs_priv));
	p->avail = SYSFS_NA;
	p->has_secure = -1;
	p->secure = -1;

	return p;
}

static int qc_sysfs_open(struct qc_handle *hdl, char **data) {
	struct sysfs_priv *p;
	char *path = NULL;
	int rc = 0, lrc;

	qc_debug(hdl, "Retrieve sysfs data\n");
	qc_debug_indent_inc();
	p = qc_sysfs_init_data(hdl, data);
	if (!p) {
		rc = -1;
		goto out;
	}
	if (qc_dbg_use_dump) {
		qc_debug(hdl, "Read sysfs from dump\n");
		if (qc_sysfs_is_old_dump_format(hdl)) {
			// Note: previously, we had a directory called 'ocf' where only one piece of data was
			//       residing. But we have switched over to a more general sys directory instead.
			qc_debug(hdl, "Old, ocf-based format\n");
			if (qc_sysfs_mkpath(hdl, qc_dbg_use_dump, "ocf/cpc_name", &path)) {
				rc = -1;
				goto out;
			}
			lrc = qc_sysfs_get_file_content(hdl, path, &p->cpc_name);
			if (lrc != 0) {
				rc = (lrc < 0 ? -1 : 0);
				goto out;
			}
			p->avail = SYSFS_AVAILABLE;
		} else {
			qc_debug(hdl, "New, sysfs-based format\n");
			if (qc_sysfs_mkpath(hdl, qc_dbg_use_dump, FILE_CPC_NAME, &path) ||
			    qc_sysfs_get_file_content(hdl, path, &p->cpc_name) < 0 ||
			    qc_sysfs_mkpath(hdl, qc_dbg_use_dump, FILE_SEC_IPL_HAS_SEC, &path) ||
			    qc_sysfs_num_attr(hdl, path, &p->has_secure) ||
			    qc_sysfs_mkpath(hdl, qc_dbg_use_dump, FILE_SEC_IPL_SEC, &path) ||
			    qc_sysfs_num_attr(hdl, path, &p->secure))
				rc = -1;
			else
				p->avail = SYSFS_AVAILABLE;
		}
	} else {
		qc_debug(hdl, "Read sysfs from system\n");
		if (qc_sysfs_get_file_content(hdl, FILE_CPC_NAME, &p->cpc_name) < 0 ||
		    qc_sysfs_num_attr(hdl, FILE_SEC_IPL_HAS_SEC, &p->has_secure) ||
		    qc_sysfs_num_attr(hdl, FILE_SEC_IPL_SEC, &p->secure))
		    	rc = -1;
		else
			p->avail = SYSFS_AVAILABLE;
	}

out:
	qc_debug(hdl, "Done reading sysfs data\n");
	qc_debug_indent_dec();
	free(path);

	return rc;
}

static void qc_sysfs_close(struct qc_handle *hdl, char *data) {
	struct sysfs_priv *p = (struct sysfs_priv *)data;

	if (p) {
		free(p->cpc_name);
		free(data);
	}
}

static int qc_sysfs_process(struct qc_handle *hdl, char *data) {
	struct sysfs_priv *p = (struct sysfs_priv *)data;
	int rc = 0;

	qc_debug(hdl, "Process sysfs\n");
	qc_debug_indent_inc();
	if (!p) {
		qc_debug(hdl, "No sysfs data, skipping\n");
		goto out;
	}

	// Set CEC layer attribute
	if ((p->cpc_name && qc_set_attr_string(hdl->root, qc_layer_name, p->cpc_name, ATTR_SRC_SYSFS))) {
		rc = -1;
		goto out;
	}

	// Set top layer attributes.
	// Note: This implies that all top layers must feature these attributes!
	hdl = qc_get_top_handle(hdl);
	if ((p->has_secure >= 0 && qc_set_attr_int(hdl, qc_has_secure, p->has_secure, ATTR_SRC_SYSFS)) ||
	    (p->secure >= 0 && qc_set_attr_int(hdl, qc_secure, p->secure, ATTR_SRC_SYSFS))) {
		rc = -1;
		goto out;
	}

out:
	qc_debug_indent_dec();

	return rc;
}

struct qc_data_src sysfs = {qc_sysfs_open,
			    qc_sysfs_process,
			    qc_sysfs_dump,
			    qc_sysfs_close,
			    NULL,
			    NULL};
