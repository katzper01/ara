use crate::components::canvas::Canvas;
use crate::components::config_panel::ConfigPanel;
use crate::plane_graph::PlaneGraph;
use dioxus::prelude::*;

#[component]
pub fn App() -> Element {
    let graph = use_signal(|| PlaneGraph {
        n: 3,
        edges: vec![(1, 2), (2, 3), (3, 1)],
        embedding: vec![(0, 0), (0, 1), (1, 0)],
    });

    rsx! {
        div {
            class: "w-screen h-screen bg-gray-900 flex flex-row divide-gray-600 divide-x-2",
            Canvas { graph }
            ConfigPanel {}
        }
    }
}
