use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a z/VM guest.
pub struct ZvmGuest<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl ZvmGuest<'_> {
    impl_attr!(capping() -> String = Capping);
    impl_attr!(capping_num() -> i32 = CappingNum);
    impl_attr!(mobility_enabled() -> i32 = MobilityEnabled);
    impl_attr!(has_secure() -> i32 = HasSecure);
    impl_attr!(secure() -> i32 = Secure);
    impl_attr!(num_cpu_total() -> i32 = NumCpuTotal);
    impl_attr!(num_cpu_configured() -> i32 = NumCpuConfigured);
    impl_attr!(num_cpu_standby() -> i32 = NumCpuStandby);
    impl_attr!(num_cpu_reserved() -> i32 = NumCpuReserved);
    impl_attr!(num_cpu_dedicated() -> i32 = NumCpuDedicated);
    impl_attr!(num_cpu_shared() -> i32 = NumCpuShared);
    impl_attr!(num_cp_total() -> i32 = NumCpTotal);
    impl_attr!(num_cp_dedicated() -> i32 = NumCpDedicated);
    impl_attr!(num_cp_shared() -> i32 = NumCpShared);
    impl_attr!(num_ifl_total() -> i32 = NumIflTotal);
    impl_attr!(num_ifl_dedicated() -> i32 = NumIflDedicated);
    impl_attr!(num_ifl_shared() -> i32 = NumIflShared);
    impl_attr!(num_ziip_total() -> i32 = NumZiipTotal);
    impl_attr!(num_ziip_dedicated() -> i32 = NumZiipDedicated);
    impl_attr!(num_ziip_shared() -> i32 = NumZiipShared);
    impl_attr!(has_multiple_cpu_types() -> i32 = HasMultipleCpuTypes);
    impl_attr!(cp_dispatch_limithard() -> i32 = CpDispatchLimithard);
    impl_attr!(cp_dispatch_type() -> i32 = CpDispatchType);
    impl_attr!(cp_capped_capacity() -> i32 = CpCappedCapacity);
    impl_attr!(ifl_dispatch_limithard() -> i32 = IflDispatchLimithard);
    impl_attr!(ifl_dispatch_type() -> i32 = IflDispatchType);
    impl_attr!(ifl_capped_capacity() -> i32 = IflCappedCapacity);
    impl_attr!(ziip_dispatch_limithard() -> i32 = ZiipDispatchLimithard);
    impl_attr!(ziip_dispatch_type() -> i32 = ZiipDispatchType);
    impl_attr!(ziip_capped_capacity() -> i32 = ZiipCappedCapacity);
}

impl ZvmGuest<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- Capping: string? = self.capping()?);
        json_pair!(map <- CappingNum: int? = self.capping_num()?);
        json_pair!(map <- MobilityEnabled: int? = self.mobility_enabled()?);
        json_pair!(map <- HasSecure: int? = self.has_secure()?);
        json_pair!(map <- Secure: int? = self.secure()?);
        json_pair!(map <- NumCpuTotal: int? = self.num_cpu_total()?);
        json_pair!(map <- NumCpuConfigured: int? = self.num_cpu_configured()?);
        json_pair!(map <- NumCpuStandby: int? = self.num_cpu_standby()?);
        json_pair!(map <- NumCpuReserved: int? = self.num_cpu_reserved()?);
        json_pair!(map <- NumCpuDedicated: int? = self.num_cpu_dedicated()?);
        json_pair!(map <- NumCpuShared: int? = self.num_cpu_shared()?);
        json_pair!(map <- NumCpTotal: int? = self.num_cp_total()?);
        json_pair!(map <- NumCpDedicated: int? = self.num_cp_dedicated()?);
        json_pair!(map <- NumCpShared: int? = self.num_cp_shared()?);
        json_pair!(map <- NumIflTotal: int? = self.num_ifl_total()?);
        json_pair!(map <- NumIflDedicated: int? = self.num_ifl_dedicated()?);
        json_pair!(map <- NumIflShared: int? = self.num_ifl_shared()?);
        json_pair!(map <- NumZiipTotal: int? = self.num_ziip_total()?);
        json_pair!(map <- NumZiipDedicated: int? = self.num_ziip_dedicated()?);
        json_pair!(map <- NumZiipShared: int? = self.num_ziip_shared()?);
        json_pair!(map <- HasMultipleCpuTypes: int? = self.has_multiple_cpu_types()?);
        json_pair!(map <- CpDispatchLimithard: int? = self.cp_dispatch_limithard()?);
        json_pair!(map <- CpCappedCapacity: int? = self.cp_capped_capacity()?);
        json_pair!(map <- IflDispatchLimithard: int? = self.ifl_dispatch_limithard()?);
        json_pair!(map <- IflCappedCapacity: int? = self.ifl_capped_capacity()?);
        json_pair!(map <- ZiipDispatchLimithard: int? = self.ziip_dispatch_limithard()?);
        json_pair!(map <- ZiipCappedCapacity: int? = self.ziip_capped_capacity()?);
        json_pair!(map <- CpDispatchType: int? = self.cp_dispatch_type()?);
        json_pair!(map <- IflDispatchType: int? = self.ifl_dispatch_type()?);
        json_pair!(map <- ZiipDispatchType: int? = self.ziip_dispatch_type()?);
        Ok(())
    }
}
