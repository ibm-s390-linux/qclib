mod cec;
mod kvm_guest;
mod kvm_hypervisor;
mod lpar;
mod lpar_group;
mod zos_hypervisor;
mod zos_tenant_resource_group;
mod zos_zcx_server;
mod zvm_cpu_pool;
mod zvm_guest;
mod zvm_hypervisor;
mod zvm_resource_pool;

pub use cec::CEC;
pub use kvm_guest::KvmGuest;
pub use kvm_hypervisor::KvmHypervisor;
pub use lpar::Lpar;
pub use lpar_group::LparGroup;
pub use zos_hypervisor::ZosHypervisor;
pub use zos_tenant_resource_group::ZosTenantResourceGroup;
pub use zos_zcx_server::ZosZcxServer;
pub use zvm_cpu_pool::ZvmCpuPool;
pub use zvm_guest::ZvmGuest;
pub use zvm_hypervisor::ZvmHypervisor;
pub use zvm_resource_pool::ZvmResourcePool;

/// A handle to retrieve layer-specific system information.
pub enum LayerDetails<'h> {
    /// CEC
    CEC(CEC<'h>),
    /// LPAR Capping Group
    LparGroup(LparGroup),
    /// LPAR
    Lpar(Lpar<'h>),
    /// z/VM Hypervisor
    ZvmHypervisor(ZvmHypervisor<'h>),
    /// z/VM CPU Pool (deprecated, use QC_LAYER_TYPE_ZVM_RESOURCE_POOL instead)
    ZvmCpuPool(ZvmCpuPool),
    /// z/VM Resource Pool
    ZvmResourcePool(ZvmResourcePool),
    /// z/VM Guest
    ZvmGuest(ZvmGuest<'h>),
    /// KVM Hypervisor
    KvmHypervisor(KvmHypervisor<'h>),
    /// KVM Guest
    KvmGuest(KvmGuest<'h>),
    /// z/OS Hypervisor
    ZosHypervisor(ZosHypervisor),
    /// z/OS Tenant Resource Group
    ZosTenantResourceGroup(ZosTenantResourceGroup),
    /// z/OS cCX Server
    ZosZcxServer(ZosZcxServer),
}
