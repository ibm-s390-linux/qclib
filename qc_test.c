/* Copyright IBM Corp. 2013, 2019 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "query_capacity.h"


int err_cnt = 0;
int attr_indent = 34;

void print_break() {
	printf("\n");
}

void print_separator(int indent) {
	printf("\n\n");
}

void print_header(int indent, int layer, const char* layer_name) {
	printf("%*s===== Layer %d: %s =============================================\n",
							indent, "", layer, layer_name);
}

const char *attr2char(enum qc_attr_id id) {
	switch (id) {
	case qc_layer_type: return "qc_layer_type";
	case qc_layer_category: return "qc_layer_category";
	case qc_layer_type_num: return "qc_layer_type_num";
	case qc_layer_category_num: return "qc_layer_category_num";
	case qc_layer_name: return "qc_layer_name";
	case qc_layer_extended_name: return "qc_layer_extended_name";
	case qc_layer_uuid: return "qc_layer_uuid";
	case qc_manufacturer: return "qc_manufacturer";
	case qc_type: return "qc_type";
	case qc_model_capacity: return "qc_model_capacity";
	case qc_model: return "qc_model";
	case qc_type_name: return "qc_type_name";
	case qc_type_family: return "qc_type_family";
	case qc_sequence_code: return "qc_sequence_code";
	case qc_lic_identifier: return "qc_lic_identifier";
	case qc_plant: return "qc_plant";
	case qc_num_cpu_total: return "qc_num_cpu_total";
	case qc_num_cpu_configured: return "qc_num_cpu_configured";
	case qc_num_cpu_standby: return "qc_num_cpu_standby";
	case qc_num_cpu_reserved: return "qc_num_cpu_reserved";
	case qc_num_cpu_dedicated: return "qc_num_cpu_dedicated";
	case qc_num_cpu_shared: return "qc_num_cpu_shared";
	case qc_num_cp_total: return "qc_num_cp_total";
	case qc_num_cp_dedicated: return "qc_num_cp_dedicated";
	case qc_num_cp_shared: return "qc_num_cp_shared";
	case qc_num_ifl_total: return "qc_num_ifl_total";
	case qc_num_ifl_dedicated: return "qc_num_ifl_dedicated";
	case qc_num_ifl_shared: return "qc_num_ifl_shared";
	case qc_capability: return "qc_capability";
	case qc_secondary_capability: return "qc_secondary_capability";
	case qc_capacity_adjustment_indication: return "qc_capacity_adjustment_indication";
	case qc_capacity_change_reason: return "qc_capacity_change_reason";
	case qc_partition_number: return "qc_partition_number";
	case qc_partition_char: return "qc_partition_char";
	case qc_partition_char_num: return "qc_partition_char_num";
	case qc_adjustment: return "qc_adjustment";
	case qc_cp_absolute_capping: return "qc_cp_absolute_capping";
	case qc_ifl_absolute_capping: return "qc_ifl_absolute_capping";
	case qc_cp_weight_capping: return "qc_cp_weight_capping";
	case qc_ifl_weight_capping: return "qc_ifl_weight_capping";
	case qc_cluster_name: return "qc_cluster_name";
	case qc_control_program_id: return "qc_control_program_id";
	case qc_limithard_consumption: return "qc_limithard_consumption";
	case qc_prorated_core_time: return "qc_prorated_core_time";
	case qc_cp_limithard_cap: return "qc_cp_limithard_cap";
	case qc_cp_capacity_cap: return "qc_cp_capacity_cap";
	case qc_ifl_limithard_cap: return "qc_ifl_limithard_cap";
	case qc_ifl_capacity_cap: return "qc_ifl_capacity_cap";
	case qc_capping: return "qc_capping";
	case qc_capping_num: return "qc_capping_num";
	case qc_mobility_enabled: return "qc_mobility_enabled";
	case qc_has_secure: return "qc_has_secure";
	case qc_secure: return "qc_secure";
	case qc_has_multiple_cpu_types: return "qc_has_multiple_cpu_types";
	case qc_cp_dispatch_limithard: return "qc_cp_dispatch_limithard";
	case qc_ifl_dispatch_limithard: return "qc_ifl_dispatch_limithard";
	case qc_cp_dispatch_type: return "qc_cp_dispatch_type";
	case qc_ifl_dispatch_type: return "qc_ifl_dispatch_type";
	case qc_cp_capped_capacity: return "qc_cp_capped_capacity";
	case qc_ifl_capped_capacity: return "qc_ifl_capped_capacity";
	case qc_num_cp_threads: return "qc_num_cp_threads";
	case qc_num_ifl_threads: return "qc_num_ifl_threads";
	case qc_num_core_total: return "qc_num_core_total";
	case qc_num_core_configured: return "qc_num_core_configured";
	case qc_num_core_standby: return "qc_num_core_standby";
	case qc_num_core_reserved: return "qc_num_core_reserved";
	case qc_num_core_dedicated: return "qc_num_core_dedicated";
	case qc_num_core_shared: return "qc_num_core_shared";
	case qc_ziip_absolute_capping: return "qc_ziip_absolute_capping";
	case qc_ziip_capacity_cap: return "qc_ziip_capacity_cap";
	case qc_ziip_capped_capacity: return "qc_ziip_capped_capacity";
	case qc_ziip_dispatch_limithard: return "qc_ziip_dispatch_limithard";
	case qc_ziip_dispatch_type: return "qc_ziip_dispatch_type";
	case qc_ziip_limithard_cap: return "qc_ziip_limithard_cap";
	case qc_ziip_weight_capping: return "qc_ziip_weight_capping";
	case qc_num_ziip_dedicated: return "qc_num_ziip_dedicated";
	case qc_num_ziip_shared: return "qc_num_ziip_shared";
	case qc_num_ziip_total: return "qc_num_ziip_total";
	case qc_num_ziip_threads: return "qc_num_ziip_threads";

	default: break;
	}

	return NULL;
}

void verify_nonexistence(void *hdl, enum qc_attr_id id, int layer) {
	const char *s;
	float f;
	int rc, i;

	rc = qc_get_attribute_string(hdl, id, layer, &s);
	if (rc != 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as string "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (s) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as string "
			"at layer %d returned %s\n", attr2char(id), layer, s);
		err_cnt++;
	}

	rc = qc_get_attribute_int(hdl, id, layer, &i);
	if (rc != 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as int "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (i >= 0) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as int "
			"at layer %d returned %d\n", attr2char(id), layer, i);
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, id, layer, &f);
	if (rc != 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as float "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as float "
			"at layer %d returned %f\n", attr2char(id), layer, f);
		err_cnt++;
	}
}

void verify_invalid(void *hdl, enum qc_attr_id id, int layer) {
	const char *s;
	float f;
	int rc, i;

	rc = qc_get_attribute_string(hdl, id, layer, &s);
	if (rc >= 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as string "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (s) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as string "
			"at layer %d returned %s\n", attr2char(id), layer, s);
		err_cnt++;
	}

	rc = qc_get_attribute_int(hdl, id, layer, &i);
	if (rc >= 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as int "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (i >= 0) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as int "
			"at layer %d returned %d\n", attr2char(id), layer, i);
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, id, layer, &f);
	if (rc >= 0) {
		printf("Error: Attribute '%s' (nonexistent) definition check as float "
			"at layer %d returned %d\n", attr2char(id), layer, rc);
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: Attribute '%s' (nonexistent) retrieval as float "
			"at layer %d returned %f\n", attr2char(id), layer, f);
		err_cnt++;
	}
}

int sanity_checks(void *hdl, int layers) {
	int i, rc;
	float f;
	const char *s;

	rc = qc_get_attribute_string(hdl, qc_layer_type, 0, &s);
	if (rc <= 0) {
		printf("Error retrieving qc_layer_type at layer 0\n");
		err_cnt++;
		return 1;
	}
	if (strcmp("CEC", s)) {
		printf("Error: First layer is not of type 'CEC'\n");
		err_cnt++;
		return 2;
	}
	rc = qc_get_attribute_string(hdl, qc_layer_type, 1, &s);
	if (rc <= 0) {
		printf("Error retrieving qc_layer_type at layer 1\n");
		err_cnt++;
		return 1;
	}
	if (strncmp("LPAR", s, strlen("LPAR"))) {
		printf("Oops. The second lowest layer is not LPAR. " \
		       "Exiting.\n");
		err_cnt++;
		return 3;
	}

	// query/retrieve nonexistent attribute
	for (i = 0; i <= layers; ++i)
		verify_invalid(hdl, 78923, i);

	// Check API NULL pointer (and related) handling
	qc_close(NULL);
	qc_get_num_layers(NULL, &rc);
	if (rc >= 0) {
		printf("Error: qc_get_num_layers(NULL, &rc) worked, returning '%d'\n", rc);
		err_cnt++;
	}
	rc = qc_get_attribute_string(NULL, qc_layer_type, 0, &s);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_string(NULL, qc_layer_type, 0, &rc) worked\n");
		err_cnt++;
	}
	if (s) {
		printf("Error: qc_get_attribute_string(NULL, qc_layer_type, 0, &rc) returned string\n");
		err_cnt++;
	}
	rc = qc_get_attribute_string(hdl, -95, 0, &s);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_string(hdl, -95, 0, &rc) worked\n");
		err_cnt++;
	}
	if (s) {
		printf("Error: qc_get_attribute_string(hdl, -95, 0, &rc) returned string\n");
		err_cnt++;
	}
	rc = qc_get_attribute_string(hdl, qc_layer_type, -1, &s);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_string(hdl, qc_layer_type, -1, &rc) worked\n");
		err_cnt++;
	}
	if (s) {
		printf("Error: qc_get_attribute_string(hdl, qc_layer_type, -1, &rc) returned string\n");
		err_cnt++;
	}

	rc = qc_get_attribute_int(NULL, qc_layer_type, 0, &i);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_int(NULL, qc_layer_type, 0, &rc) worked\n");
		err_cnt++;
	}
	if (i >= 0) {
		printf("Error: qc_get_attribute_int(NULL, qc_layer_type, 0, &rc) returned value\n");
		err_cnt++;
	}
	rc = qc_get_attribute_int(hdl, -95, 0, &i);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_int(hdl, -95, 0, &rc) worked\n");
		err_cnt++;
	}
	if (i >= 0) {
		printf("Error: qc_get_attribute_int(hdl, -95, 0, &rc) returned value\n");
		err_cnt++;
	}
	rc = qc_get_attribute_int(hdl, qc_layer_type, -1, &i);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_int(hdl, qc_layer_type, -1, &rc) worked\n");
		err_cnt++;
	}
	if (i >= 0) {
		printf("Error: qc_get_attribute_int(hdl, qc_layer_type, -1, &rc) returned value\n");
		err_cnt++;
	}

	rc = qc_get_attribute_float(NULL, qc_layer_type, 0, &f);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_float(NULL, qc_layer_type, 0, &rc) worked\n");
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: qc_get_attribute_float(NULL, qc_layer_type, 0, &rc) returned value\n");
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, -95, 0, &f);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_float(hdl, -05, 0, &rc) worked\n");
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: qc_get_attribute_float(hdl, -95, 0, &rc) returned value\n");
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, qc_layer_type, -1, &f);
	if (rc >= 0) {
		printf("Error: qc_get_attribute_float(hdl, qc_layer_type, -1, &rc) worked\n");
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: qc_get_attribute_float(hdl, qc_layer_type, -1, &rc) returned value\n");
		err_cnt++;
	}


	return 0;
}

void print_int_attr(void *hdl, enum qc_attr_id id, char *src, int layer, int indent) {
	int rc, val;
	float f;
	const char *s;

	rc = qc_get_attribute_int(hdl, id, layer, &val);
	switch (rc) {
	case 1:	if (val < 0) {
			printf("Error: Attribute '%s' is defined but not set!\n", attr2char(id));
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: %d\n", indent, "", attr_indent, attr2char(id), src, val);
		break;
	case 0:	if (val >= 0) {
			printf("Error: Attribute '%s' is not defined but set to %d\n", attr2char(id), val);
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: <undefined>\n", indent, "", attr_indent, attr2char(id), src);
		break;
	default: printf("Error: Attribute '%s' retrieval as int returned %d\n", attr2char(id), rc);
		err_cnt++;
	}
	// False positive tests
	rc = qc_get_attribute_string(hdl, id, layer, &s);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as string (is int) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (s) {
		printf("Error: Attribute '%s' retrieval as string (is int) returned %s\n",
			attr2char(id), s);
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, id, layer, &f);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as float (is int) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: Attribute '%s' retrieval as float (is int) returned %f\n",
			attr2char(id), f);
		err_cnt++;
	}
}

void print_float_attr(void *hdl, enum qc_attr_id id, char *src, int layer, int indent) {
	float fl;
	int rc, val;
	const char *s;

	rc = qc_get_attribute_float(hdl, id, layer, &fl);
	switch (rc) {
	case 1:	if (fl < 0) {
			printf("Error: Attribute '%s' is defined but not set!\n", attr2char(id));
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: %f\n", indent, "", attr_indent, attr2char(id), src, fl);
		break;
	case 0:	if (fl >= 0) {
			printf("Error: Attribute '%s' is not defined but set to %f\n", attr2char(id), fl);
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: <undefined>\n", indent, "", attr_indent, attr2char(id), src);
		break;
	default: printf("Error: Attribute '%s' retrieval as int returned %d\n", attr2char(id), rc);
		err_cnt++;
	}

	// False positive tests
	rc = qc_get_attribute_string(hdl, id, layer, &s);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as string (is float) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (s) {
		printf("Error: Attribute '%s' retrieval as string (is float) returned %s\n",
			attr2char(id), s);
		err_cnt++;
	}
	rc = qc_get_attribute_int(hdl, id, layer, &val);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as int (is float) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (val >= 0) {
		printf("Error: Attribute '%s' retrieval as int (is float) returned %d\n",
			attr2char(id), val);
		err_cnt++;
	}
}

void print_string_attr(void *hdl, enum qc_attr_id id, char *src, int layer, int indent) {
	const char *s;
	float f;
	int rc, val;

	rc = qc_get_attribute_string(hdl, id, layer, &s);
	switch (rc) {
	case 1:	if (!s) {
			printf("Error: Attribute '%s' is defined but not set!\n", attr2char(id));
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: %s\n", indent, "", attr_indent, attr2char(id), src, s);
		break;
	case 0:	if (s) {
			printf("Error: Attribute '%s' is not defined but set to '%s'\n", attr2char(id), s);
			err_cnt++;
			return;
		}
		printf("%*s%*s [%3s]: <undefined>\n", indent, "", attr_indent, attr2char(id), src);
		break;
	default: printf("Error: Attribute '%s' retrieval as string returned %d\n", attr2char(id), rc);
		err_cnt++;
	}

	// False positive tests
	rc = qc_get_attribute_int(hdl, id, layer, &val);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as int (is string) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (val >= 0) {
		printf("Error: Attribute '%s' retrieval as int (is string) returned %d\n",
			attr2char(id), val);
		err_cnt++;
	}
	rc = qc_get_attribute_float(hdl, id, layer, &f);
	if (rc != 0) {
		printf("Error: Attribute '%s' definition check as float (is string) returned rc=%d\n",
			attr2char(id), rc);
		err_cnt++;
	}
	if (f >= 0) {
		printf("Error: Attribute '%s' retrieval as float (is string) returned %f\n",
			attr2char(id), f);
		err_cnt++;
	}
}

void print_cec_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "CEC");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "F V", layer, indent);
	print_string_attr(hdl, qc_manufacturer, "S V", layer, indent);
	print_string_attr(hdl, qc_type, "S V", layer, indent);
	print_string_attr(hdl, qc_type_name, "S  ", layer, indent);
	print_int_attr(hdl, qc_type_family, "S  ", layer, indent);
	print_string_attr(hdl, qc_model_capacity, "S  ", layer, indent);
	print_string_attr(hdl, qc_model, "S  ", layer, indent);
	print_string_attr(hdl, qc_sequence_code, "S V", layer, indent);
	print_string_attr(hdl, qc_lic_identifier, "S  ", layer, indent);
	print_string_attr(hdl, qc_plant, "S V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_core_total, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_configured, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_standby, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_reserved, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_core_shared, " hV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, " hV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_threads, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ifl_threads, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ziip_threads, "S  ", layer, indent);

	print_break();
	print_float_attr(hdl, qc_capability, "S  ", layer, indent);
	print_float_attr(hdl, qc_secondary_capability, "S  ", layer, indent);
	print_int_attr(hdl, qc_capacity_adjustment_indication, "S  ", layer, indent);
	print_int_attr(hdl, qc_capacity_change_reason, "S  ", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_lpar_group_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "LPAR-Group");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, " hV", layer, indent);
	print_int_attr(hdl, qc_cp_absolute_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ifl_absolute_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ziip_absolute_capping, " hV", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_secondary_capability, layer);
}

void print_lpar_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "LPAR");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "S V", layer, indent);
	print_string_attr(hdl, qc_layer_extended_name, "S  ", layer, indent);
	print_string_attr(hdl, qc_layer_uuid, "S  ", layer, indent);
	print_int_attr(hdl, qc_partition_number, "S V", layer, indent);
	print_string_attr(hdl, qc_partition_char, "S  ", layer, indent);
	print_int_attr(hdl, qc_partition_char_num, "S  ", layer, indent);
	print_int_attr(hdl, qc_adjustment, "S  ", layer, indent);
	print_int_attr(hdl, qc_has_secure, "F  ", layer, indent);
	print_int_attr(hdl, qc_secure, "F  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_core_total, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_configured, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_standby, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_reserved, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_dedicated, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_core_shared, "S  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, " hV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_threads, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ifl_threads, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ziip_threads, "S  ", layer, indent);
	print_int_attr(hdl, qc_cp_absolute_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_cp_weight_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ifl_absolute_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ifl_weight_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ziip_absolute_capping, " hV", layer, indent);
	print_int_attr(hdl, qc_ziip_weight_capping, " hV", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_secondary_capability, layer);
}

void print_zvmhyp_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "z/VM-hypervisor");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "  V", layer, indent);
	print_string_attr(hdl, qc_cluster_name, "  V", layer, indent);
	print_string_attr(hdl, qc_control_program_id, "S  ", layer, indent);
	print_int_attr(hdl, qc_adjustment, "S  ", layer, indent);
	print_int_attr(hdl, qc_limithard_consumption, "  V", layer, indent);
	print_int_attr(hdl, qc_prorated_core_time, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_core_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_core_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_core_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_threads, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_threads, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_threads, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_zoshyp_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "z/OS-hypervisor");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "  V", layer, indent);
	print_string_attr(hdl, qc_cluster_name, "  V", layer, indent);
	print_string_attr(hdl, qc_control_program_id, "S  ", layer, indent);
	print_int_attr(hdl, qc_adjustment, "S  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_core_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_core_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_core_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_threads, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_threads, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_zvmpool_information(void *hdl, int layer, int indent) {
#ifdef CONFIG_V1_COMPATIBILITY
	print_header(indent, layer, "z/VM-CPU-pool");
#else
	print_header(indent, layer, "z/VM-resource-pool");
#endif
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_cp_limithard_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capacity_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_limithard_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_capacity_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_limithard_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capacity_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capped_capacity, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_zosresgroup_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "z/OS-tenant-resource-group");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_cp_limithard_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capacity_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_limithard_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capacity_cap, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capped_capacity, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_zoszcxserver_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "z/OS-zCX-Server");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "S V", layer, indent);
	print_string_attr(hdl, qc_capping, " H ", layer, indent);
	print_int_attr(hdl, qc_capping_num, " H ", layer, indent);
	print_int_attr(hdl, qc_has_secure, "F  ", layer, indent);
	print_int_attr(hdl, qc_secure, "F  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cpu_total, "S V", layer, indent);
	print_int_attr(hdl, qc_num_cpu_configured, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_standby, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_reserved, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_dedicated, " HV", layer, indent);
	print_int_attr(hdl, qc_num_cpu_shared, " HV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_cp_dispatch_type, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_dispatch_limithard, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_dispatch_type, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_dispatch_limithard, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capped_capacity, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_has_multiple_cpu_types, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_zvmguest_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "z/VM-guest");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "S V", layer, indent);
	print_string_attr(hdl, qc_capping, " H ", layer, indent);
	print_int_attr(hdl, qc_capping_num, " H ", layer, indent);
	print_int_attr(hdl, qc_mobility_enabled, "  V", layer, indent);
	print_int_attr(hdl, qc_has_secure, "F  ", layer, indent);
	print_int_attr(hdl, qc_secure, "F  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cpu_total, "S V", layer, indent);
	print_int_attr(hdl, qc_num_cpu_configured, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_standby, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_reserved, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_dedicated, " HV", layer, indent);
	print_int_attr(hdl, qc_num_cpu_shared, " HV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_dedicated, "  V", layer, indent);
	print_int_attr(hdl, qc_num_ziip_shared, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_cp_dispatch_type, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_dispatch_limithard, "  V", layer, indent);
	print_int_attr(hdl, qc_cp_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_dispatch_type, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_dispatch_limithard, "  V", layer, indent);
	print_int_attr(hdl, qc_ifl_capped_capacity, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_dispatch_type, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_dispatch_limithard, "  V", layer, indent);
	print_int_attr(hdl, qc_ziip_capped_capacity, "  V", layer, indent);

	print_break();
	print_int_attr(hdl, qc_has_multiple_cpu_types, "  V", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_kvmhyp_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "KVM-hypervisor");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_control_program_id, "S  ", layer, indent);
	print_int_attr(hdl, qc_adjustment, "S  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_core_total, "  V", layer, indent);
	print_int_attr(hdl, qc_num_core_dedicated, "SHV", layer, indent);
	print_int_attr(hdl, qc_num_core_shared, "SHV", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cp_total, " HV", layer, indent);
	print_int_attr(hdl, qc_num_cp_dedicated, " hV", layer, indent);
	print_int_attr(hdl, qc_num_cp_shared, " hV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_total, "SHV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, "ShV", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, "ShV", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

void print_kvmguest_information(void *hdl, int layer, int indent) {
	print_header(indent, layer, "KVM-guest");
	indent += 2;
	print_string_attr(hdl, qc_layer_type, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_category, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_type_num, "n/a", layer, indent);
	print_int_attr(hdl, qc_layer_category_num, "n/a", layer, indent);
	print_string_attr(hdl, qc_layer_name, "S  ", layer, indent);
	print_string_attr(hdl, qc_layer_extended_name, "S  ", layer, indent);
	print_string_attr(hdl, qc_layer_uuid, "S  ", layer, indent);
	print_int_attr(hdl, qc_has_secure, "F  ", layer, indent);
	print_int_attr(hdl, qc_secure, "F  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_cpu_total, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_configured, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_standby, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_reserved, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_dedicated, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_cpu_shared, "S  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_num_ifl_total, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ifl_dedicated, "S  ", layer, indent);
	print_int_attr(hdl, qc_num_ifl_shared, "S  ", layer, indent);

	print_break();
	print_int_attr(hdl, qc_ifl_dispatch_type, "SHV", layer, indent);

	// check an attribute that only exists at a different layer
	verify_nonexistence(hdl, qc_cp_absolute_capping, layer);
}

int get_handle(void **hdl, int *layers, int quiet) {
	int rc;

	*hdl = qc_open(&rc);
	if (rc < 0) {
		if (!quiet)
			printf("Error: Could not open capacity data, rc=%d\n", rc);
		return 1;
	}
	if (rc > 0) {
		if (!quiet)
			printf("Warning: Capacity data inconsistent, try again later (rc=%d)\n", rc);
		return 2;
	}
	if (!*hdl) {
		if (!quiet)
			printf("Error: Capacity data returned invalid handle, rc=%d\n", rc);
		return 3;
	}
	*layers = qc_get_num_layers(*hdl, &rc);
	if (rc != 0) {
		if (!quiet)
			printf("Error: Could not retrieve number of layers, rc=%d\n", rc);
		return 4;
	}
	if (*layers < 0) {
		if (!quiet)
			printf("Error: Invalid number of layers: %d\n", *layers);
		return 5;
	}

	return 0;
}

// Retrieve handle, dump data, and return *hdl to leave it at the caller's discretion when to close it
static void *run_test(int quiet, int fulltest) {
	int indent = 0, layers, i, etype;
	void *hdl = NULL, *hdl2 = NULL;

	err_cnt = 0;
	if (fulltest) {
		// First sanity check: Call with invalid handle before any were opened
		qc_get_num_layers((void*)0x1, &i);
		if (i >= 0) {
			printf("Error: qc_get_num_layers(0x1, &rc) worked, returning '%d'\n", i);
			err_cnt++;
		}
	}
	if (get_handle(&hdl, &layers, quiet) != 0) {
		err_cnt++;
		goto out;
	}
	if (quiet)
		goto out;
	print_break();
	printf("We are running %i layer(s)\n", layers);
	print_break();

	if (sanity_checks(hdl, layers))
		goto out;

	for (i = 0; i < layers; i++) {
		if (i > 0)
			print_separator(indent);
		indent += 2;
		if (qc_get_attribute_int(hdl, qc_layer_type_num, i, &etype) <= 0) {
			printf("Error: Failed to retrieve 'qc_layer_type_num'\n");
			err_cnt++;
			goto out;
		}
		switch (etype) {
		case QC_LAYER_TYPE_CEC:
			print_cec_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_LPAR_GROUP:
			print_lpar_group_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_LPAR:
			print_lpar_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZVM_HYPERVISOR:
			print_zvmhyp_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_KVM_HYPERVISOR:
			print_kvmhyp_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZOS_HYPERVISOR:
			print_zoshyp_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZVM_CPU_POOL:
			print_zvmpool_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZOS_TENANT_RESOURCE_GROUP:
			print_zosresgroup_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZVM_GUEST:
			print_zvmguest_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_KVM_GUEST:
			print_kvmguest_information(hdl, i, indent);
			break;
		case QC_LAYER_TYPE_ZOS_ZCX_SERVER:
			print_zoszcxserver_information(hdl, i, indent);
			break;
		default:
			printf("Error: Unhandled layer type '%d' at layer %i, no detailed "
				"information available.\n\n", etype, i);
			err_cnt++;
		}
	}
	if (fulltest) {
		// finally, get another handle before closing the existing one
		if (get_handle(&hdl2, &layers, quiet) != 0)
			err_cnt++;
	}

out:
	if (fulltest) {
		qc_close(hdl);
		hdl = hdl2;
	}
	/* disable debugging on final qc_close() call to have tracing properly disabled
	   without memleaks */
	setenv("QC_DEBUG", "0", 1);
	setenv("QC_AUTODUMP", "0", 1);
	if (!quiet) {
		print_separator(indent);
		if (err_cnt) {
			printf("%d error(s) detected\n", err_cnt);
			print_break();
		}
	}

	return hdl;
}

