use cairo::Context;
use gtk::{prelude::*, DrawingArea};
use gtk::{Application, ApplicationWindow};

use crate::draw::draw_embedding;
use crate::plane_graph::PlaneGraph;

pub fn build_window(app: &Application) {
    let drawing_area = DrawingArea::builder()
        .margin_end(20)
        .margin_top(20)
        .margin_bottom(20)
        .margin_start(20)
        .build();

    drawing_area.set_content_width(600);
    drawing_area.set_content_height(600);

    let graph = PlaneGraph {
        n: 5,
        edges: vec![(1, 2), (2, 3), (1, 3)],
        embedding: vec![(0, 0), (1, 0), (0, 1)],
    };

    drawing_area.set_draw_func(
        move |da: &DrawingArea, context: &Context, width: i32, height: i32| {
            on_draw(da, context, width, height, graph.clone());
        },
    );

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Ara")
        .default_width(640)
        .default_height(640)
        .child(&drawing_area)
        .build();

    window.present();
}

fn on_draw(_: &DrawingArea, context: &Context, width: i32, height: i32, graph: PlaneGraph) {
    draw_embedding(context, width as f64, height as f64, &graph);
}
