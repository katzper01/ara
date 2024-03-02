#[derive(Clone)]
pub struct PlaneGraph {
    pub n: u32,
    pub edges: Vec<(u32, u32)>,
    pub embedding: Vec<(u32, u32)>,
}
