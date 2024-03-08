use dioxus::events::eval_provider;

use crate::constants::{CANVAS_ID, CANVAS_SIZE};

pub fn paint_vertex(x: f64, y: f64) {
    let run_js = eval_provider();

    let pos_x = (CANVAS_SIZE as f64) * x;
    let pos_y = (CANVAS_SIZE as f64) * y;

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
