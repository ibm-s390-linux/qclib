/* Copyright IBM Corp. 2019, 2020 */

#include "zhypinfo.h"

#define OPTS_ALL		(1<<0)
#define OPTS_CAPACITY		(1<<1)
#define OPTS_CPUID		(1<<2)
#define OPTS_MODEL		(1<<3)
#define OPTS_MANUFACTURER	(1<<4)
#define OPTS_NAME		(1<<5)


int print_model_info(void *hdl, int layer, int opts) {
	const char *mdl_name, *cpu_id, *manufacturer, *capacity, *model;

	if (qc_get_attribute_string(hdl, qc_type_name, layer, &mdl_name) < 0
	    || qc_get_attribute_string(hdl, qc_type, layer, &cpu_id) < 0
	    || qc_get_attribute_string(hdl, qc_manufacturer, layer, &manufacturer) < 0
	    || qc_get_attribute_string(hdl, qc_model_capacity, layer, &capacity) < 0
	    || qc_get_attribute_string(hdl, qc_model, layer, &model) < 0)
		return 1;

	if (opts & OPTS_ALL) {
		printf("%s %s %s %s %s\n", (mdl_name ? mdl_name : "<unavailable>"),
					   (model ? model : "<unavailable>"),
					   (capacity ? capacity : "<unavailable>"),
					   (cpu_id ? cpu_id : "<unavailable>"),
					   (manufacturer ? manufacturer : "<unavailable>"));
	} else {
		if (opts & OPTS_NAME)
			printf("%s ", (mdl_name ? mdl_name : "<unavailable>"));
		if (opts & OPTS_MODEL)
			printf("%s ", (model ? model : "<unavailable>"));
		if (opts & OPTS_CAPACITY)
			printf("%s ", (capacity ? capacity : "<unavailable>"));
		if (opts & OPTS_CPUID)
			printf("%s ", (cpu_id ? cpu_id : "<unavailable>"));
		if (opts & OPTS_MANUFACTURER)
			printf("%s ", (manufacturer ? manufacturer : "<unavailable>"));
		printf("\n");
	}

	return 0;
}

static void print_help() {
	printf("\n");
	printf("Usage: zname [OPTIONS]\n");
	printf("\n");
	printf("Print information about IBM Z hardware.\n");
	printf("\n");
	printf("  -a, --all            Print all available information\n");
	printf("  -c, --capacity       Print capacity information\n");
	printf("  -h, --help           Print usage information and exit\n");
	printf("  -i, --cpuid          Print the CPU identifier\n");
	printf("  -m, --model          Print model information\n");
	printf("  -n, --name           Print the model name (default)\n");
	printf("  -u, --manufacturer   Print manufacturer information\n");
	printf("\n");
}

int main(int argc, char **argv) {
	static struct option long_options[] = {
		{ "all",		no_argument, NULL, 'a'},
		{ "capacity",		no_argument, NULL, 'c'},
		{ "cpuid",		no_argument, NULL, 'i'},
		{ "help",		no_argument, NULL, 'h'},
		{ "manufacturer",	no_argument, NULL, 'u'},
		{ "model",		no_argument, NULL, 'm'},
		{ "name",		no_argument, NULL, 'n'},
		{ 0,			0,	     0,    0  }
	};
	int layers, i, type, opts = 0, rc = 0;
	void *hdl = NULL;
	int c;

	while ((c = getopt_long(argc, argv, "acihumn", long_options, NULL)) != EOF) {
		switch (c) {
		case 'a': opts |= OPTS_ALL;
			  break;
		case 'c': opts |= OPTS_CAPACITY;
	  		  break;
	  	case 'h': print_help();
	  		  return 0;
		case 'i': opts |= OPTS_CPUID;
			  break;
		case 'm': opts |= OPTS_MODEL;
			  break;
		case 'n': opts |= OPTS_NAME;
			  break;
	  	case 'u': opts |= OPTS_MANUFACTURER;
	  	  	  break;
		default:  print_help();
			  return 1;
		}
	}
	if (!opts)
		opts = OPTS_NAME;

	if ((rc = get_handle(&hdl, &layers)) != 0)
		goto out;

	for (i = 0; i < layers; i++) {
		if (qc_get_attribute_int(hdl, qc_layer_type_num, i, &type) <= 0) {
			fprintf(stderr, "Error: Failed to retrieve layer type information\n");
			rc = 1;
			goto out;
		}
		if (type == QC_LAYER_TYPE_CEC) {
			rc = print_model_info(hdl, i, opts);
			goto out;
		}
	}
	fprintf(stderr, "Error: Could not retrieve CEC information\n");

out:
	qc_close(hdl);

	return rc;
}
