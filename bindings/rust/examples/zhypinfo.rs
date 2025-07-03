use clap::Parser;
use qclib::{LayerCategory, LayerDetails, QueryCapacity};

type Error = Box<dyn std::error::Error + 'static>;

#[derive(Debug, Parser)]
struct Args {
    #[arg(short, long)]
    json: bool,

    #[arg(short, long)]
    layers: bool,

    #[arg(short = 'L', long)]
    levels: bool,
}

impl Args {
    fn validate(self) -> Result<Self, Error> {
        if self.json && self.layers || self.layers && self.levels || self.levels && self.json {
            Err("Error: Only one of options --json, --layers and --levels is allowed".into())
        } else {
            Ok(self)
        }
    }
}

fn print_json(qc: QueryCapacity) -> Result<(), Error> {
    println!("{}", serde_json::to_string_pretty(&qc.to_json()?)?);
    Ok(())
}

fn print_layer_count(qc: QueryCapacity) -> Result<(), Error> {
    let layers = qc.layers()?.count();
    println!("{layers}");
    Ok(())
}

fn print_level_count(qc: QueryCapacity) -> Result<(), Error> {
    let levels = qc
        .layers()?
        .filter(|layer| layer.layer_category().unwrap() == LayerCategory::Host)
        .count();
    println!("{levels}");
    Ok(())
}

fn print_layers(qc: QueryCapacity) -> Result<(), Error> {
    let mut level = -1;

    let infos =  qc.layers()?.enumerate().map(|(n, layer)| {
        if layer.layer_category()? == LayerCategory::Host {
            level += 1;
        }

        let (ifl, cp, total) = match layer.details()? {
            LayerDetails::CEC(cec) => {
                let ifl = cec.num_ifl_total()?;
                let cp = cec.num_cp_total()?;
                let total = match (cec.num_core_dedicated()?, cec.num_core_shared()?) {
                    (Some(ifl), Some(cp)) => Some(ifl + cp),
                    _ => None,
                };
                (ifl, cp, total)
            }
            LayerDetails::LparGroup(_) => todo!(),
            LayerDetails::Lpar(lpar) => {
                let ifl = lpar.num_ifl_total()?;
                let cp = lpar.num_cp_total()?;
                let total = match (ifl, cp) {
                    (Some(ifl), Some(cp)) => Some(ifl + cp),
                    _ => None,
                };

                (ifl, cp, total)
            }
            LayerDetails::ZvmHypervisor(hypervisor) => {
                let ifl = hypervisor.num_ifl_total()?;
                let cp = hypervisor.num_cp_total()?;
                let total = hypervisor.num_core_total()?;
                (ifl, cp, total)
            },
            LayerDetails::ZvmCpuPool(_) => todo!(),
            LayerDetails::ZvmResourcePool(_) => todo!(),
            LayerDetails::ZvmGuest(guest) => {
                let ifl = guest.num_ifl_total()?;
                let cp = guest.num_cp_total()?;
                let total = guest.num_cpu_total()?;
                (ifl, cp, total)
            }
            LayerDetails::KvmHypervisor(hypervisor) => {
                let ifl = hypervisor.num_ifl_total()?;
                let cp = hypervisor.num_cp_total()?;
                let total = hypervisor.num_core_total()?;
                (ifl, cp, total)
            }
            LayerDetails::KvmGuest(guest) => {
                let ifl = guest.num_ifl_total()?;
                (ifl, Some(0), ifl)
            }
            LayerDetails::ZosHypervisor(_) => todo!(),
            LayerDetails::ZosTenantResourceGroup(_) => todo!(),
            LayerDetails::ZosZcxServer(_) => todo!(),
        };

        let layer_type = layer.layer_type()?;
        let category = layer.layer_category()?;
        let name = layer.layer_name()?.unwrap_or("-".to_string());
        let ifl = ifl.map(|v| format!("{v}")).unwrap_or("-".to_string());
        let cp = cp.map(|v| format!("{v}")).unwrap_or("-".to_string());
        let total = total.map(|v| format!("{v}")).unwrap_or("-".to_string());

        let fields = [
            format!("{n:3}"),
            format!("{layer_type:26}"),
            format!("{level:3}"),
            format!("{category:5}"),
            format!("{name:8}"),
            format!("{ifl:>5}"),
            format!("{cp:>5}"),
            format!("{total:>5}"),
        ];

        Ok(fields.join("  "))
    }).collect::<Result<Vec<_>, Error>>()?;

    println!("  #  Layer_Type                  Lvl  Categ  Name       IFLs    CPs  Total");
    println!("--------------------------------------------------------------------------");

    for info in infos.iter().rev() {
        println!("{info}");
    }

    Ok(())
}

fn main() {
    let args = match Args::parse().validate() {
        Ok(args) => args,
        Err(err) => {
            eprintln!("{}", err);
            std::process::exit(1);
        }
    };

    let qc = match QueryCapacity::new() {
        Ok(qc) => qc,
        Err(err) => {
            eprintln!("Error: Could not open capacity data: {}", err);
            std::process::exit(1);
        }
    };

    if let Err(err) = {
        if args.json {
            print_json(qc)
        } else if args.layers {
            print_layer_count(qc)
        } else if args.levels {
            print_level_count(qc)
        } else {
            print_layers(qc)
        }
    } {
        eprintln!("{}", err);
        std::process::exit(1);
    }
}
