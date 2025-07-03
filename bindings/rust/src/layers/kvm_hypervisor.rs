use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a KVM hypervisor.
pub struct KvmHypervisor<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl KvmHypervisor<'_> {
    impl_attr!(control_program_id() -> String = ControlProgramId);
    impl_attr!(adjustment() -> i32 = Adjustment);
    impl_attr!(num_core_total() -> i32 = NumCoreTotal);
    impl_attr!(num_core_dedicated() -> i32 = NumCoreDedicated);
    impl_attr!(num_core_shared() -> i32 = NumCoreShared);
    impl_attr!(num_cp_total() -> i32 = NumCpTotal);
    impl_attr!(num_cp_dedicated() -> i32 = NumCpDedicated);
    impl_attr!(num_cp_shared() -> i32 = NumCpShared);
    impl_attr!(num_ifl_total() -> i32 = NumIflTotal);
    impl_attr!(num_ifl_dedicated() -> i32 = NumIflDedicated);
    impl_attr!(num_ifl_shared() -> i32 = NumIflShared);
}

impl KvmHypervisor<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- ControlProgramId: string? = self.control_program_id()?);
        json_pair!(map <- Adjustment: int? = self.adjustment()?);
        json_pair!(map <- NumCoreTotal: int? = self.num_core_total()?);
        json_pair!(map <- NumCoreDedicated: int? = self.num_core_dedicated()?);
        json_pair!(map <- NumCoreShared: int? = self.num_core_shared()?);
        json_pair!(map <- NumCpTotal: int? = self.num_cp_total()?);
        json_pair!(map <- NumCpDedicated: int? = self.num_cp_dedicated()?);
        json_pair!(map <- NumCpShared: int? = self.num_cp_shared()?);
        json_pair!(map <- NumIflTotal: int? = self.num_ifl_total()?);
        json_pair!(map <- NumIflDedicated: int? = self.num_ifl_dedicated()?);
        json_pair!(map <- NumIflShared: int? = self.num_ifl_shared()?);
        Ok(())
    }
}
