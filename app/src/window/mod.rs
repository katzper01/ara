mod imp;

use cairo::Context;
use glib::clone;
use glib::Object;
use gtk::gio::Cancellable;
use gtk::prelude::ObjectExt;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk::DrawingArea;
use gtk::FileDialog;
use gtk::{gio, glib, Application};

use crate::draw::draw_embedding;
use crate::service::build_plane_graph_from_file;

glib::wrapper! {
    pub struct Window(ObjectSubclass<imp::Window>)
        @extends gtk::ApplicationWindow, gtk::Window, gtk::Widget,
        @implements gio::ActionGroup, gio::ActionMap, gtk::Accessible, gtk::Buildable,
                    gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl Window {
    pub fn new(app: &Application) -> Self {
        Object::builder().property("application", app).build()
    }

    fn setup_signals(&self) {
        // Open file dialog button on click.
        self.imp()
            .select_file_button
            .connect_clicked(clone!(@weak self as window =>
                move |_| {
                    let file_dialog = FileDialog::new();
                    file_dialog.open(
                        Some(&window),
                        Some(&Cancellable::new()),
                        clone!(@weak window => move |result| {
                            match result {
                                Ok(file) => window.set_selected_file(
                                    file.path()
                                    .map(|path| String::from(path.to_str().unwrap()))
                                ),
                                Err(_) => {}
                            };
                        })
                    );
                }
            ));

        // Draw button on click.
        self.imp()
            .draw_button
            .connect_clicked(clone!(@weak self as window => move |_| window.draw_selected_graph()));
    }

    fn set_selected_file(&self, path: Option<String>) {
        self.imp().selected_file_path.replace(path);

        let label = self
            .imp()
            .selected_file_path
            .clone()
            .take()
            .unwrap_or(String::from("???"));

        self.imp()
            .selected_file_label
            .set_property("label", String::from("Selected file: ") + &label)
    }

    fn draw_selected_graph(&self) {
        let graph = match self.imp().selected_file_path.clone().take() {
            Some(path) => build_plane_graph_from_file(&path),
            _ => None,
        };

        if graph.is_some() {
            self.imp().drawing_area.set_draw_func(
                move |_: &DrawingArea, context: &Context, width: i32, height: i32| {
                    draw_embedding(
                        context,
                        width as f64,
                        height as f64,
                        &graph.clone().unwrap(),
                    );
                },
            )
        }
    }
}
