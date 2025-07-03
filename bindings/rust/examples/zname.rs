use clap::Parser;
use qclib::{LayerDetails, LayerType, QueryCapacity};

type Error = Box<dyn std::error::Error + 'static>;

#[derive(Debug, Parser)]
struct Args {
    #[arg(short, long)]
    all: bool,

    #[arg(short, long)]
    capacity: bool,

    #[arg(short = 'i', long)]
    cpuid: bool,

    #[arg(short = 'u', long)]
    manufacturer: bool,

    #[arg(short, long)]
    model: bool,

    #[arg(short, long)]
    name: bool,
}

impl Args {
    fn defaults(mut self) -> Self {
        if !self.all
            && !self.capacity
            && !self.cpuid
            && !self.manufacturer
            && !self.model
            && !self.name
        {
            self.name = true;
        }
        self
    }
}

fn print_model_info(args: Args) -> Result<(), Error> {
    let qc = QueryCapacity::new()?;
    let Some(layer) = qc
        .layers()?
        .find(|layer| matches!(layer.layer_type(), Ok(LayerType::CEC)))
    else {
        return Err("Error: Could not retrieve CEC information".into());
    };

    match layer.details()? {
        LayerDetails::CEC(cec) => {
            let name = cec.type_name()?.unwrap_or("<unavailable>".to_string());
            let cpuid = cec.machine_type()?.unwrap_or("<unavailable>".to_string());
            let manufacturer = cec.manufacturer()?.unwrap_or("<unavailable>".to_string());
            let model_capacity = cec.model_capacity()?.unwrap_or("<unavailable>".to_string());
            let model = cec.model()?.unwrap_or("<unavailable>".to_string());

            if args.all {
                println!("{name} {model} {model_capacity} {cpuid} {manufacturer}");
            } else {
                if args.name {
                    print!("{name} ");
                }
                if args.model {
                    print!("{model} ");
                }
                if args.capacity {
                    print!("{model_capacity} ");
                }
                if args.cpuid {
                    print!("{cpuid} ");
                }
                if args.manufacturer {
                    print!("{manufacturer} ");
                }
                println!();
            }
        }
        _ => unreachable!(),
    };

    Ok(())
}

fn main() {
    let args = Args::parse().defaults();

    if let Err(err) = print_model_info(args) {
        eprintln!("{}", err);
        std::process::exit(1);
    }
}
