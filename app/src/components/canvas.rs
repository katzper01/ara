use dioxus::prelude::*;

use crate::constants::{CANVAS_ID, CANVAS_SIZE, MAIN_PADDING};

#[component]
pub fn Canvas() -> Element {
    let canvas_side_length = format!("{CANVAS_SIZE}px");
    let canvas_container_side_length = (CANVAS_SIZE + 2 * MAIN_PADDING).to_string() + "px";

    rsx! {
        div {
            class: format!("h-full flex items-center min-w-[{canvas_container_side_length}] p-8"),
            canvas {
                // class: "w-full object-contain",
                width: canvas_side_length.clone(),
                height: canvas_side_length.clone(),
                id: CANVAS_ID,
                background_color: "#000000",
                onmousedown: move |event| {
                    log::info!("mouse down ({:?})", event.data.coordinates().element().to_tuple())
                    // log::info!("OnMouseDown Event: {event:?}");
                },
                onmouseup: move |event| {
                    log::info!("mouse up ({:?})", event.data.coordinates().element().to_tuple())
                },
                // onmousemove: move |event| {
                    // println!("onmouse move event");
                // }
            }
        }
    }
}
