mod imp;

use glib::Object;
use gtk::prelude::ObjectExt;
use gtk::subclass::prelude::*;
use gtk::{gio, glib, Application};

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

    fn set_selected_file(&self, val: Option<String>) {
        self.imp().selected_file_path.replace(val);

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
}
