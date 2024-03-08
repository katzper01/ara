use crate::components::canvas::Canvas;
use crate::components::config_panel::ConfigPanel;
use crate::painter::paint_vertex;
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
            Canvas { /*canvas_size*/ }
            div {
                class: "flex flex-col items-center grow p-8",
                ConfigPanel { }
                button {
                    class: "bg-blue-900 text-white rounded m-4 w-48 h-10",
                    onclick: move |_| paint_vertex(0.5, 0.5),
                    "Draw"
                }
            }
        }
    }
}
