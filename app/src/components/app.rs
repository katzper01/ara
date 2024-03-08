use crate::components::canvas::Canvas;
use crate::components::config_panel::ConfigPanel;
use crate::painter::paint_vertex;
use crate::plane_graph::PlaneGraph;
use dioxus::prelude::*;
use dioxus_desktop::use_window;

use std::cmp::min;

#[component]
pub fn App() -> Element {
    let graph = use_signal(|| PlaneGraph {
        n: 3,
        edges: vec![(1, 2), (2, 3), (3, 1)],
        embedding: vec![(0, 0), (0, 1), (1, 0)],
    });

    let window = use_window();
    let window_size = use_memo(move || window.inner_size());
    let window_width = use_memo(move || window_size.read().width);
    let window_height = use_memo(move || window_size.read().height);

    let canvas_size = use_signal(|| min(*window_width.read(), *window_height.read()) as f64);

    use_effect(move || log::info!("{canvas_size}"));

    rsx! {
        div {
            class: "w-screen h-screen bg-gray-900 flex flex-row divide-gray-600 divide-x-2",
            Canvas { canvas_size }
            div {
                class: "flex flex-col grow",
                ConfigPanel { }
                button {
                    class: "bg-blue-900 text-white m-1 rounded",
                    onclick: move |_| paint_vertex(300.0, 50.0, 50.0),
                    "Draw"
                }
            }
        }
    }
}
