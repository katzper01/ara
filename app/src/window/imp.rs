use std::cell::RefCell;

use cairo::Context;
use glib::clone;
use glib::subclass::InitializingObject;
use gtk::subclass::prelude::*;
use gtk::{glib, CompositeTemplate, DrawingArea, Label};
use gtk::{prelude::*, Button};

use crate::{draw::draw_embedding, plane_graph::PlaneGraph};

#[derive(CompositeTemplate, Default)]
#[template(resource = "/ara/window.ui")]
pub struct Window {
    #[template_child]
    pub select_file_button: TemplateChild<Button>,
    #[template_child]
    pub selected_file_label: TemplateChild<Label>,
    #[template_child]
    pub drawing_area: TemplateChild<DrawingArea>,

    pub selected_file_path: RefCell<Option<String>>,
}

#[glib::object_subclass]
impl ObjectSubclass for Window {
    const ABSTRACT: bool = false;
    const NAME: &'static str = "AraWindow";
    type Type = super::Window;
    type ParentType = gtk::ApplicationWindow;

    fn class_init(klass: &mut Self::Class) {
        klass.bind_template();
    }

    fn instance_init(obj: &InitializingObject<Self>) {
        obj.init_template();
    }
}

impl ObjectImpl for Window {
    fn constructed(&self) {
        self.parent_constructed();

        self.obj().set_selected_file(None);

        self.select_file_button
            .connect_clicked(clone!(@weak self as window =>
                move |_| {
                window.obj()
                    .set_selected_file(Some(String::from("/some/path/to/file")));
            }));

        self.drawing_area.set_content_width(600);
        self.drawing_area.set_content_height(600);

        let graph = PlaneGraph {
            n: 5,
            edges: vec![(1, 2), (2, 3), (1, 3)],
            embedding: vec![(0, 0), (1, 0), (0, 1)],
        };

        self.drawing_area.set_draw_func(
            move |_: &DrawingArea, context: &Context, width: i32, height: i32| {
                draw_embedding(context, width as f64, height as f64, &graph);
            },
        );
    }
}

impl WidgetImpl for Window {}

impl WindowImpl for Window {}

impl ApplicationWindowImpl for Window {}
