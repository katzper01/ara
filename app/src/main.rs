#![allow(non_snake_case)]

use dioxus::prelude::*;

pub mod io;
pub mod plane_graph;
pub mod service;

fn main() {
    launch(App);
}

fn App() -> Element {
    let mut text = use_signal(|| String::from("Hello world!"));

    rsx! {
        SimpleDiv {
            text
        }
        button {
            onclick: move |_| *text.write() = String::from("Other text"),
            "Change text"
        }
    }
}

#[component]
fn SimpleDiv(text: Signal<String>) -> Element {
    rsx! { div { "{text}" } }
}
