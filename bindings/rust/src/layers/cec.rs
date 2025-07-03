use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents the CEC (lowest layer, i.e. physical machine).
pub struct CEC<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl CEC<'_> {
    impl_attr_string!(manufacturer: Manufacturer);
    impl_attr_string!(machine_type: Type);
    impl_attr_string!(type_name: TypeName);
    impl_attr_int!(type_family: TypeFamily);
    impl_attr_string!(model_capacity: ModelCapacity);
    impl_attr_string!(model: Model);
    impl_attr_string!(sequence_code: SequenceCode);
    impl_attr_string!(lic_identifier: LicIdentifier);
    impl_attr_string!(plant: Plant);
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
    impl_attr_float!(capability: Capability);
    impl_attr_float!(secondary_capability: SecondaryCapability);
    impl_attr_int!(capacity_adjustment_indication: CapacityAdjustmentIndication);
    impl_attr_int!(capacity_change_reason: CapacityChangeReason);
}

impl CEC<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; Manufacturer => string?: self.manufacturer()?);
        json_pair!(map; Type => string?: self.machine_type()?);
        json_pair!(map; TypeName => string?: self.type_name()?);
        json_pair!(map; TypeFamily => int?: self.type_family()?);
        json_pair!(map; ModelCapacity => string?: self.model_capacity()?);
        json_pair!(map; Model => string?: self.model()?);
        json_pair!(map; SequenceCode => string?: self.sequence_code()?);
        json_pair!(map; LicIdentifier => string?: self.lic_identifier()?);
        json_pair!(map; Plant => string?: self.plant()?);
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
        json_pair!(map; Capability => float?: self.capability()?);
        json_pair!(map; SecondaryCapability => float?: self.secondary_capability()?);
        json_pair!(map; CapacityAdjustmentIndication => int?: self.capacity_adjustment_indication()?);
        json_pair!(map; CapacityChangeReason => int?: self.capacity_change_reason()?);
        Ok(())
    }
}
