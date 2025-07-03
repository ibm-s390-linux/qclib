#[macro_use]
mod macros;

mod enums;
mod error;
mod layer;
mod layers;

use serde_json::{Map, Value};
use sys::*;

pub use enums::{AttributeId, LayerCategory, LayerType};
pub use error::Error;
pub use layer::Layer;
pub use layers::LayerDetails;
pub use layers::{KvmGuest, KvmHypervisor, Lpar, ZvmGuest, ZvmHypervisor, CEC};

type Result<T> = std::result::Result<T, Error>;

/// Opaque handle to the underlying sources of system information provided by [qclib].
///
/// [qclib]: https://github.com/ibm-s390-linux/qclib/
pub struct QueryCapacity {
    handle: *mut ::std::os::raw::c_void,
}

impl QueryCapacity {
    /// Create and return a new handle by attaching to the system information sources for
    /// extracting system information.
    pub fn new() -> Result<Self> {
        let mut rc = 0;
        let handle = unsafe { qc_open(&mut rc) };

        if rc < 0 {
            return Err(Error { code: rc });
        }

        Ok(Self { handle })
    }

    /// Returns an iterator over all available layers.
    ///
    /// Layers are iterated over from bottom to top, i.e. starting from the CEC to the current
    /// layer.
    pub fn layers(&self) -> Result<LayersIter> {
        Ok(LayersIter {
            qc: self,
            layer: 0,
            max_layer: self.get_num_layers()?,
        })
    }

    /// Returns a JSON object containing available layers with their attributes.
    pub fn to_json(&self) -> Result<Value> {
        let map = self
            .layers()?
            .enumerate()
            .map(|(n, layer)| Ok((format!("Layer {n}"), layer.to_json()?)))
            .collect::<Result<Map<_, _>>>()?;
        Ok(Value::Object(map))
    }
}

impl QueryCapacity {
    fn get_num_layers(&self) -> Result<u32> {
        let mut rc = 0;
        let layers = unsafe { sys::qc_get_num_layers(self.as_raw(), &mut rc) };

        if rc < 0 {
            return Err(Error { code: rc });
        }

        Ok(layers as u32)
    }

    fn get_attribute_string(&self, layer: u32, id: AttributeId) -> Result<Option<String>> {
        let mut value: *const std::os::raw::c_char = std::ptr::null_mut();
        let rc = unsafe {
            qc_get_attribute_string(self.as_raw(), id.as_raw(), layer as i32, &mut value)
        };

        match rc {
            1 => Ok(Some(
                unsafe { std::ffi::CStr::from_ptr(value) }
                    .to_string_lossy()
                    .to_string(),
            )),
            0 => Ok(None),
            _ => Err(Error { code: rc }),
        }
    }

    fn get_attribute_int(&self, layer: u32, id: AttributeId) -> Result<Option<i32>> {
        let mut value = 0;
        let rc =
            unsafe { qc_get_attribute_int(self.as_raw(), id.as_raw(), layer as i32, &mut value) };

        match rc {
            1 => Ok(Some(value)),
            0 => Ok(None),
            _ => Err(Error { code: rc }),
        }
    }

    fn get_attribute_float(&self, layer: u32, id: AttributeId) -> Result<Option<f32>> {
        let mut value = 0.0;
        let rc =
            unsafe { qc_get_attribute_float(self.as_raw(), id.as_raw(), layer as i32, &mut value) };

        match rc {
            1 => Ok(Some(value)),
            0 => Ok(None),
            _ => Err(Error { code: rc }),
        }
    }

    fn as_raw(&self) -> *mut ::std::os::raw::c_void {
        self.handle
    }
}

impl Drop for QueryCapacity {
    fn drop(&mut self) {
        unsafe {
            qc_close(self.handle);
        }
    }
}

/// An iterator over layers.
///
/// This struct is created with the [`layers`] method on [`QueryCapacity`]. See it's documentation
/// for more.
///
/// [`layers`]: QueryCapacity::layers
pub struct LayersIter<'h> {
    qc: &'h QueryCapacity,
    layer: u32,
    max_layer: u32,
}

impl<'h> Iterator for LayersIter<'h> {
    type Item = Layer<'h>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.layer == self.max_layer {
            return None;
        }

        let layer = Layer {
            qc: self.qc,
            layer: self.layer,
        };
        self.layer += 1;
        Some(layer)
    }
}

impl DoubleEndedIterator for LayersIter<'_> {
    fn next_back(&mut self) -> Option<Self::Item> {
        if self.layer == self.max_layer {
            return None;
        }

        self.layer += 1;
        Some(Layer {
            qc: self.qc,
            layer: self.max_layer - self.layer,
        })
    }
}
