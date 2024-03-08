use dioxus::prelude::*;

pub static CANVAS_ID: &str = "main_canvas";

#[component]
pub fn Canvas(canvas_size: Signal<f64>) -> Element {
    let size = format!("{canvas_size}");

    rsx! {
        div {
            class: "h-full aspect-square p-8",
            canvas {
                // class: "w-full object-contain",
                width: size.clone(),
                height: size.clone(),
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
