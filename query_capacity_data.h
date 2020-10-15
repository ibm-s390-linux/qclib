/* Copyright IBM Corp. 2013, 2015 */

#ifndef QUERY_CAPACITY_DATA
#define QUERY_CAPACITY_DATA


#include "query_capacity_int.h"


/* Functions to set and get attributes */
int qc_set_attr_int(struct qc_handle *hdl, enum qc_attr_id id, int val, char src);
int qc_set_attr_float(struct qc_handle *hdl, enum qc_attr_id id, float val, char src);
int qc_set_attr_string(struct qc_handle *hdl, enum qc_attr_id id, const char *val, char src);
int qc_set_attr_ebcdic_string(struct qc_handle *hdl, enum qc_attr_id id, unsigned char *val, unsigned int val_len, char src);
int qc_is_attr_set_int(struct qc_handle *hdl, enum qc_attr_id id);
int qc_is_attr_set_float(struct qc_handle *hdl, enum qc_attr_id id);
int qc_is_attr_set_string(struct qc_handle *hdl, enum qc_attr_id id);

const char *qc_attr_id_to_char(struct qc_handle *hdl, enum qc_attr_id id);

int   *qc_get_attr_value_int(struct qc_handle *hdl, enum qc_attr_id id);
float *qc_get_attr_value_float(struct qc_handle *hdl, enum qc_attr_id id);
char  *qc_get_attr_value_string(struct qc_handle *hdl, enum qc_attr_id id);

// Result is undefined in case attribute doesn't exist
char qc_get_attr_value_src_int(struct qc_handle *hdl, enum qc_attr_id id);
char qc_get_attr_value_src_float(struct qc_handle *hdl, enum qc_attr_id id);
char qc_get_attr_value_src_string(struct qc_handle *hdl, enum qc_attr_id id);

// print all attributes in the list in json format
void qc_print_attrs_json(struct qc_handle *hdl, int indent);
#endif
