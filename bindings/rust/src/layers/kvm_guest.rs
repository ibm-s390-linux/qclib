use serde_json::{Map, Value};

use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a KVM guest.
pub struct KvmGuest<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl KvmGuest<'_> {
    impl_attr_string!(extended_name: LayerExtendedName);
    impl_attr_string!(uuid: LayerUuid);
    impl_attr_int!(has_secure: HasSecure);
    impl_attr_int!(secure: Secure);
    impl_attr_int!(num_cpu_total: NumCoreTotal);
    impl_attr_int!(num_cpu_configured: NumCoreConfigured);
    impl_attr_int!(num_cpu_standby: NumCoreStandby);
    impl_attr_int!(num_cpu_reserved: NumCoreReserved);
    impl_attr_int!(num_cpu_dedicated: NumCoreDedicated);
    impl_attr_int!(num_cpu_shared: NumCoreShared);
    impl_attr_int!(num_ifl_total: NumIflTotal);
    impl_attr_int!(num_ifl_dedicated: NumIflDedicated);
    impl_attr_int!(num_ifl_shared: NumIflShared);
    impl_attr_int!(ifl_dispatch_type: IflDispatchType);
}

impl KvmGuest<'_> {
    pub(crate) fn update_json(&self, map: &mut Map<String, Value>) -> Result<()> {
        json_pair!(map; LayerExtendedName => string?: self.extended_name()?);
        json_pair!(map; LayerUuid => string?: self.uuid()?);
        json_pair!(map; HasSecure => int?: self.has_secure()?);
        json_pair!(map; Secure => int?: self.secure()?);
        json_pair!(map; NumCpuTotal => int?: self.num_cpu_total()?);
        json_pair!(map; NumCpuConfigured => int?: self.num_cpu_configured()?);
        json_pair!(map; NumCpuStandby => int?: self.num_cpu_standby()?);
        json_pair!(map; NumCpuReserved => int?: self.num_cpu_reserved()?);
        json_pair!(map; NumCpuDedicated => int?: self.num_cpu_dedicated()?);
        json_pair!(map; NumCpuShared => int?: self.num_cpu_shared()?);
        json_pair!(map; NumIflTotal => int?: self.num_ifl_total()?);
        json_pair!(map; NumIflDedicated => int?: self.num_ifl_dedicated()?);
        json_pair!(map; NumIflShared => int?: self.num_ifl_shared()?);
        json_pair!(map; IflDispatchType => int?: self.ifl_dispatch_type()?);
        Ok(())
    }
}
