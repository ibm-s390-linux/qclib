use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a logical partition (LPAR).
pub struct Lpar<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl Lpar<'_> {
    impl_attr_string!(extended_name: LayerExtendedName);
    impl_attr_string!(uuid: LayerUuid);
    impl_attr_int!(partition_number: PartitionNumber);
    impl_attr_string!(partition_char: PartitionChar);
    impl_attr_int!(partition_char_num: PartitionCharNum);
    impl_attr_int!(adjustment: Adjustment);
    impl_attr_int!(has_secure: HasSecure);
    impl_attr_int!(secure: Secure);
    impl_attr_int!(num_core_total: NumCoreTotal);
    impl_attr_int!(num_core_configured: NumCoreConfigured);
    impl_attr_int!(num_core_standby: NumCoreStandby);
    impl_attr_int!(num_core_reserved: NumCoreReserved);
    impl_attr_int!(num_core_dedicated: NumCoreDedicated);
    impl_attr_int!(num_core_shared: NumCoreShared);
    impl_attr_int!(num_cp_total: NumCpTotal);
    impl_attr_int!(num_cp_dedicated: NumCpDedicated);
    impl_attr_int!(num_cp_shared: NumCpShared);
    impl_attr_int!(num_ifl_total: NumIflTotal);
    impl_attr_int!(num_ifl_dedicated: NumIflDedicated);
    impl_attr_int!(num_ifl_shared: NumIflShared);
    impl_attr_int!(num_ziip_total: NumZiipTotal);
    impl_attr_int!(num_ziip_dedicated: NumZiipDedicated);
    impl_attr_int!(num_ziip_shared: NumZiipShared);
    impl_attr_int!(num_cp_threads: NumCpThreads);
    impl_attr_int!(num_ifl_threads: NumIflThreads);
    impl_attr_int!(num_ziip_threads: NumZiipThreads);
    impl_attr_int!(cp_absolute_capping: CpAbsoluteCapping);
    impl_attr_int!(cp_weight_capping: CpWeightCapping);
    impl_attr_int!(ifl_absolute_capping: IflAbsoluteCapping);
    impl_attr_int!(ifl_weight_capping: IflWeightCapping);
    impl_attr_int!(ziip_absolute_capping: ZiipAbsoluteCapping);
    impl_attr_int!(ziip_weight_capping: ZiipWeightCapping);
}

impl Lpar<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; LayerExtendedName => string?: self.extended_name()?);
        json_pair!(map; LayerUuid => string?: self.uuid()?);
        json_pair!(map; Adjustment => int?: self.adjustment()?);
        json_pair!(map; PartitionNumber => int?: self.partition_number()?);
        json_pair!(map; PartitionChar => string?: self.partition_char()?);
        json_pair!(map; PartitionCharNum => int?: self.partition_char_num()?);
        json_pair!(map; HasSecure => int?: self.has_secure()?);
        json_pair!(map; Secure => int?: self.secure()?);
        json_pair!(map; NumCoreTotal => int?: self.num_core_total()?);
        json_pair!(map; NumCoreConfigured => int?: self.num_core_configured()?);
        json_pair!(map; NumCoreStandby => int?: self.num_core_standby()?);
        json_pair!(map; NumCoreReserved => int?: self.num_core_reserved()?);
        json_pair!(map; NumCoreDedicated => int?: self.num_core_dedicated()?);
        json_pair!(map; NumCoreShared => int?: self.num_core_shared()?);
        json_pair!(map; NumCpTotal => int?: self.num_cp_total()?);
        json_pair!(map; NumCpDedicated => int?: self.num_cp_dedicated()?);
        json_pair!(map; NumCpShared => int?: self.num_cp_shared()?);
        json_pair!(map; NumIflTotal => int?: self.num_ifl_total()?);
        json_pair!(map; NumIflDedicated => int?: self.num_ifl_dedicated()?);
        json_pair!(map; NumIflShared => int?: self.num_ifl_shared()?);
        json_pair!(map; NumZiipTotal => int?: self.num_ziip_total()?);
        json_pair!(map; NumZiipDedicated => int?: self.num_ziip_dedicated()?);
        json_pair!(map; NumZiipShared => int?: self.num_ziip_shared()?);
        json_pair!(map; NumCpThreads => int?: self.num_cp_threads()?);
        json_pair!(map; NumIflThreads => int?: self.num_ifl_threads()?);
        json_pair!(map; NumZiipThreads => int?: self.num_ziip_threads()?);
        json_pair!(map; CpAbsoluteCapping => int?: self.cp_absolute_capping()?);
        json_pair!(map; IflAbsoluteCapping => int?: self.ifl_absolute_capping()?);
        json_pair!(map; ZiipAbsoluteCapping => int?: self.ziip_absolute_capping()?);
        json_pair!(map; CpWeightCapping => int?: self.cp_weight_capping()?);
        json_pair!(map; IflWeightCapping => int?: self.ifl_weight_capping()?);
        json_pair!(map; ZiipWeightCapping => int?: self.ziip_weight_capping()?);
        Ok(())
    }
}
