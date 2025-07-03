use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a KVM hypervisor.
pub struct KvmHypervisor<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl KvmHypervisor<'_> {
    impl_attr_string!(control_program_id: ControlProgramId);
    impl_attr_int!(adjustment: Adjustment);
    impl_attr_int!(num_core_total: NumCoreTotal);
    impl_attr_int!(num_core_dedicated: NumCoreDedicated);
    impl_attr_int!(num_core_shared: NumCoreShared);
    impl_attr_int!(num_cp_total: NumCpTotal);
    impl_attr_int!(num_cp_dedicated: NumCpDedicated);
    impl_attr_int!(num_cp_shared: NumCpShared);
    impl_attr_int!(num_ifl_total: NumIflTotal);
    impl_attr_int!(num_ifl_dedicated: NumIflDedicated);
    impl_attr_int!(num_ifl_shared: NumIflShared);
}

impl KvmHypervisor<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; ControlProgramId => string?: self.control_program_id()?);
        json_pair!(map; Adjustment => int?: self.adjustment()?);
        json_pair!(map; NumCoreTotal => int?: self.num_core_total()?);
        json_pair!(map; NumCoreDedicated => int?: self.num_core_dedicated()?);
        json_pair!(map; NumCoreShared => int?: self.num_core_shared()?);
        json_pair!(map; NumCpTotal => int?: self.num_cp_total()?);
        json_pair!(map; NumCpDedicated => int?: self.num_cp_dedicated()?);
        json_pair!(map; NumCpShared => int?: self.num_cp_shared()?);
        json_pair!(map; NumIflTotal => int?: self.num_ifl_total()?);
        json_pair!(map; NumIflDedicated => int?: self.num_ifl_dedicated()?);
        json_pair!(map; NumIflShared => int?: self.num_ifl_shared()?);
        Ok(())
    }
}
