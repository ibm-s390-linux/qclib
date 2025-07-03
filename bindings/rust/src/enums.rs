#![allow(clippy::upper_case_acronyms)]
use sys::*;

macro_rules! enum_def {
    (
        $(#[$attr:meta])*
        $vis:vis enum $name:ident: $T:ty {
            $(
                $(#[doc = $doc:literal])*
                #[display($display:literal)]
                $item:ident = $value:expr
            ),+ $(,)?
        }
    ) => {
        $(#[$attr])*
        $vis enum $name {
            $(
                $(#[doc = $doc])*
                $item,
            )+
        }

        impl $name {
            #[allow(dead_code)]
            #[inline]
            pub(crate) fn from_raw(raw: $T) -> Self {
                $(if raw == $value {
                    Self::$item
                } else )+ {
                    panic!("invalid variant");
                }
            }

            #[allow(dead_code)]
            #[inline]
            pub(crate) fn as_raw(&self) -> $T {
                match *self {
                    $(Self::$item => $value,)+
                }
            }
        }

        impl ::std::fmt::Display for $name {
            fn fmt(&self, f: &mut ::std::fmt::Formatter<'_>) -> ::std::fmt::Result {
                let s = match *self {
                    $(Self::$item => $display,)+
                };
                s.fmt(f)
            }
        }
    };
}

enum_def! {
    #[derive(Clone, Copy, Debug, PartialEq, Eq)]
    pub enum LayerType: qc_layer_type {
        /// CEC
        #[display("CEC")]
        CEC = QC_LAYER_TYPE_CEC,

        /// LPAR Capping Group
        #[display("LPAR-GROUP")]
        LparGroup = QC_LAYER_TYPE_LPAR_GROUP,

        /// LPAR
        #[display("LPAR")]
        Lpar = QC_LAYER_TYPE_LPAR,

        /// z/VM Hypervisor
        #[display("z/VM-hypervisor")]
        ZvmHypervisor = QC_LAYER_TYPE_ZVM_HYPERVISOR,

        /// z/VM CPU Pool (deprecated, use QC_LAYER_TYPE_ZVM_RESOURCE_POOL instead)
        #[display("z/VM-CPU-pool")]
        ZvmCpuPool = QC_LAYER_TYPE_ZVM_CPU_POOL,

        /// z/VM Resource Pool
        #[display("z/VM-resource-pool")]
        ZvmResourcePool = QC_LAYER_TYPE_ZVM_RESOURCE_POOL,

        /// z/VM Guest
        #[display("z/VM-guest")]
        ZvmGuest = QC_LAYER_TYPE_ZVM_GUEST,

        /// KVM Hypervisor
        #[display("KVM-hypervisor")]
        KvmHypervisor = QC_LAYER_TYPE_KVM_HYPERVISOR,

        /// KVM Guest
        #[display("KVM-guest")]
        KvmGuest = QC_LAYER_TYPE_KVM_GUEST,

        /// z/OS Hypervisor
        #[display("z/OS-hypervisor")]
        ZosHypervisor = QC_LAYER_TYPE_ZOS_HYPERVISOR,

        /// z/OS Tenant Resource Group
        #[display("z/OS-tenant-resource-group")]
        ZosTenantResourceGroup = QC_LAYER_TYPE_ZOS_TENANT_RESOURCE_GROUP,

        /// z/OS cCX Server
        #[display("z/OS-zCX-Server")]
        ZosZcxServer = QC_LAYER_TYPE_ZOS_ZCX_SERVER,
    }
}

enum_def! {
    #[derive(Clone, Copy, Debug, PartialEq, Eq)]
    pub enum LayerCategory: qc_layer_category {
        /// Layer category for guests, namely LPARs, z/VM and KVM guests.
        #[display("GUEST")]
        Guest = QC_LAYER_CAT_GUEST,

        /// Layer category for hosts, namely CEC, z/VM and KVM hosts.
        #[display("HOST")]
        Host = QC_LAYER_CAT_HOST,

        /// Layer category for pools (currently z/VM Pools and LPAR capping groups).
        #[display("POOL")]
        Pool = QC_LAYER_CAT_POOL,
    }
}

enum_def! {
    #[derive(Clone, Copy, Debug)]
    pub enum AttributeId: qc_attr_id {
        /// The adjustment factor indicates the maximum percentage of the machine (in parts of
        /// 1000) that could be used by the primary processor type in the worst case by the
        /// respective layer; taking cappings and other limiting factors into consideration.
        /// Note: This value can lead to wrong conclusions for layers that utilize more than one
        /// processor type!
        #[display("adjustment")]
        Adjustment = QC_ADJUSTMENT,

        /// Capability rating, see STSI instruction in [2].
        #[display("capability")]
        Capability = QC_CAPABILITY,

        /// Capacity adjustment value, see STSI instruction in [2].
        #[display("capacity_adjustment_indication")]
        CapacityAdjustmentIndication = QC_CAPACITY_ADJUSTMENT_INDICATION,

        /// Reason for capacity adjustment, see STSI instruction in [2].
        #[display("capacity_change_reason")]
        CapacityChangeReason = QC_CAPACITY_CHANGE_REASON,

        /// Capping type: "off", "soft", "hard".
        #[display("capping")]
        Capping = QC_CAPPING,

        /// Numeric representation of capping type, see enum #qc_cappings.
        #[display("capping_num")]
        CappingNum = QC_CAPPING_NUM,

        /// SSI name if part of SSI cluster.
        #[display("cluster_name")]
        ClusterName = QC_CLUSTER_NAME,

        /// ID of control program.
        #[display("control_program_id")]
        ControlProgramId = QC_CONTROL_PROGRAM_ID,

        /// CP absolute capping value. Scaled value where 0x10000 equals to one core, or 0 if no
        /// capping set.
        #[display("absolute_capping")]
        CpAbsoluteCapping = QC_CP_ABSOLUTE_CAPPING,

        /// 1 if pool's CP virtual type has capped capacity 0 if not. See DEFINE CPUPOOL
        /// command in [3].
        #[display("cp_capacity_cap")]
        CpCapacityCap = QC_CP_CAPACITY_CAP,

        /// Guest current capped capacity for shared virtual CPs -- scaled value where 0x10000
        /// equals to one core; or 0 if no capping set. While this field displays the capacity,
        /// either #qc_cp_capacity_cap or #qc_cp_limithard_cap must is set to indicate the kind of
        /// limit.
        #[display("cp_capped_capacity")]
        CpCappedCapacity = QC_CP_CAPPED_CAPACITY,

        /// 1 if guest CP dispatch type has LIMITHARD capping, 0 if not. See SET SRM command
        /// in [3].
        #[display("cp_dispatch_limithard")]
        CpDispatchLimithard = QC_CP_DISPATCH_LIMITHARD,

        /// Dispatch type for guest CPs: 0=General Purpose (CP).
        #[display("cp_dispatch_type")]
        CpDispatchType = QC_CP_DISPATCH_TYPE,

        /// 1 if pool's CP virtual type has limithard capping 0 if not. See DEFINE CPUPOOL
        /// command in [3].
        #[display("cp_limithard_cap")]
        CpLimithardCap = QC_CP_LIMITHARD_CAP,

        /// CP weight-based capping value -- scaled value where 0x10000 equals to one core, or 0 if
        /// no capping set.
        #[display("cp_weight_capping")]
        CpWeightCapping = QC_CP_WEIGHT_CAPPING,

        /// 1 if SRM limithard setting is consumption 0 if deadline. See SET SRM command in
        /// [3].
        #[display("limithard_consumption")]
        LimithardConsumption = QC_LIMITHARD_CONSUMPTION,

        /// Deprecated, see #qc_limithard_consumption.
        #[display("hardlimit_consumption")]
        HardlimitConsumption = QC_HARDLIMIT_CONSUMPTION,

        /// 1 if layer has multiple CPU types (e.g. CPs, IFLs, zIIPs), 0 if not.
        #[display("has_multiple_cpu_types")]
        HasMultipleCpuTypes = QC_HAS_MULTIPLE_CPU_TYPES,

        /// IFL absolute capping value -- scaled value where 0x10000 equals to one core, or 0 if no
        /// capping set.
        #[display("ifl_absolute_capping")]
        IflAbsoluteCapping = QC_IFL_ABSOLUTE_CAPPING,

        /// 1 if pool's IFL virtual type has capped capacity 0 if not. See DEFINE CPUPOOL
        /// command in [3].
        #[display("ifl_capacity_cap")]
        IflCapacityCap = QC_IFL_CAPACITY_CAP,

        /// Guest current capped capacity for shared virtual IFLs -- scaled value where 0x10000
        /// equals to one core; or 0 if no capping set. While this field displays the capacity,
        /// either #qc_ifl_capacity_cap or #qc_ifl_limithard_cap must is set to indicate the kind
        /// of limit.
        #[display("ifl_capped_capacity")]
        IflCappedCapacity = QC_IFL_CAPPED_CAPACITY,

        /// 1 if guest IFL dispatch type has LIMITHARD capping, 0 if not. See SET SRM command
        /// in [3].
        #[display("ifl_dispatch_limithard")]
        IflDispatchLimithard = QC_IFL_DISPATCH_LIMITHARD,

        /// Dispatch type for guest IFLs: 0=General Purpose (CP), 3=Integrated Facility for Linux
        /// (IFL).
        #[display("ifl_dispatch_type")]
        IflDispatchType = QC_IFL_DISPATCH_TYPE,

        /// 1 if pool's IFL virtual type has limithard capping 0 if not. See DEFINE CPUPOOL
        /// command in [3].
        #[display("ifl_limithard_cap")]
        IflLimithardCap = QC_IFL_LIMITHARD_CAP,

        /// IFL weight-based capping value -- scaled value where 0x10000 equals to one core, or 0
        /// if no capping set.
        #[display("ifl_weight_capping")]
        IflWeightCapping = QC_IFL_WEIGHT_CAPPING,

        /// zIIP absolute capping value -- scaled value where 0x10000 equals to one core, or 0 if
        /// no capping set.
        #[display("ziip_absolute_capping")]
        ZiipAbsoluteCapping = QC_ZIIP_ABSOLUTE_CAPPING,

        /// 1 if pool's zIIP virtual type has capped capacity 0 if not. See DEFINE CPUPOOL
        /// command in [3].
        #[display("ziip_capacity_cap")]
        ZiipCapacityCap = QC_ZIIP_CAPACITY_CAP,

        /// Guest current capped capacity for shared virtual zIIPs -- scaled value where 0x10000
        /// equals to one core; or 0 if no capping set. While this field displays the capacity,
        /// either #qc_ziip_capacity_cap or #qc_ziip_limithard_cap must is set to indicate the kind
        /// of limit.
        #[display("ziip_capped_capacity")]
        ZiipCappedCapacity = QC_ZIIP_CAPPED_CAPACITY,

        /// 1 if guest zIIP dispatch type has LIMITHARD capping, 0 if not. See SET SRM
        /// command in [3].
        #[display("ziip_dispatch_limithard")]
        ZiipDispatchLimithard = QC_ZIIP_DISPATCH_LIMITHARD,

        /// Dispatch type for guest zIIPs: 0=General Purpose (CP), 5=zSeries Integrated Information
        /// Processor (zIIP), ff=zIIP or CP.
        #[display("ziip_dispatch_type")]
        ZiipDispatchType = QC_ZIIP_DISPATCH_TYPE,

        /// 1 if pool's zIIP virtual type has limithard capping 0 if not. See DEFINE CPUPOOL
        ///  command in [3].
        #[display("ziip_limithard_cap")]
        ZiipLimithardCap = QC_ZIIP_LIMITHARD_CAP,

        /// zIIP weight-based capping value -- scaled value where 0x10000 equals to one core, or 0
        /// if no capping set.
        #[display("ziip_weight_capping")]
        ZiipWeightCapping = QC_ZIIP_WEIGHT_CAPPING,

        /// Layer category, see layer tables above for details.
        #[display("layer_category")]
        LayerCategory = QC_LAYER_CATEGORY,

        /// Numeric representation  of layer category, see enum #qc_layer_categories.
        #[display("layer_category_num")]
        LayerCategoryNum = QC_LAYER_CATEGORY_NUM,

        /// Guest extended name.
        #[display("layer_extended_name")]
        LayerExtendedName = QC_LAYER_EXTENDED_NAME,

        /// Name of container, see layer tables for details
        #[display("layer_name")]
        LayerName = QC_LAYER_NAME,

        /// Layer type, see layer tables above for details.
        #[display("layer_type")]
        LayerType = QC_LAYER_TYPE,

        /// Numeric representation  of layer type, see enum #qc_layer_types.
        #[display("layer_type_num")]
        LayerTypeNum = QC_LAYER_TYPE_NUM,

        /// Universal unique ID.
        #[display("layer_uuid")]
        LayerUuid = QC_LAYER_UUID,

        /// Company that manufactured box.
        #[display("manufacturer")]
        Manufacturer = QC_MANUFACTURER,

        /// 1 if guest is enabled for mobility, 0 if not.
        #[display("mobility_enabled")]
        MobilityEnabled = QC_MOBILITY_ENABLED,

        /// Deprecated, see #qc_mobility_enabled.
        #[display("mobility_eligible")]
        MobilityEligible = QC_MOBILITY_ELIGIBLE,

        /// Indicates whether secure boot is available to the entity. Requires Linux kernel 5.3 or
        /// later.
        /// Note: This attribute is only ever available for the topmost layer.
        #[display("has_secure")]
        HasSecure = QC_HAS_SECURE,

        /// Indicates whether entity was booted using the secure boot feature Requires Linux kernel
        /// 5.3 or later.
        /// Note: This attribute is only ever available for the topmost layer.
        #[display("secure")]
        Secure = QC_SECURE,

        /// Model identifier, see STSI instruction in [2].
        #[display("model")]
        Model = QC_MODEL,

        /// Model capacity of machine, see STSI instruction in [2].
        #[display("model_capacity")]
        ModelCapacity = QC_MODEL_CAPACITY,

        /// Family of the model, enum #qc_model_families.
        #[display("type_family")]
        TypeFamily = QC_TYPE_FAMILY,

        /// Sum of dedicated CPs in layer.
        #[display("num_cp_dedicated")]
        NumCpDedicated = QC_NUM_CP_DEDICATED,

        /// Sum of shared CPs in layer.
        #[display("num_cp_shared")]
        NumCpShared = QC_NUM_CP_SHARED,

        /// Sum of all CPs in layer.
        #[display("num_cp_total")]
        NumCpTotal = QC_NUM_CP_TOTAL,

        /// Sum of configured CPs and IFLs in layer.
        #[display("num_cpu_configured")]
        NumCpuConfigured = QC_NUM_CPU_CONFIGURED,

        /// Sum of dedicated CPs and IFLs in layer.
        #[display("num_cpu_dedicated")]
        NumCpuDedicated = QC_NUM_CPU_DEDICATED,

        /// Sum of reserved CPs and IFLs in layer.
        #[display("num_cpu_reserved")]
        NumCpuReserved = QC_NUM_CPU_RESERVED,

        /// Sum of shared CPs and IFLs in layer.
        #[display("num_cpu_shared")]
        NumCpuShared = QC_NUM_CPU_SHARED,

        /// Sum of standby CPs and IFLs in layer.
        #[display("num_cpu_standby")]
        NumCpuStandby = QC_NUM_CPU_STANDBY,

        /// Sum of all CPs and IFLs in layer.
        #[display("num_cpu_total")]
        NumCpuTotal = QC_NUM_CPU_TOTAL,

        /// Sum of dedicated IFLs in layer.
        #[display("num_ifl_dedicated")]
        NumIflDedicated = QC_NUM_IFL_DEDICATED,

        /// Sum of shared IFLs in layer.
        #[display("num_ifl_shared")]
        NumIflShared = QC_NUM_IFL_SHARED,

        /// Sum of all IFLs (Integrated Facility for Linux) in layer.
        #[display("num_ifl_total")]
        NumIflTotal = QC_NUM_IFL_TOTAL,

        /// Sum of dedicated zIIPs in layer.
        #[display("num_ziip_dedicated")]
        NumZiipDedicated = QC_NUM_ZIIP_DEDICATED,

        /// Sum of shared zIIPs in layer.
        #[display("num_ziip_shared")]
        NumZiipShared = QC_NUM_ZIIP_SHARED,

        /// Sum of all zIIPs (Integrated Information Processor) in layer.
        #[display("num_ziip_total")]
        NumZiipTotal = QC_NUM_ZIIP_TOTAL,

        /// Partition characteristics, any combination of "Dedicated", "Shared" and
        /// "Limited", also see STSI instruction in [2].
        #[display("partition_char")]
        PartitionChar = QC_PARTITION_CHAR,

        /// Numeric representation of partition characteristics, see enum #qc_part_chars.
        #[display("partition_char_num")]
        PartitionCharNum = QC_PARTITION_CHAR_NUM,

        /// Partition number, see STSI instruction in [2].
        #[display("partition_number")]
        PartitionNumber = QC_PARTITION_NUMBER,

        /// Identifier of the manufacturing plant, see STSI instruction in [2].
        #[display("plant")]
        Plant = QC_PLANT,

        /// Secondary capability rating, see STSI instruction in [2].
        #[display("secondary_capability")]
        SecondaryCapability = QC_SECONDARY_CAPABILITY,

        /// Sequence code of machine, see STSI instruction in [2].
        #[display("sequence_code")]
        SequenceCode = QC_SEQUENCE_CODE,

        /// 4-digit machine type.
        #[display("type")]
        Type = QC_TYPE,

        /// 1 if limithard caps uses prorated core time for capping 0 if raw CPU time is used. See
        /// APAR VM65680.
        #[display("prorated_core_time")]
        ProratedCoreTime = QC_PRORATED_CORE_TIME,

        /// Threads per CP, values >1 indicate that SMT is enabled.
        #[display("num_cp_threads")]
        NumCpThreads = QC_NUM_CP_THREADS,

        /// Threads per IFL, values >1 indicate that SMT is enabled.
        #[display("num_ifl_threads")]
        NumIflThreads = QC_NUM_IFL_THREADS,

        /// Threads per zIIP, values >1 indicate that SMT is enabled.
        #[display("num_ziip_threads")]
        NumZiipThreads = QC_NUM_ZIIP_THREADS,

        /// Sum of all CP and IFL cores in layer.
        #[display("num_core_total")]
        NumCoreTotal = QC_NUM_CORE_TOTAL,

        /// Sum of configure CP and IFL cores in layer.
        #[display("num_core_configured")]
        NumCoreConfigured = QC_NUM_CORE_CONFIGURED,

        /// Sum of standby CP and IFL cores in layer.
        #[display("num_core_standby")]
        NumCoreStandby = QC_NUM_CORE_STANDBY,

        /// Sum of reserved CP and IFL cores in layer.
        #[display("num_core_reserved")]
        NumCoreReserved = QC_NUM_CORE_RESERVED,

        /// Sum of dedicated CP and IFL cores in layer.
        #[display("num_core_dedicated")]
        NumCoreDedicated = QC_NUM_CORE_DEDICATED,

        /// Sum of shared CP and IFL cores in layer.
        #[display("num_core_shared")]
        NumCoreShared = QC_NUM_CORE_SHARED,

        /// Name of IBM Z model in clear text.
        #[display("type_name")]
        TypeName = QC_TYPE_NAME,

        /// Licensed Internal Code (LIC) level.
        #[display("lic_identifier")]
        LicIdentifier = QC_LIC_IDENTIFIER,
    }
}
