extern crate cairo;

mod draw;
mod io;
mod plane_graph;
mod service;
mod ui;

use gtk::prelude::*;
use gtk::{glib, Application};
use ui::build_window;

const APP_ID: &str = "org.gtk_rs.ara";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_window);
    app.run()
}
