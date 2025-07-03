use crate::{AttributeId, QueryCapacity, Result};

/// A handle that represents a logical partition (LPAR).
pub struct Lpar<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl Lpar<'_> {
    impl_attr!(extended_name() -> String = LayerExtendedName);
    impl_attr!(uuid() -> String = LayerUuid);
    impl_attr!(partition_number() -> i32 = PartitionNumber);
    impl_attr!(partition_char() -> String = PartitionChar);
    impl_attr!(partition_char_num() -> i32 = PartitionCharNum);
    impl_attr!(adjustment() -> i32 = Adjustment);
    impl_attr!(has_secure() -> i32 = HasSecure);
    impl_attr!(secure() -> i32 = Secure);
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
    impl_attr!(cp_absolute_capping() -> i32 = CpAbsoluteCapping);
    impl_attr!(cp_weight_capping() -> i32 = CpWeightCapping);
    impl_attr!(ifl_absolute_capping() -> i32 = IflAbsoluteCapping);
    impl_attr!(ifl_weight_capping() -> i32 = IflWeightCapping);
    impl_attr!(ziip_absolute_capping() -> i32 = ZiipAbsoluteCapping);
    impl_attr!(ziip_weight_capping() -> i32 = ZiipWeightCapping);
}

impl Lpar<'_> {
    #[cfg(feature = "json")]
    pub(crate) fn update_json(
        &self,
        map: &mut serde_json::Map<String, serde_json::Value>,
    ) -> Result<()> {
        json_pair!(map <- LayerExtendedName: string? = self.extended_name()?);
        json_pair!(map <- LayerUuid: string? = self.uuid()?);
        json_pair!(map <- Adjustment: int? = self.adjustment()?);
        json_pair!(map <- PartitionNumber: int? = self.partition_number()?);
        json_pair!(map <- PartitionChar: string? = self.partition_char()?);
        json_pair!(map <- PartitionCharNum: int? = self.partition_char_num()?);
        json_pair!(map <- HasSecure: int? = self.has_secure()?);
        json_pair!(map <- Secure: int? = self.secure()?);
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
        json_pair!(map <- CpAbsoluteCapping: int? = self.cp_absolute_capping()?);
        json_pair!(map <- IflAbsoluteCapping: int? = self.ifl_absolute_capping()?);
        json_pair!(map <- ZiipAbsoluteCapping: int? = self.ziip_absolute_capping()?);
        json_pair!(map <- CpWeightCapping: int? = self.cp_weight_capping()?);
        json_pair!(map <- IflWeightCapping: int? = self.ifl_weight_capping()?);
        json_pair!(map <- ZiipWeightCapping: int? = self.ziip_weight_capping()?);
        Ok(())
    }
}
