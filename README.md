qclib (Query Capacity Library)
==============================

`qclib` provides a C API and associated utilities for extraction of system
information for Linux on IBM Z.

For instance, it will provide the number of CPUs

  * on the machine (**CEC**, Central Electronic Complex) layer
  * in **PR/SM** (Processor Resource/Systems Manager), i.e. visible to LPARs,
    including LPAR groups
  * in **z/VM** hosts, guests and resource pools
  * in **KVM** hosts and guests
  * in **zCX** hosts, tenant resource groups and servers

This allows calculation of the upper limit of CPU resources a highest level
guest can use.
E.g.: If an LPAR on a z13 provides 4 CPUs to a z/VM hypervisor, and the
hypervisor provides 8 virtual CPUs to a guest, `qclib` can be used to retrieve
all of these numbers, and it can be concluded that not more capacity than 4
CPUs can be used by the software running in the guest.

`qclib` uses various interfaces and provides this data through a common and
consistent API (Application Programming Interface), using information provided
by:

  * **STSI** (Store System Information) instruction - for more details, refer to
    '*z/Architecture Principles of Operation (SA22-7832)*'
  * **STHYI** (Store Hypervisor Information) instruction as provided by a z/VM
    hypervisor - for more information, refer to '*z/VM: V6R3 CP Programming
    Service*' (SC24-6179), chapter '*Store Hypervisor Information (STHYI)
    Instruction*'.
  * **hypfs** file system - for more information, refer to '*Device Drivers,
    Features, and Commands*', chapter '*S/390 hypervisor file system*'.
  * **Firmware** and other interfaces as made available through sysfs. For more
    information, refer to '*Device Drivers, Features, and Commands*', chapter
    '*Identifying the z Systems hardware*'.


Usage
-----
See [query_capacity.h](query_capacity.h) for details on how to use the API, and
[qc_test.c](qc_test.c) for a
sample program.


Requirements
------------
See [query_capacity.h](query_capacity.h) for details.


Build
-----
Use the following `make` targets:

  * `all` (default): Build static and dynamic libraries, as well as
           - `qc_test`: Sample program, statically linked
           - `qc_test-sh`: Sample program, dynamically linked
           - `zhypinfo`: Utility to print information about virtualization
                         layers on IBM Z.
           - `zname`: Utility to print information about the IBM Z hardware
  * `test`: Build and run the statically linked test program `qc_test`.
           Note: Requires a static version of `glibc`, which some distributions
           do not install by default.
  * `test-sh`: Build and run the dynamically linked test program `qc_test-sh`.
  * `doc`: Generate documentation (requires `doxygen 1.8.6` (or higher)) in
           subdirectory `html`.


API Documentation
-----------------
All documentation is available in file [query_capacity.h](query_capacity.h).
Alternatively, see `make` target `doc`.
documentation in subdirectory html, after using `make doc`.


License
-------
See [LICENSE](LICENSE).


Code Contributions
------------------
See [CONTRIBUTING.md](CONTRIBUTING.md).


Release History
---------------

* __v2.4.99 (tbd)__

    _Changes_:
    - `zname`, `zhypinfo`: Add command line switch `-d`
    - Removed now unused compile option `CONFIG_DEBUG_TIMESTAMPS`

    _Bug fixes_:


* __v2.4.1 (2023-12-17)__

    _Bug fixes_:
    - Handle systems with `/sys/kernel/security/lockdown` enabled
    - Consistency check: Fix accounting of reserved cores and CPUs
    - Trace: Fix display of version information
    - Doxygen config: Remove unused options

* __v2.4.0 (2023-04-08)__

    _Changes_:
    - Recognize single frame models and rackable models
    - `zname`: Add support for option `--json`
    - `qc_dump`: Add trace and JSON dump to `.tgz`

* __v2.3.2 (2022-09-19)__

    _Changes_:
    - Recognize IBM LinuxONE Emperor 4

* __v2.3.1 (2022-04-05)__

    _Changes_:
    - Recognize IBM z16

* __v2.3.0 (2021-04-28)__

    _Changes_:
    - Only export symbols exposed in public header
    - `config.doxygen`: Remove obsolete variables
    - `zname`, `zhypinfo`: Add version switch

* __v2.2.1 (2020-10-14)__

    _Bug fixes_:
    - `zhypinfo`: Fix memory leaks
    - `Makefile`: Various fixes

* __v2.2.0 (2020-09-09)__

    _Changes_:
    - Add new tools `zname` and `zhypinfo`

    _Bug fixes_:
    - KVM hosts in mixed mode LPARs indicated both, CPs and IFLs, while only
      CPs are actually used

