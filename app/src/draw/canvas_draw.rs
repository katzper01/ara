use dioxus::events::eval_provider;

use crate::components::canvas::CANVAS_ID;

pub fn draw_vertex(x: f64, y: f64) {
    let run_js = eval_provider();

    let command = format!(
        r#"
        const canvas = document.getElementById('{CANVAS_ID}');
        const ctx = canvas.getContext('2d');

        var radius = 10;
        ctx.beginPath();
        ctx.strokeStyle = '#ffffff';
        ctx.arc({x}, {y}, radius, 0, 2 * Math.PI);
        ctx.stroke()
        "#
    );

    log::info!("Command:\n{command}");

    run_js(command.as_str());
}
