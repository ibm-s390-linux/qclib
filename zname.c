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
	printf("  -d, --debug          Increase debug level\n");
	printf("  -h, --help           Print usage information and exit\n");
	printf("  -i, --cpuid          Print the CPU identifier\n");
	printf("  -j, --json           Dump all available data in JSON format\n");
	printf("  -m, --model          Print model information\n");
	printf("  -n, --name           Print the model name (default)\n");
	printf("  -u, --manufacturer   Print the manufacturer\n");
	printf("  -v, --version        Print version information\n");
	printf("\n");
}

static void print_version() {
	printf("zname utility, qclib-%s\n", QC_VERSION);
}

int main(int argc, char **argv) {
	static struct option long_options[] = {
		{ "all",		no_argument, NULL, 'a'},
		{ "capacity",		no_argument, NULL, 'c'},
		{ "debug",		no_argument, NULL, 'd'},
		{ "cpuid",		no_argument, NULL, 'i'},
		{ "help",		no_argument, NULL, 'h'},
		{ "json",               no_argument, NULL, 'j'},
		{ "manufacturer",	no_argument, NULL, 'u'},
		{ "model",		no_argument, NULL, 'm'},
		{ "name",		no_argument, NULL, 'n'},
		{ "version",		no_argument, NULL, 'v'},
		{ 0,			0,	     0,    0  }
	};
	int layers, i, type, opts = 0, rc = 0, dbg = 0;
	void *hdl = NULL;
	int c, json = 0;

	setenv("QC_DEBUG_CONSOLE", "1", 1);

	while ((c = getopt_long(argc, argv, "acdhijmnuv", long_options, NULL)) != EOF) {
		switch (c) {
		case 'a': opts |= OPTS_ALL;
			  break;
		case 'c': opts |= OPTS_CAPACITY;
	  		  break;
		case 'd': dbg++;
			  break;
	  	case 'h': print_help();
	  		  return 0;
		case 'i': opts |= OPTS_CPUID;
			  break;
		case 'j': json = 1;
			  break;
		case 'm': opts |= OPTS_MODEL;
			  break;
		case 'n': opts |= OPTS_NAME;
			  break;
	  	case 'u': opts |= OPTS_MANUFACTURER;
	  	  	  break;
		case 'v': print_version();
			  return 0;
		default:  print_help();
			  return 1;
		}
	}
	if (dbg == 1)
		setenv("QC_DEBUG", "1", 1);
	if (dbg > 1)
		setenv("QC_DEBUG", "2", 1);
	if (json && opts) {
		fprintf(stderr, "Error: Specifiy either one of the options to print info, or --json\n");
			rc = 2;
		goto out;
	}

	if ((rc = get_handle(&hdl, &layers)) != 0)
		goto out;

	if (json) {
		qc_export_json(hdl);
		rc = 0;
		goto out;
	}

	if (!opts)
		opts = OPTS_NAME;

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
