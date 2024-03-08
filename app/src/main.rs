#![allow(non_snake_case)]

mod components;
mod painter;

mod constants;
mod io;
mod plane_graph;
mod service;

use crate::components::app::App;
use constants::{INITIAL_WINDOW_HEIGHT, INITIAL_WINDOW_WIDTH};
use dioxus_desktop::{launch::launch, tao::dpi::PhysicalSize, WindowBuilder};

use log::LevelFilter;

fn main() {
    dioxus_logger::init(LevelFilter::Info).expect("failed to init logger");

    launch(
        App,
        vec![],
        dioxus_desktop::Config::new()
            .with_default_menu_bar(false)
            // .with_disable_context_menu(true)
            .with_window(WindowBuilder::new().with_inner_size(PhysicalSize::new(
                INITIAL_WINDOW_WIDTH,
                INITIAL_WINDOW_HEIGHT,
            )))
            .with_custom_head(r#"<link rel="stylesheet" href="public/tailwind.css">"#.to_string()),
    );
}
