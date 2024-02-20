extern crate cairo;
use cairo::{Context, Format, ImageSurface};
use itertools::Itertools;
use std::env;
use std::f64::consts::PI;
use std::fs::{read_to_string, File};
use std::process::Command;

fn parse_output(output: &str) -> Vec<(u32, u32)> {
    output
        .split("\n")
        .filter(|line| line.len() > 0)
        .map(|line| {
            line.to_string()
                .split(" ")
                .map(|c| c.parse::<u32>().unwrap())
                .next_tuple()
                .unwrap()
        })
        .collect()
}

fn read_graph_edges(filename: &str) -> Vec<(u32, u32)> {
    read_to_string(filename)
        .expect("Failed to open input file.")
        .lines()
        .skip(1)
        .map(|line| {
            line.to_string()
                .split(" ")
                .map(|c| c.parse::<u32>().unwrap())
                .next_tuple()
                .unwrap()
        })
        .collect()
}

fn draw_embedding(embedding: Vec<(u32, u32)>, edges: Vec<(u32, u32)>) {
    // Some constants
    let height = 600.0;
    let width = 600.0; // For now we assume height = width
    let margin = 50.0;

    let surface = ImageSurface::create(Format::ARgb32, height as i32, width as i32)
        .expect("Failed to create a surface.");
    let context = Context::new(&surface).expect("Failed to create context.");

    context.set_source_rgb(0.0, 0.0, 0.0);
    let _ = context.paint();

    let n = embedding.len();

    // Draw grid.
    context.set_source_rgb(0.5, 0.5, 0.5);
    let space = (height - 2.0 * margin) / ((n as f64) - 2.0);

    let mut x = margin;
    for _ in 0..n - 1 {
        context.move_to(x, margin);
        context.line_to(x, height - margin);
        x += space;
    }
    let mut y = margin;
    for _ in 0..n - 1 {
        context.move_to(margin, y);
        context.line_to(width - margin, y);
        y += space;
    }
    let _ = context.stroke();

    // Draw vertices.
    context.set_source_rgb(90.0 / 256.0, 23.0 / 256.0, 135.0 / 256.0);
    for (x, y) in embedding.clone() {
        context.arc(
            margin + (x as f64) * space,
            margin + (y as f64) * space,
            15.0,
            0.0,
            2.0 * PI,
        );
        let _ = context.fill_preserve();
        let _ = context.stroke();
    }

    // Draw edges.
    for (a, b) in edges {
        let (ax, ay) = embedding[(a - 1) as usize];
        let (bx, by) = embedding[(b - 1) as usize];
        context.move_to(margin + (ax as f64) * space, margin + (ay as f64) * space);
        context.line_to(margin + (bx as f64) * space, margin + (by as f64) * space);
    }
    let _ = context.stroke();

    let mut file = File::create("output.png").expect("Failed to create output file.");

    surface
        .write_to_png(&mut file)
        .expect("Failed to write to file.");
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("Usage: ara [path to input file].");
        std::process::exit(1);
    }

    let input_filename = &args[1];

    let edges: Vec<(u32, u32)> = read_graph_edges(input_filename);

    let output = String::from_utf8(
        Command::new("aracli")
            .arg(input_filename)
            .output()
            .unwrap()
            .stdout,
    )
    .expect("Failed to read output from ara service.");

    let embedding = parse_output(&output);

    draw_embedding(embedding, edges);

    println!("Success!\nThe drawing has been saved to output.png.")
}
