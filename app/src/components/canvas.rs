use dioxus::prelude::*;

use crate::plane_graph::PlaneGraph;

#[component]
pub fn Canvas(graph: Signal<PlaneGraph>) -> Element {
    rsx! {
        div {
            class: "h-full aspect-square p-8",
            canvas {
                class: "size-full",
                id: "canvas1",
                border_width: "2",
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
