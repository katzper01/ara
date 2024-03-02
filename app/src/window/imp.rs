use cairo::Context;
use glib::subclass::InitializingObject;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk::{glib, CompositeTemplate, DrawingArea};

use crate::{draw::draw_embedding, plane_graph::PlaneGraph};

#[derive(CompositeTemplate, Default)]
#[template(resource = "/ara/window.ui")]
pub struct Window {
    #[template_child]
    pub drawing_area: TemplateChild<DrawingArea>,
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

        self.drawing_area.set_content_width(600);
        self.drawing_area.set_content_height(600);

        let graph = PlaneGraph {
            n: 5,
            edges: vec![(1, 2), (2, 3), (1, 3)],
            embedding: vec![(0, 0), (1, 0), (0, 1)],
        };

        self.drawing_area.set_draw_func(
            move |_: &DrawingArea, context: &Context, width: i32, height: i32| {
                // on_draw(da, context, width, height, graph.clone());
                draw_embedding(context, width as f64, height as f64, &graph);
            },
        );
    }
}

impl WidgetImpl for Window {}

impl WindowImpl for Window {}

impl ApplicationWindowImpl for Window {}
