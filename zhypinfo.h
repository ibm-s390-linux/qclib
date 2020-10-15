/* Copyright IBM Corp. 2020 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "query_capacity.h"


int get_handle(void **hdl, int *layers) {
	int rc;

	*hdl = qc_open(&rc);
	if (rc < 0) {
		fprintf(stderr, "Error: Could not open capacity data, rc=%d\n", rc);
		return rc;
	}
	if (rc > 0) {
		fprintf(stderr, "Warning: Capacity data inconsistent, try again later (rc=%d)\n", rc);
		return rc;
	}
	if (!*hdl) {
		fprintf(stderr, "Error: Capacity data returned invalid handle, rc=%d\n", rc);
		return rc;
	}
	*layers = qc_get_num_layers(*hdl, &rc);
	if (rc != 0) {
		fprintf(stderr, "Error: Could not retrieve number of layers, rc=%d\n", rc);
		return rc;
	}
	if (*layers < 1) {
		fprintf(stderr, "Error: Invalid number of layers: %d\n", *layers);
		return 1;
	}

	return 0;
}
