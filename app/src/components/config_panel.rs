use dioxus::prelude::*;

#[component]
pub fn ConfigPanel() -> Element {
    rsx! {
        div {
            class: "m-1 grow p-8",
            div {
                class: "size-full bg-lime-900"
            }
        }
    }
}