* __v2.1.0 (2020-04-20)__

    _Changes_:
    - New attributes in support of secure boot in all final layers:
         * `qc_has_secure`
         * `qc_secure`
      Note: Changed src column indicator from `o` to `F` to summarize
            firmware-related data
    - Recognize IBM z15 single frame models

    _Bug fixes_:
    - `qc_open()`: Memory leaks on errors

* __v2.0.1 (2020-01-07)__

    _Changes_:
    - Retry up to three times when a live guest migration is detected

    _Bug fixes_:
    - Attribute `qc_num_ziip_threads` in CEC layer was not correctly set.
    - Do not account zIIPs in layer `QC_LAYER_TYPE_ZOS_ZCX_SERVER` for
      `qc_num_cpu_*` attributes.
    - `qc_dump`: Handle non-writable `/tmp`

* __v2.0.0 (2019-11-11)__

    _Changes_:
    - Add support for _z/OS Container Extensions_ (zCX)
    - New attributes in layer CEC:
         * `qc_type_name`
         * `qc_type_family`
         * `qc_lic_identifier`
    - `qc_test`: Reworked output for subtle consistency improvements.
    - Replaced attribute `qc_hardlimit_consumption` with
      `qc_limithard_consumption`. Use `CONFIG_V1_COMPATIBILITY` for previous
      version.
    - Require `CONFIG_DUMP_READING` in `query_capacity.h` to allow running from 
      a dump. Disabled by default.
    - Disabled v1 compatibility functionality per default. To re-enable,
      activate `CONFIG_V1_COMPATIBILITY` in `query_capacity.h`.

* __v1.4.1 (2018-06-25)__

    _Bug fixes_:
    - `qc_dump`: Don't abort the dump in case `qc_test` fails.
    - Attributes `qc_cp_weight_capping` and `qc_ifl_weight_capping` were set
      even when initial capping was not set in the LPAR's activation profile.

* __v1.4.0 (2018-04-10)__

    _Changes_:
    - Added SMT support by properly differentiating between cores and CPUs.
      I.e. switched from `qc_num_cpu_*` to `qc_num_core_*` attributes in layers
      CEC, LPAR, ZVM_HYPERVISOR and KVM_HYPERVISOR.
      NOTE: `qc_num_cpu_*` attributes remain to be valid in these cases to
            preserve backwards compatibility for now. This will be removed in
            one of the next releases! It is recommended to switch to the new
            attributes _now_ and test with `CONFIG_V1_COMPATIBILITY` disabled!
    - Added new attributes `qc_num_threads_cp` and `qc_num_threads_ifl` to
      layers CEC, LPAR and ZVM_HYPERVISOR.
    - Deprecated attribute `qc_mobility_eligible` (remains valid for now) and
      replaced with `qc_mobility_enabled` to match z/VM terminology. Likewise
      switched `QC_LAYER_TYPE_ZVM_CPU_POOL` to
      `QC_LAYER_TYPE_ZVM_RESOURCE_POOL`.
    - Moved build customization defines (e.g. `CONFIG_V1_COMPATIBILITY`) to
      `query_capacity.h`.
    - Do not build with textual hypfs per default anymore due to unrecoverable
      issues (see section 'Bug fixes'). Since all Linux distributions ship
      with debugfs (providing binary hypfs support), overriding textual hypfs,
      for years, this change will hardly ever be noticable. Enable `#define
      CONFIG_TEXTUAL_HYPFS` in `query_capacity.h` to revert.
      Note that textual hypfs support will be removed in a future release.

    _Bug fixes_:
    - Added an exception to consistency check to ignore inconsistencies between
      textual hypfs and STHYI for attributes `qc_num_cp_total` and
      `qc_num_ifl_total` in the LPAR layer.
      Background: Textual hypfs cannot tell whether a core is configured or
                  not. It therefore reports all cores as configured, which can
                  be wrong.

* __v1.3.1 (2018-01-18)__

    _Bug fixes_:
    - Security: Fix PATH attack vulnerability when dumping (see `QC_DEBUG=2`)
    - STHYI: Add fallback for pre-glibc 2.16 (not using `getauxval()`)
    - Handle mismatching STHYI and `/proc/sysinfo` layer counts
    - On LPAR, fix incomplete dump of binary hypfs when textual hypfs is mounted

* __v1.3.0 (2017-10-27)__

    _Changes_:
    - Added STHYI support in LPAR
    - Added new env variable `QC_DEBUG_FILE` (see `qc_open()`)
      Note: Failure to open a file for logging is now treated as a fatal error
    - Added script `qc_dump` to collect debug data in a standardized manner
    - Added attributes qc_layer_uuid and qc_layer_extended_name to LPAR layer
    - `/proc/sysinfo` parsing: Switch from "KVM/Linux" to the less strict "KVM"
      to detect KVM systems
    - Detect unregistered and closed handles
    - `Makefile`: Compile SONAME into shared library

    _Bug fixes_:
    - STHYI: Properly support `cc==3&&rc==4` as introduced in APAR VM65419
    - Logs: Fix month in timestamp (was off by 1)
    - `qc_test`: Fix flags for `qc_layer_name` in `QC_LAYER_TYPE_ZVM_HYPERVISOR`

