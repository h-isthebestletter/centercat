fn main() -> Result<(), &'static str> {
    let mut args = std::env::args();
    if args.len() != 2 {
        // user did not supply a filename
        return Err("No filename supplied");
    }

    // unwrap args.nth() because we know that it exists (not None)
    let contents = std::fs::read_to_string(args.nth(1).unwrap()).map_err(|_| "Could not read file")?;
    if contents.len() == 0 {
        // file is empty
        return Ok(());
    }

    let lines: Vec<&str> = contents.split('\n').collect();
    // max() will return Option::None if the iterator is empty.
    // since we checked above that the file is not empty,
    // we can unwrap.
    let longest_line_len = lines.iter().map(|line| line.len()).max().unwrap();

    for line in lines {
        if line.len() == 0 {
            // blank line, so we skip to the next line
            println!();
        } else {
            print!(
                "{}",
                " ".repeat((longest_line_len - line.len()) / 2)
            );
            println!("{}", line);
        }
    }

    Ok(())
}
