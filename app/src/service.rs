use cairo::glib::Error;
use std::process::Command;

use crate::io::{parse_output, read_graph_edges};
use crate::plane_graph::PlaneGraph;

pub fn build_plane_graph_from_file(input_file_path: &str) -> Result<PlaneGraph, Error> {
    let edges: Vec<(u32, u32)> = read_graph_edges(input_file_path);
    let output = call_ara_service(input_file_path);
    let embedding = parse_output(&output);

    let graph = PlaneGraph {
        n: embedding.len() as u32,
        edges,
        embedding,
    };

    Ok(graph)
}

fn call_ara_service(input_file_path: &str) -> String {
    let output = String::from_utf8(
        Command::new("aracli")
            .arg(input_file_path)
            .output()
            .unwrap()
            .stdout,
    )
    .expect("Failed to read output from ara service.");

    // TODO: If failed do something
    // for example return result

    output
}
