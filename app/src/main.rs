#![allow(non_snake_case)]

mod components;
mod io;
mod plane_graph;
mod service;

use crate::components::app::App;
use dioxus_desktop::launch::launch;

use log::LevelFilter;

fn main() {
    dioxus_logger::init(LevelFilter::Info).expect("failed to init logger");

    launch(
        App,
        vec![],
        dioxus_desktop::Config::new()
            .with_custom_head(r#"<link rel="stylesheet" href="public/tailwind.css">"#.to_string()),
    );
    // launch(App);
}

// fn draw_rectangle(cx: Scope) {
//     let create_eval = use_eval(cx);
//
//     create_eval(
//         r#"
//         const canvas = document.getElementById('canvas1');
//         const ctx = canvas.getContext('2d');
//
//         ctx.fillStyle = 'white';
//         ctx.fillRect(10, 20, 150, 50);
//         "#,
//     )
//     .unwrap();
// }

// #[component]
// fn SimpleDiv(text: Signal<String>) -> Element {
//     rsx! {
//         div { "{text}" }
//     }
// }
