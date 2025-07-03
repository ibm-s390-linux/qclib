#![allow(nonstandard_style)]

pub type qc_layer_type = ::std::os::raw::c_uint;
pub const QC_LAYER_TYPE_CEC: qc_layer_type = 1;
pub const QC_LAYER_TYPE_LPAR_GROUP: qc_layer_type = 8;
pub const QC_LAYER_TYPE_LPAR: qc_layer_type = 2;
pub const QC_LAYER_TYPE_ZVM_HYPERVISOR: qc_layer_type = 3;
pub const QC_LAYER_TYPE_ZVM_CPU_POOL: qc_layer_type = 4;
pub const QC_LAYER_TYPE_ZVM_RESOURCE_POOL: qc_layer_type = 4;
pub const QC_LAYER_TYPE_ZVM_GUEST: qc_layer_type = 5;
pub const QC_LAYER_TYPE_KVM_HYPERVISOR: qc_layer_type = 6;
pub const QC_LAYER_TYPE_KVM_GUEST: qc_layer_type = 7;
pub const QC_LAYER_TYPE_ZOS_HYPERVISOR: qc_layer_type = 9;
pub const QC_LAYER_TYPE_ZOS_TENANT_RESOURCE_GROUP: qc_layer_type = 10;
pub const QC_LAYER_TYPE_ZOS_ZCX_SERVER: qc_layer_type = 11;

pub type qc_layer_category = ::std::os::raw::c_uint;
pub const QC_LAYER_CAT_GUEST: qc_layer_category = 1;
pub const QC_LAYER_CAT_HOST: qc_layer_category = 2;
pub const QC_LAYER_CAT_POOL: qc_layer_category = 3;

