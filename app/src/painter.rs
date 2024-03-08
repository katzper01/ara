use dioxus::events::eval_provider;

use crate::components::canvas::CANVAS_ID;

static VERTEX_RADIUS: f64 = 20.0;

pub fn paint_vertex(canvas_size: f64, x: f64, y: f64) {
    let run_js = eval_provider();

    let pos_x = canvas_size * x;
    let pos_y = canvas_size * y;

    let command = format!(
        r#"
        const canvas = document.getElementById('{CANVAS_ID}');
        const ctx = canvas.getContext('2d');

        var radius = 10;
        ctx.beginPath();
        ctx.strokeStyle = '#ffffff';
        ctx.arc({pos_x}, {pos_y}, radius, 0, 2 * Math.PI);
        ctx.stroke()
        "#
    );

    log::info!("Command:\n{command}");

    run_js(command.as_str());
}
