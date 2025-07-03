use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a z/VM hypervisor.
pub struct ZvmHypervisor<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl ZvmHypervisor<'_> {
    impl_attr_string!(cluster_name: ClusterName);
    impl_attr_string!(control_program_id: ControlProgramId);
    impl_attr_int!(adjustment: Adjustment);
    impl_attr_int!(limithard_consumption: LimithardConsumption);
    impl_attr_int!(prorated_core_time: ProratedCoreTime);
    impl_attr_int!(num_core_total: NumCoreTotal);
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
}

impl ZvmHypervisor<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; ClusterName => string?: self.cluster_name()?);
        json_pair!(map; ControlProgramId => string?: self.control_program_id()?);
        json_pair!(map; Adjustment => int?: self.adjustment()?);
        json_pair!(map; LimithardConsumption => int?: self.limithard_consumption()?);
        json_pair!(map; ProratedCoreTime => int?: self.prorated_core_time()?);
        json_pair!(map; NumCoreTotal => int?: self.num_core_total()?);
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
        Ok(())
    }
}
