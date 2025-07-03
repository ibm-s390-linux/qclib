use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents the CEC (lowest layer, i.e. physical machine).
pub struct CEC<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl CEC<'_> {
    impl_attr!(manufacturer() -> String = Manufacturer);
    impl_attr!(machine_type() -> String = Type);
    impl_attr!(type_name() -> String = TypeName);
    impl_attr!(type_family() -> i32 = TypeFamily);
    impl_attr!(model_capacity() -> String = ModelCapacity);
    impl_attr!(model() -> String = Model);
    impl_attr!(sequence_code() -> String = SequenceCode);
    impl_attr!(lic_identifier() -> String = LicIdentifier);
    impl_attr!(plant() -> String = Plant);
    impl_attr!(num_core_total() -> i32 = NumCoreTotal);
    impl_attr!(num_core_configured() -> i32 = NumCoreConfigured);
    impl_attr!(num_core_standby() -> i32 = NumCoreStandby);
    impl_attr!(num_core_reserved() -> i32 = NumCoreReserved);
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
    impl_attr!(capability() -> f32 = Capability);
    impl_attr!(secondary_capability() -> f32 = SecondaryCapability);
    impl_attr!(capacity_adjustment_indication() -> i32 = CapacityAdjustmentIndication);
    impl_attr!(capacity_change_reason() -> i32 = CapacityChangeReason);
}

impl CEC<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- Manufacturer: string? = self.manufacturer()?);
        json_pair!(map <- Type: string? = self.machine_type()?);
        json_pair!(map <- TypeName: string? = self.type_name()?);
        json_pair!(map <- TypeFamily: int? = self.type_family()?);
        json_pair!(map <- ModelCapacity: string? = self.model_capacity()?);
        json_pair!(map <- Model: string? = self.model()?);
        json_pair!(map <- SequenceCode: string? = self.sequence_code()?);
        json_pair!(map <- LicIdentifier: string? = self.lic_identifier()?);
        json_pair!(map <- Plant: string? = self.plant()?);
        json_pair!(map <- NumCoreTotal: int? = self.num_core_total()?);
        json_pair!(map <- NumCoreConfigured: int? = self.num_core_configured()?);
        json_pair!(map <- NumCoreStandby: int? = self.num_core_standby()?);
        json_pair!(map <- NumCoreReserved: int? = self.num_core_reserved()?);
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
        json_pair!(map <- Capability: float? = self.capability()?);
        json_pair!(map <- SecondaryCapability: float? = self.secondary_capability()?);
        json_pair!(map <- CapacityAdjustmentIndication: int? = self.capacity_adjustment_indication()?);
        json_pair!(map <- CapacityChangeReason: int? = self.capacity_change_reason()?);
        Ok(())
    }
}
