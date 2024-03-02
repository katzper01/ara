use cairo::Context;
use std::f64::consts::PI;

use crate::plane_graph::PlaneGraph;

pub fn draw_embedding(context: &Context, width: f64, height: f64, graph: &PlaneGraph) {
    let margin = 50.0;

    // let surface = ImageSurface::create(Format::ARgb32, height as i32, width as i32)
    //     .expect("Failed to create a surface.");
    // let context = Context::new(&surface).expect("Failed to create context.");

    context.set_source_rgb(0.0, 0.0, 0.0);
    context.paint().unwrap();

    let n = graph.embedding.len();

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
    context.stroke().unwrap();

    // Draw vertices.
    context.set_source_rgb(90.0 / 256.0, 23.0 / 256.0, 135.0 / 256.0);
    for (x, y) in graph.embedding.clone() {
        context.arc(
            margin + (x as f64) * space,
            margin + (y as f64) * space,
            15.0,
            0.0,
            2.0 * PI,
        );
        context.fill_preserve().unwrap();
        context.stroke().unwrap();
    }

    // Draw edges.
    for (a, b) in graph.edges.clone() {
        let (ax, ay) = graph.embedding[(a - 1) as usize];
        let (bx, by) = graph.embedding[(b - 1) as usize];
        context.move_to(margin + (ax as f64) * space, margin + (ay as f64) * space);
        context.line_to(margin + (bx as f64) * space, margin + (by as f64) * space);
    }
    context.stroke().unwrap();

    // let mut file = File::create("output.png").expect("Failed to create output file.");
    // surface
    //     .write_to_png(&mut file)
    //     .expect("Failed to write to file.");
}