* __v1.2.0 (2016-06-10)__

    _Changes_:
    - Removed source `[S]` for attributes `qc_num_cpu_dedicated` and
      `qc_num_cpu_shared` in LPAR layer for consistency
    - Retrieve `qc_layer_name` in CEC layer from OCF
    - Extended hypfs usage to provide more CP, IFL and CPU counts for
      CEC and LPAR layers, as well as capping information for LPAR group
      and LPAR layers
    - Added attributes for IFLs, CPs and CPUs for KVM hypervisor and guest
      layers
    - Added support for LPAR Groups
    - Added new attribute `qc_prorated_core_time`
    - Fill `qc_num_cp_total` and `qc_num_ifl_total` in LPAR layer from STHYI
    - Consistency checks: Detect inconsistent values across data sources
    - Documentation improvements

    _Bug fixes_:
    - Fixed crash when setting `QC_USE_DUMP` to an invalid directory and
      `QC_DEBUG=1`
    - Fixed reset of debug level when `QC_CHECK_CONSISTENCY` is invalid
    - Display all values in attribute `qc_partition_char` in case of multiple
    - Set `qc_cp_dispatch_type` in presence of CPs only

* __v1.1.0 (2016-02-15)__

    _Changes_:
    - `Makefile`: Added targets `clean` and `install`
    - `qc_test`: Support command line options
    - Performance improvements
    - Consistency checks: Turned into a run-time option. Disabled per
      default, enabled in `qc_test`

    _Bug fixes_:
    - Makefile: Fixed breakages, including when run in verbose mode
    - Fixed handling of hostnames with <8 characters in presence of hypfs
    - If no SSI cluster was present, attribute `qc_cluster_name` was set
      to an empty string instead of being left undefined
    - Consistency checks: Fixed wrong positive
    - Fixed source indicators in log

* __v1.0.0 (2015-08-28)__

    _Changes_:
    - Introduced new API, replacing the previous one
    - Renamed the following attributes for consistency:
        * `qc_container_name` became `qc_layer_name`
        * `qc_entity_*` became `qc_layer_*`
    - Introduced the following new attributes for a numeric representation
      of the respective string attributes:
        * `qc_layer_type_num`      (alternative to `qc_layer_type`)
        * `qc_layer_category_num`  (alternative to `qc_layer_category`)
        * `qc_partition_char_num`  (alternative to `qc_partition_char`)
        * `qc_capping_num`         (alternative to `qc_capping`)
    - Removed/renamed the following attributes, since they were duplicates of
      other layers' content:
        * In layer type `QC_LAYER_TYPE_ZVM_GUEST`: Removed `qc_hyp_*`,
          `qc_pool_*`, `qc_system_identifier`, `qc_cluster_name`,
          `qc_control_program_id`, `qc_adjustment`, and
          `qc_hardlimit_consumption`
        * In layer type `QC_LAYER_TYPE_ZVM_CPU_POOL`: Removed `qc_hyp_*`,
          `qc_system_identifier`, `qc_cluster_name`, `qc_hardlimit_consumption`,
          and renamed `qc_pool_*` to `qc_*`
    - Added support for KVM hypervisor
    - Added logging facility
    - Added dump support: Capability to create and run on dumps
    - Added autodump support: Create dumps on errors only
    - Added doxygen support for API description in query_capacity.h
    - Added support for hypfs mounted at arbitrary locations
    - Added support for binary hypfs API (requires RHEL6.1 and SLES11.2 or
      higher)
    - Added detection of Live Guest Migration while retrieving data
    - Handled NULL pointer arguments in all API calls
    - Reported errors as such when occurring while searching for capacity
      information

    _Bug fixes_:
    - Handled file access errors
    - Enabled attributes that were incorrectly indicated as not present
    - Fixed `qc_get_num_layers()` to return the number of layers
      (as documented), not the highest index
    - Fixed race by reading `/proc/sysinfo` only once
    - Only set `qc_ifl_dispatch_type` in presence of IFLs (as intended)

* __v0.9.2__

    _Bug fixes_:
    - Fixed memory leaks

* __v0.9.1__

    _Bug fixes_:
    - Fixed crash with more than 1 layers of nested z/VM virtualization
    - Fixed crash on 1st layer z/VM systems with hypfs
    - Fix: Information from /proc/sysinfo was collected in wrong sequence
           with more than 1 layers of nested virtualization
    - Fixed left open file handles in hypfs parsing code.
    - Added consistency check for hypfs

* __v0.9.0__

    _Initial version_



Copyright IBM Corp. 2013, 2020
