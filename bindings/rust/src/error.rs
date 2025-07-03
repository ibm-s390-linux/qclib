#[derive(Debug)]
pub struct Error {
    pub(crate) code: i32,
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "internal qclib error: {}", self.code)
    }
}

impl std::error::Error for Error {}
