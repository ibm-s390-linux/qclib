/* Copyright IBM Corp. 2020 */

#include "zhypinfo.h"

static int get_max_level(void *hdl, int layers) {
	int lvl = 0, cat;

	for (; layers >= 0; layers--) {
		if (qc_get_attribute_int(hdl, qc_layer_category_num, layers, &cat) < 0)
			return -1;
		if (cat == QC_LAYER_CAT_HOST)
			lvl++;
	}

	return --lvl;
}

static void print_CPU(int count) {
	if (count < 0)
		printf("      -");
	else
		printf("  %5d", count);
}

static void print_pool_CPU(float count) {
	if (count < 0)
		printf("      -");
	else
		printf("  %5.1f", count / 65536.);
}

/** Calculate the total from two values that might include invalids (indicated by <0) */
static int get_total_from(int a, int b) {
	if (a >= 0) {
		if (b >= 0)
			return a + b;
		else
			return a;
	} else
		return b;
}

static void print_pool_cpu_counts(void *hdl, int layer, int id_cp, int id_ifl) {
	int cp, ifl, total;

	if (qc_get_attribute_int(hdl, id_ifl, layer, &ifl) <= 0)
		ifl = -1;
	if (qc_get_attribute_int(hdl, id_cp, layer, &cp) <= 0)
		cp = -1;
	total = get_total_from(ifl, cp);
	print_pool_CPU(ifl);
	print_pool_CPU(cp);
	print_pool_CPU(total);
}

static void print_cpu_counts(void *hdl, int layer, int id_ifl, int id_cp, int id_total) {
	int ifl, cp, total;

	if (qc_get_attribute_int(hdl, id_ifl, layer, &ifl) <= 0)
		ifl = -1;
	if (qc_get_attribute_int(hdl, id_cp, layer, &cp) <= 0)
		cp = -1;
	if (id_total >= 0) {
		if (qc_get_attribute_int(hdl, id_total, layer, &total) <= 0)
			total = -1;
	} else
		total = get_total_from(ifl, cp);
	print_CPU(ifl);
	print_CPU(cp);
	print_CPU(total);
}

