use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a KVM guest.
pub struct KvmGuest<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl KvmGuest<'_> {
    impl_attr!(extended_name() -> String = LayerExtendedName);
    impl_attr!(uuid() -> String = LayerUuid);
    impl_attr!(has_secure() -> i32 = HasSecure);
    impl_attr!(secure() -> i32 = Secure);
    impl_attr!(num_cpu_total() -> i32 = NumCoreTotal);
    impl_attr!(num_cpu_configured() -> i32 = NumCoreConfigured);
    impl_attr!(num_cpu_standby() -> i32 = NumCoreStandby);
    impl_attr!(num_cpu_reserved() -> i32 = NumCoreReserved);
    impl_attr!(num_cpu_dedicated() -> i32 = NumCoreDedicated);
    impl_attr!(num_cpu_shared() -> i32 = NumCoreShared);
    impl_attr!(num_ifl_total() -> i32 = NumIflTotal);
    impl_attr!(num_ifl_dedicated() -> i32 = NumIflDedicated);
    impl_attr!(num_ifl_shared() -> i32 = NumIflShared);
    impl_attr!(ifl_dispatch_type() -> i32 = IflDispatchType);
}

impl KvmGuest<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- LayerExtendedName: string? = self.extended_name()?);
        json_pair!(map <- LayerUuid: string? = self.uuid()?);
        json_pair!(map <- HasSecure: int? = self.has_secure()?);
        json_pair!(map <- Secure: int? = self.secure()?);
        json_pair!(map <- NumCpuTotal: int? = self.num_cpu_total()?);
        json_pair!(map <- NumCpuConfigured: int? = self.num_cpu_configured()?);
        json_pair!(map <- NumCpuStandby: int? = self.num_cpu_standby()?);
        json_pair!(map <- NumCpuReserved: int? = self.num_cpu_reserved()?);
        json_pair!(map <- NumCpuDedicated: int? = self.num_cpu_dedicated()?);
        json_pair!(map <- NumCpuShared: int? = self.num_cpu_shared()?);
        json_pair!(map <- NumIflTotal: int? = self.num_ifl_total()?);
        json_pair!(map <- NumIflDedicated: int? = self.num_ifl_dedicated()?);
        json_pair!(map <- NumIflShared: int? = self.num_ifl_shared()?);
        json_pair!(map <- IflDispatchType: int? = self.ifl_dispatch_type()?);
        Ok(())
    }
}
