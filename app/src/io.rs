use itertools::Itertools;
use std::fs::read_to_string;

pub fn parse_output(output: &str) -> Vec<(u32, u32)> {
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

pub fn read_graph_edges(filename: &str) -> Vec<(u32, u32)> {
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