static int print_layers(void *hdl, int print_lvls, int print_lays) {
	int rc, layer, lvl, ifl, cp, type_num, total, cat;
	const char *layer_type, *name, *type;

	layer = qc_get_num_layers(hdl, &rc) - 1;
	if (rc)
		return rc;
	lvl = get_max_level(hdl, layer);
	if (lvl < 0)
		return -1;
	if (print_lays) {
		printf("%d\n", layer + 1);
		return 0;
	}
	if (print_lvls) {
		printf("%d\n", lvl + 1);
		return 0;
	}
	printf("  #  Layer_Type                  Lvl  Categ  Name       IFLs    CPs  Total\n");
	printf("--------------------------------------------------------------------------\n");
	for (; layer >= 0; layer--) {
		if (qc_get_attribute_string(hdl, qc_layer_type, layer, &layer_type) < 0 ||
		    qc_get_attribute_string(hdl, qc_layer_category, layer, &type) < 0 ||
		    qc_get_attribute_string(hdl, qc_layer_name, layer, &name) < 0 ||
		    qc_get_attribute_int(hdl, qc_layer_category_num, layer, &cat) < 0 ||
		    qc_get_attribute_int(hdl, qc_layer_type_num, layer, &type_num) < 0)
			return -1;

		printf("%3d  %-26s  %3d  %-5s  %-8s", layer, layer_type, lvl, type, name ? name : "   -");
		// some layers don't have all CPU types or are very special
		switch (type_num) {
			case QC_LAYER_TYPE_LPAR_GROUP:
				print_pool_cpu_counts(hdl, layer, qc_cp_absolute_capping, qc_ifl_absolute_capping);
				break;
			case QC_LAYER_TYPE_ZOS_TENANT_RESOURCE_GROUP:
				print_pool_cpu_counts(hdl, layer, qc_cp_capped_capacity, qc_ziip_capped_capacity);
				break;
			case QC_LAYER_TYPE_ZVM_RESOURCE_POOL:
				print_pool_cpu_counts(hdl, layer, qc_cp_capped_capacity, qc_ifl_capped_capacity);
				break;
			case QC_LAYER_TYPE_CEC:
				// Getting the total is a bit more complicated
				total = -1;
				if (qc_get_attribute_int(hdl, qc_num_core_dedicated, layer, &ifl) > 0 &&
				    qc_get_attribute_int(hdl, qc_num_core_shared, layer, &cp) > 0)
					total = ifl + cp;
				if (qc_get_attribute_int(hdl, qc_num_ifl_total, layer, &ifl) <= 0)
					ifl = -1;
				if (qc_get_attribute_int(hdl, qc_num_cp_total, layer, &cp) <= 0)
					cp = -1;
				print_CPU(ifl);
				print_CPU(cp);
				print_CPU(total);
				break;
			case QC_LAYER_TYPE_LPAR:
				print_cpu_counts(hdl, layer, qc_num_ifl_total, qc_num_cp_total, -1);
				break;
			case QC_LAYER_TYPE_ZVM_HYPERVISOR:
			case QC_LAYER_TYPE_KVM_HYPERVISOR:
				print_cpu_counts(hdl, layer, qc_num_ifl_total, qc_num_cp_total, qc_num_core_total);
				break;
			case QC_LAYER_TYPE_KVM_GUEST:
				if (qc_get_attribute_int(hdl, qc_num_ifl_total, layer, &ifl) <= 0)
					ifl = -1;
				print_CPU(ifl);
				print_CPU(0);
				print_CPU(ifl);
				break;
			case QC_LAYER_TYPE_ZOS_HYPERVISOR:
				// we map zIIPs to IFLs for z/OS
				print_cpu_counts(hdl, layer, qc_num_ziip_total, qc_num_cp_total, -1);
				break;
			case QC_LAYER_TYPE_ZOS_ZCX_SERVER:
				// we map zIIPs to IFLs for z/OS
				print_cpu_counts(hdl, layer, qc_num_ziip_total, qc_num_cp_total, -1);
				break;
			default:
				print_cpu_counts(hdl, layer, qc_num_ifl_total, qc_num_cp_total, qc_num_cpu_total);
				break;
		}
		printf("\n");

		if (cat == QC_LAYER_CAT_HOST)
			lvl--;
	}

	return 0;
}

static void print_help() {
	printf("\n");
	printf("Usage: zhypinfo [OPTION]\n");
	printf("\n");
	printf("Print information about virtualization layers on IBM Z.\n");
	printf("\n");
	printf("  -h, --help           Print usage information and exit\n");
	printf("  -j, --json           Dump all available data in JSON format\n");
	printf("  -l, --layers         Print number of layers\n");
	printf("  -L, --levels         Print number of virtualization levels\n");
	printf("\n");
}

int main(int argc, char **argv) {
	static struct option long_options[] = {
		{ "help",		no_argument, NULL, 'h'},
		{ "json",		no_argument, NULL, 'j'},
		{ "layers",		no_argument, NULL, 'l'},
		{ "levels",		no_argument, NULL, 'L'},
		{ 0,			0,	     0,    0  }
	};
	int layers, rc = 0, json = 0, lvls = 0, lays = 0;
	void *hdl = NULL;
	int c;

	while ((c = getopt_long(argc, argv, "hjlL", long_options, NULL)) != EOF) {
		switch (c) {
		case 'h': print_help();
			  return 0;
		case 'j': json = 1;
			  break;
		case 'l': lays = 1;
			  break;
		case 'L': lvls = 1;
			  break;
		default:  print_help();
			  return 1;
		}
	}

	if ((rc = get_handle(&hdl, &layers)) != 0)
		goto out;
	if (json + lays + lvls > 1) {
		fprintf(stderr, "Error: Only one of options --json, --layers and --levels is allowed\n");
		return 2;
	}
	if (json) {
		qc_export_json(hdl);
		return 0;
	}
	rc = print_layers(hdl, lvls, lays);

out:
	qc_close(hdl);

	return rc;
}
