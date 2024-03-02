extern crate cairo;

mod draw;
mod io;
mod plane_graph;
mod service;
mod window;

use gtk::prelude::*;
use gtk::{gio, glib, Application};
use window::Window;

const APP_ID: &str = "org.gtk_rs.ara";

fn main() -> glib::ExitCode {
    gio::resources_register_include!("ara.gresource").expect("Failed to register resources.");

    let app = Application::builder().application_id(APP_ID).build();

    app.connect_activate(|app: &Application| {
        let window = Window::new(app);
        window.present();
    });

    app.run()
}
