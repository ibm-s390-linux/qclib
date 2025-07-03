fn main() {
    println!("cargo::rustc-link-lib=dylib:+verbatim=libqc.so.2");
}
