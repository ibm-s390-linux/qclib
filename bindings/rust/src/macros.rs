#[macro_export]
macro_rules! impl_attr_string {
    ($f:ident: $attr:ident) => {
        pub fn $f(&self) -> Result<Option<String>> {
            self.qc.get_attribute_string(self.layer, AttributeId::$attr)
        }
    };
    ($f:ident: $attr:ident; $msg:literal) => {
        pub fn $f(&self) -> Result<String> {
            Ok(self
                .qc
                .get_attribute_string(self.layer, AttributeId::$attr)?
                .expect($msg))
        }
    };
}

#[macro_export]
macro_rules! impl_attr_int {
    ($f:ident: $attr:ident) => {
        pub fn $f(&self) -> Result<Option<i32>> {
            self.qc.get_attribute_int(self.layer, AttributeId::$attr)
        }
    };
    ($f:ident: $attr:ident; $msg:literal) => {
        pub fn $f(&self) -> Result<i32> {
            Ok(self
                .qc
                .get_attribute_int(self.layer, AttributeId::$attr)?
                .expect($msg))
        }
    };
}

#[macro_export]
macro_rules! impl_attr_float {
    ($f:ident: $attr:ident) => {
        pub fn $f(&self) -> Result<Option<f32>> {
            self.qc.get_attribute_float(self.layer, AttributeId::$attr)
        }
    };
}

macro_rules! json_pair {
    ($map:ident; $attr:ident => string: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            ::serde_json::Value::String($v.to_string()),
        )
    };
    ($map:ident; $attr:ident => string?: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            match $v {
                Some(v) => ::serde_json::Value::String(v.to_string()),
                None => ::serde_json::Value::Null,
            },
        )
    };
    ($map:ident; $attr:ident => int: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            ::serde_json::Value::Number($v.into()),
        )
    };
    ($map:ident; $attr:ident => int?: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            match $v {
                Some(v) => ::serde_json::Value::Number(v.into()),
                None => ::serde_json::Value::Null,
            },
        )
    };
    ($map:ident; $attr:ident => float: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            ::serde_json::Value::Number(
                ::serde_json::Number::from_f64($v.into())
                    .unwrap_or_else(|| panic!("BUG: {} is NaN: {:?}", AttributeId::$attr, v)),
            ),
        )
    };
    ($map:ident; $attr:ident => float?: $v:expr) => {
        $map.insert(
            AttributeId::$attr.to_string(),
            match $v {
                Some(v) => ::serde_json::Value::Number(
                    ::serde_json::Number::from_f64(v.into())
                        .unwrap_or_else(|| panic!("BUG: {} is NaN: {:?}", AttributeId::$attr, v)),
                ),
                None => ::serde_json::Value::Null,
            },
        )
    };
}
