#[cfg(feature = "json")]
use serde_json::{Map, Value};
use sys::*;

use crate::enums::{AttributeId, LayerCategory, LayerType};
use crate::layers::*;
use crate::{QueryCapacity, Result};

/// A handle to a system layer (e.g. CEC, LPAR, etc.).
pub struct Layer<'h> {
    pub(crate) qc: &'h QueryCapacity,
    pub(crate) layer: u32,
}

impl Layer<'_> {
    /// See [AttributeId::LayerTypeNum].
    pub fn layer_type_num(&self) -> Result<i32> {
        Ok(self
            .qc
            .get_attribute_int(self.layer, AttributeId::LayerTypeNum)?
            .expect("BUG: qc_layer_type_num is a common attribute"))
    }

    /// See [AttributeId::LayerCategoryNum].
    pub fn layer_category_num(&self) -> Result<i32> {
        Ok(self
            .qc
            .get_attribute_int(self.layer, AttributeId::LayerCategoryNum)?
            .expect("BUG: qc_layer_category_num is a common attribute"))
    }

    /// See [AttributeId::LayerType] and [LayerType].
    pub fn layer_type(&self) -> Result<LayerType> {
        let ltype: qc_layer_type = self
            .qc
            .get_attribute_int(self.layer, AttributeId::LayerTypeNum)?
            .expect("BUG: qc_layer_type_num is a common attribute")
            .try_into()
            .expect("BUG: qc_layer_type < 0");
        Ok(LayerType::from_raw(ltype))
    }

    /// See [AttributeId::LayerCategory] and [LayerCategory].
    pub fn layer_category(&self) -> Result<LayerCategory> {
        let category: qc_layer_category = self
            .qc
            .get_attribute_int(self.layer, AttributeId::LayerCategoryNum)?
            .expect("BUG: qc_layer_category_num is a common attribute")
            .try_into()
            .expect("BUG: qc_layer_category < 0");
        Ok(LayerCategory::from_raw(category))
    }

    impl_attr!(layer_name() -> String = LayerName);
}

impl<'h> Layer<'h> {
    /// Returns a layer-specific handle that matches the type of the layer.
    pub fn details(&self) -> Result<LayerDetails<'h>> {
        let layer = match self.layer_type()? {
            LayerType::CEC => LayerDetails::CEC(CEC {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::LparGroup => todo!(),
            LayerType::Lpar => LayerDetails::Lpar(Lpar {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::ZvmHypervisor => LayerDetails::ZvmHypervisor(ZvmHypervisor {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::ZvmCpuPool => todo!(),
            LayerType::ZvmResourcePool => todo!(),
            LayerType::ZvmGuest => LayerDetails::ZvmGuest(ZvmGuest {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::KvmHypervisor => LayerDetails::KvmHypervisor(KvmHypervisor {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::KvmGuest => LayerDetails::KvmGuest(KvmGuest {
                qc: self.qc,
                layer: self.layer,
            }),
            LayerType::ZosHypervisor => todo!(),
            LayerType::ZosTenantResourceGroup => todo!(),
            LayerType::ZosZcxServer => todo!(),
        };

        Ok(layer)
    }

    #[cfg(feature = "json")]
    pub fn to_json(&self) -> Result<Value> {
        let mut map = Map::new();

        json_pair!(map <- LayerTypeNum: int = self.layer_type_num()?);
        json_pair!(map <- LayerCategoryNum: int = self.layer_category_num()?);
        json_pair!(map <- LayerType: string = self.layer_type()?);
        json_pair!(map <- LayerCategory: string = self.layer_category()?);
        json_pair!(map <- LayerName: string? = self.layer_name()?);

        match self.details()? {
            LayerDetails::CEC(cec) => cec.update_json(&mut map)?,
            LayerDetails::LparGroup(_) => todo!(),
            LayerDetails::Lpar(lpar) => lpar.update_json(&mut map)?,
            LayerDetails::ZvmHypervisor(zvm_hypervisor) => zvm_hypervisor.update_json(&mut map)?,
            LayerDetails::ZvmCpuPool(_) => todo!(),
            LayerDetails::ZvmResourcePool(_) => todo!(),
            LayerDetails::ZvmGuest(zvm_guest) => zvm_guest.update_json(&mut map)?,
            LayerDetails::KvmHypervisor(kvm_hypervisor) => kvm_hypervisor.update_json(&mut map)?,
            LayerDetails::KvmGuest(kvm_guest) => kvm_guest.update_json(&mut map)?,
            LayerDetails::ZosHypervisor(_) => todo!(),
            LayerDetails::ZosTenantResourceGroup(_) => todo!(),
            LayerDetails::ZosZcxServer(_) => todo!(),
        }

        Ok(Value::Object(map))
    }
}
