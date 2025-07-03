use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a z/VM hypervisor.
pub struct ZvmHypervisor<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl ZvmHypervisor<'_> {
    impl_attr!(cluster_name() -> String = ClusterName);
    impl_attr!(control_program_id() -> String = ControlProgramId);
    impl_attr!(adjustment() -> i32 = Adjustment);
    impl_attr!(limithard_consumption() -> i32 = LimithardConsumption);
    impl_attr!(prorated_core_time() -> i32 = ProratedCoreTime);
    impl_attr!(num_core_total() -> i32 = NumCoreTotal);
    impl_attr!(num_core_dedicated() -> i32 = NumCoreDedicated);
    impl_attr!(num_core_shared() -> i32 = NumCoreShared);
    impl_attr!(num_cp_total() -> i32 = NumCpTotal);
    impl_attr!(num_cp_dedicated() -> i32 = NumCpDedicated);
    impl_attr!(num_cp_shared() -> i32 = NumCpShared);
    impl_attr!(num_ifl_total() -> i32 = NumIflTotal);
    impl_attr!(num_ifl_dedicated() -> i32 = NumIflDedicated);
    impl_attr!(num_ifl_shared() -> i32 = NumIflShared);
    impl_attr!(num_ziip_total() -> i32 = NumZiipTotal);
    impl_attr!(num_ziip_dedicated() -> i32 = NumZiipDedicated);
    impl_attr!(num_ziip_shared() -> i32 = NumZiipShared);
    impl_attr!(num_cp_threads() -> i32 = NumCpThreads);
    impl_attr!(num_ifl_threads() -> i32 = NumIflThreads);
    impl_attr!(num_ziip_threads() -> i32 = NumZiipThreads);
}

impl ZvmHypervisor<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- ClusterName: string? = self.cluster_name()?);
        json_pair!(map <- ControlProgramId: string? = self.control_program_id()?);
        json_pair!(map <- Adjustment: int? = self.adjustment()?);
        json_pair!(map <- LimithardConsumption: int? = self.limithard_consumption()?);
        json_pair!(map <- ProratedCoreTime: int? = self.prorated_core_time()?);
        json_pair!(map <- NumCoreTotal: int? = self.num_core_total()?);
        json_pair!(map <- NumCoreDedicated: int? = self.num_core_dedicated()?);
        json_pair!(map <- NumCoreShared: int? = self.num_core_shared()?);
        json_pair!(map <- NumCpTotal: int? = self.num_cp_total()?);
        json_pair!(map <- NumCpDedicated: int? = self.num_cp_dedicated()?);
        json_pair!(map <- NumCpShared: int? = self.num_cp_shared()?);
        json_pair!(map <- NumIflTotal: int? = self.num_ifl_total()?);
        json_pair!(map <- NumIflDedicated: int? = self.num_ifl_dedicated()?);
        json_pair!(map <- NumIflShared: int? = self.num_ifl_shared()?);
        json_pair!(map <- NumZiipTotal: int? = self.num_ziip_total()?);
        json_pair!(map <- NumZiipDedicated: int? = self.num_ziip_dedicated()?);
        json_pair!(map <- NumZiipShared: int? = self.num_ziip_shared()?);
        json_pair!(map <- NumCpThreads: int? = self.num_cp_threads()?);
        json_pair!(map <- NumIflThreads: int? = self.num_ifl_threads()?);
        json_pair!(map <- NumZiipThreads: int? = self.num_ziip_threads()?);
        Ok(())
    }
}
