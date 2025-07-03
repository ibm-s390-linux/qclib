use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a z/VM guest.
pub struct ZvmGuest<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl ZvmGuest<'_> {
    impl_attr_string!(capping: Capping);
    impl_attr_int!(capping_num: CappingNum);
    impl_attr_int!(mobility_enabled: MobilityEnabled);
    impl_attr_int!(has_secure: HasSecure);
    impl_attr_int!(secure: Secure);
    impl_attr_int!(num_cpu_total: NumCpuTotal);
    impl_attr_int!(num_cpu_configured: NumCpuConfigured);
    impl_attr_int!(num_cpu_standby: NumCpuStandby);
    impl_attr_int!(num_cpu_reserved: NumCpuReserved);
    impl_attr_int!(num_cpu_dedicated: NumCpuDedicated);
    impl_attr_int!(num_cpu_shared: NumCpuShared);
    impl_attr_int!(num_cp_total: NumCpTotal);
    impl_attr_int!(num_cp_dedicated: NumCpDedicated);
    impl_attr_int!(num_cp_shared: NumCpShared);
    impl_attr_int!(num_ifl_total: NumIflTotal);
    impl_attr_int!(num_ifl_dedicated: NumIflDedicated);
    impl_attr_int!(num_ifl_shared: NumIflShared);
    impl_attr_int!(num_ziip_total: NumZiipTotal);
    impl_attr_int!(num_ziip_dedicated: NumZiipDedicated);
    impl_attr_int!(num_ziip_shared: NumZiipShared);
    impl_attr_int!(has_multiple_cpu_types: HasMultipleCpuTypes);
    impl_attr_int!(cp_dispatch_limithard: CpDispatchLimithard);
    impl_attr_int!(cp_dispatch_type: CpDispatchType);
    impl_attr_int!(cp_capped_capacity: CpCappedCapacity);
    impl_attr_int!(ifl_dispatch_limithard: IflDispatchLimithard);
    impl_attr_int!(ifl_dispatch_type: IflDispatchType);
    impl_attr_int!(ifl_capped_capacity: IflCappedCapacity);
    impl_attr_int!(ziip_dispatch_limithard: ZiipDispatchLimithard);
    impl_attr_int!(ziip_dispatch_type: ZiipDispatchType);
    impl_attr_int!(ziip_capped_capacity: ZiipCappedCapacity);
}

impl ZvmGuest<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; Capping => string?: self.capping()?);
        json_pair!(map; CappingNum => int?: self.capping_num()?);
        json_pair!(map; MobilityEnabled => int?: self.mobility_enabled()?);
        json_pair!(map; HasSecure => int?: self.has_secure()?);
        json_pair!(map; Secure => int?: self.secure()?);
        json_pair!(map; NumCpuTotal => int?: self.num_cpu_total()?);
        json_pair!(map; NumCpuConfigured => int?: self.num_cpu_configured()?);
        json_pair!(map; NumCpuStandby => int?: self.num_cpu_standby()?);
        json_pair!(map; NumCpuReserved => int?: self.num_cpu_reserved()?);
        json_pair!(map; NumCpuDedicated => int?: self.num_cpu_dedicated()?);
        json_pair!(map; NumCpuShared => int?: self.num_cpu_shared()?);
        json_pair!(map; NumCpTotal => int?: self.num_cp_total()?);
        json_pair!(map; NumCpDedicated => int?: self.num_cp_dedicated()?);
        json_pair!(map; NumCpShared => int?: self.num_cp_shared()?);
        json_pair!(map; NumIflTotal => int?: self.num_ifl_total()?);
        json_pair!(map; NumIflDedicated => int?: self.num_ifl_dedicated()?);
        json_pair!(map; NumIflShared => int?: self.num_ifl_shared()?);
        json_pair!(map; NumZiipTotal => int?: self.num_ziip_total()?);
        json_pair!(map; NumZiipDedicated => int?: self.num_ziip_dedicated()?);
        json_pair!(map; NumZiipShared => int?: self.num_ziip_shared()?);
        json_pair!(map; HasMultipleCpuTypes => int?: self.has_multiple_cpu_types()?);
        json_pair!(map; CpDispatchLimithard => int?: self.cp_dispatch_limithard()?);
        json_pair!(map; CpCappedCapacity => int?: self.cp_capped_capacity()?);
        json_pair!(map; IflDispatchLimithard => int?: self.ifl_dispatch_limithard()?);
        json_pair!(map; IflCappedCapacity => int?: self.ifl_capped_capacity()?);
        json_pair!(map; ZiipDispatchLimithard => int?: self.ziip_dispatch_limithard()?);
        json_pair!(map; ZiipCappedCapacity => int?: self.ziip_capped_capacity()?);
        json_pair!(map; CpDispatchType => int?: self.cp_dispatch_type()?);
        json_pair!(map; IflDispatchType => int?: self.ifl_dispatch_type()?);
        json_pair!(map; ZiipDispatchType => int?: self.ziip_dispatch_type()?);
        Ok(())
    }
}