static void print_help() {
	printf("\n");
	printf("Usage: qc_test [-q] [-h] [<dump>*]\n");
	printf("\n");
	printf("Print live system information and perform self-test. Specify dumps to display\n");
	printf("previously dumped data instead (but skipping a minor part of the self-test).\n");
	printf("\n");
	printf("  -h, --help       Print usage information and exit\n");
	printf("  -q, --quiet      Quiet mode: Only gather system informtion; skip self-test and\n");
	printf("                   suppress any output.\n");
	printf("\n");
}

int main(int argc, char **argv) {
	static struct option long_options[] = {
		{ "help",  no_argument, NULL, 'h'},
		{ "quiet", no_argument, NULL, 'q'},
		{ 0,       0,		0,    0  }
	};
	int i, j, c, quiet = 0, rc = 0;
	void **hdls = NULL;

	while ((c = getopt_long(argc, argv, "hq", long_options, NULL)) != EOF) {
		switch (c) {
		case 'h': print_help();
			  return 0;
		case 'q': quiet = 1;
			  break;
		default:  print_help();
			  return 1;
		}
	}
	setenv("QC_CHECK_CONSISTENCY", "1", 0);
	hdls = malloc(argc * sizeof(void *));
	if (!hdls)
		return 1;
	if (optind < argc) {
		// dump(s) specified on command line - dump all, and close handles later on
		for (j = 0, i = optind; i < argc; ++i, ++j) {
			setenv("QC_USE_DUMP", argv[i], 1);
			if ((hdls[j] = run_test(quiet, 0)) == NULL)
				rc++;
		}
		for (--j; j >= 0; --j)
			qc_close(hdls[j]);
	} else {
		if ((hdls[0] = run_test(quiet, 1)) == NULL)
			rc = 1;
		qc_close(hdls[0]);
	}
	free(hdls);

	return rc;
}
