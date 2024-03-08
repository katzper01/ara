use std::rc::Rc;

use crate::components::canvas::Canvas;
use crate::components::config_panel::ConfigPanel;
use crate::draw::canvas_draw::draw_vertex;
use crate::plane_graph::PlaneGraph;
use dioxus::prelude::*;

#[component]
pub fn App() -> Element {
    let graph = use_signal(|| PlaneGraph {
        n: 3,
        edges: vec![(1, 2), (2, 3), (3, 1)],
        embedding: vec![(0, 0), (0, 1), (1, 0)],
    });

    // let f = RefCell::new(|| log::info!("Button clicked!"));

    // let f = use_ref(|| || log::info!("Clicked draw button"));

    // f();

    // let f = use_signal(|| Rc::new(|| log::info!("Button clicked!")));

    rsx! {
        div {
            class: "w-screen h-screen bg-gray-900 flex flex-row divide-gray-600 divide-x-2",
            Canvas { }
            div {
                class: "flex flex-col grow",
                ConfigPanel { }
                button {
                    class: "bg-blue-900 text-white m-1 rounded",
                    onclick: move |_| draw_vertex(50.0, 50.0),
                    "Draw"
                }
            }
        }
    }
}