pub type qc_attr_id = ::std::os::raw::c_uint;
pub const QC_ADJUSTMENT: qc_attr_id = 0;
pub const QC_CAPABILITY: qc_attr_id = 1;
pub const QC_CAPACITY_ADJUSTMENT_INDICATION: qc_attr_id = 2;
pub const QC_CAPACITY_CHANGE_REASON: qc_attr_id = 3;
pub const QC_CAPPING: qc_attr_id = 4;
pub const QC_CAPPING_NUM: qc_attr_id = 5;
pub const QC_CLUSTER_NAME: qc_attr_id = 6;
pub const QC_CONTROL_PROGRAM_ID: qc_attr_id = 7;
pub const QC_CP_ABSOLUTE_CAPPING: qc_attr_id = 8;
pub const QC_CP_CAPACITY_CAP: qc_attr_id = 9;
pub const QC_CP_CAPPED_CAPACITY: qc_attr_id = 10;
pub const QC_CP_DISPATCH_LIMITHARD: qc_attr_id = 11;
pub const QC_CP_DISPATCH_TYPE: qc_attr_id = 12;
pub const QC_CP_LIMITHARD_CAP: qc_attr_id = 13;
pub const QC_CP_WEIGHT_CAPPING: qc_attr_id = 14;
pub const QC_LIMITHARD_CONSUMPTION: qc_attr_id = 15;
pub const QC_HARDLIMIT_CONSUMPTION: qc_attr_id = 15;
pub const QC_HAS_MULTIPLE_CPU_TYPES: qc_attr_id = 16;
pub const QC_IFL_ABSOLUTE_CAPPING: qc_attr_id = 17;
pub const QC_IFL_CAPACITY_CAP: qc_attr_id = 18;
pub const QC_IFL_CAPPED_CAPACITY: qc_attr_id = 19;
pub const QC_IFL_DISPATCH_LIMITHARD: qc_attr_id = 20;
pub const QC_IFL_DISPATCH_TYPE: qc_attr_id = 21;
pub const QC_IFL_LIMITHARD_CAP: qc_attr_id = 22;
pub const QC_IFL_WEIGHT_CAPPING: qc_attr_id = 23;
pub const QC_ZIIP_ABSOLUTE_CAPPING: qc_attr_id = 66;
pub const QC_ZIIP_CAPACITY_CAP: qc_attr_id = 67;
pub const QC_ZIIP_CAPPED_CAPACITY: qc_attr_id = 68;
pub const QC_ZIIP_DISPATCH_LIMITHARD: qc_attr_id = 69;
pub const QC_ZIIP_DISPATCH_TYPE: qc_attr_id = 70;
pub const QC_ZIIP_LIMITHARD_CAP: qc_attr_id = 71;
pub const QC_ZIIP_WEIGHT_CAPPING: qc_attr_id = 72;
pub const QC_LAYER_CATEGORY: qc_attr_id = 24;
pub const QC_LAYER_CATEGORY_NUM: qc_attr_id = 25;
pub const QC_LAYER_EXTENDED_NAME: qc_attr_id = 26;
pub const QC_LAYER_NAME: qc_attr_id = 27;
pub const QC_LAYER_TYPE: qc_attr_id = 28;
pub const QC_LAYER_TYPE_NUM: qc_attr_id = 29;
pub const QC_LAYER_UUID: qc_attr_id = 30;
pub const QC_MANUFACTURER: qc_attr_id = 31;
pub const QC_MOBILITY_ENABLED: qc_attr_id = 32;
pub const QC_MOBILITY_ELIGIBLE: qc_attr_id = 32;
pub const QC_HAS_SECURE: qc_attr_id = 77;
pub const QC_SECURE: qc_attr_id = 78;
pub const QC_MODEL: qc_attr_id = 33;
pub const QC_MODEL_CAPACITY: qc_attr_id = 34;
pub const QC_TYPE_FAMILY: qc_attr_id = 65;
pub const QC_NUM_CP_DEDICATED: qc_attr_id = 35;
pub const QC_NUM_CP_SHARED: qc_attr_id = 36;
pub const QC_NUM_CP_TOTAL: qc_attr_id = 37;
pub const QC_NUM_CPU_CONFIGURED: qc_attr_id = 38;
pub const QC_NUM_CPU_DEDICATED: qc_attr_id = 39;
pub const QC_NUM_CPU_RESERVED: qc_attr_id = 40;
pub const QC_NUM_CPU_SHARED: qc_attr_id = 41;
pub const QC_NUM_CPU_STANDBY: qc_attr_id = 42;
pub const QC_NUM_CPU_TOTAL: qc_attr_id = 43;
pub const QC_NUM_IFL_DEDICATED: qc_attr_id = 44;
pub const QC_NUM_IFL_SHARED: qc_attr_id = 45;
pub const QC_NUM_IFL_TOTAL: qc_attr_id = 46;
pub const QC_NUM_ZIIP_DEDICATED: qc_attr_id = 73;
pub const QC_NUM_ZIIP_SHARED: qc_attr_id = 74;
pub const QC_NUM_ZIIP_TOTAL: qc_attr_id = 75;
pub const QC_PARTITION_CHAR: qc_attr_id = 47;
pub const QC_PARTITION_CHAR_NUM: qc_attr_id = 48;
pub const QC_PARTITION_NUMBER: qc_attr_id = 49;
pub const QC_PLANT: qc_attr_id = 50;
pub const QC_SECONDARY_CAPABILITY: qc_attr_id = 51;
pub const QC_SEQUENCE_CODE: qc_attr_id = 52;
pub const QC_TYPE: qc_attr_id = 53;
pub const QC_PRORATED_CORE_TIME: qc_attr_id = 54;
pub const QC_NUM_CP_THREADS: qc_attr_id = 55;
pub const QC_NUM_IFL_THREADS: qc_attr_id = 56;
pub const QC_NUM_ZIIP_THREADS: qc_attr_id = 76;
pub const QC_NUM_CORE_TOTAL: qc_attr_id = 57;
pub const QC_NUM_CORE_CONFIGURED: qc_attr_id = 58;
pub const QC_NUM_CORE_STANDBY: qc_attr_id = 59;
pub const QC_NUM_CORE_RESERVED: qc_attr_id = 60;
pub const QC_NUM_CORE_DEDICATED: qc_attr_id = 61;
pub const QC_NUM_CORE_SHARED: qc_attr_id = 62;
pub const QC_TYPE_NAME: qc_attr_id = 63;
pub const QC_LIC_IDENTIFIER: qc_attr_id = 64;

extern "C" {
    pub fn qc_open(rc: *mut ::std::os::raw::c_int) -> *mut ::std::os::raw::c_void;
    pub fn qc_close(hdl: *mut ::std::os::raw::c_void);

    pub fn qc_get_num_layers(
        hdl: *mut ::std::os::raw::c_void,
        rc: *mut ::std::os::raw::c_int,
    ) -> ::std::os::raw::c_int;

    pub fn qc_get_attribute_string(
        hdl: *mut ::std::os::raw::c_void,
        id: qc_attr_id,
        layer: ::std::os::raw::c_int,
        value: *mut *const ::std::os::raw::c_char,
    ) -> ::std::os::raw::c_int;
    pub fn qc_get_attribute_int(
        hdl: *mut ::std::os::raw::c_void,
        id: qc_attr_id,
        layer: ::std::os::raw::c_int,
        value: *mut ::std::os::raw::c_int,
    ) -> ::std::os::raw::c_int;
    pub fn qc_get_attribute_float(
        hdl: *mut ::std::os::raw::c_void,
        id: qc_attr_id,
        layer: ::std::os::raw::c_int,
        value: *mut ::std::os::raw::c_float,
    ) -> ::std::os::raw::c_int;
}
