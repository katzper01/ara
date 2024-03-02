# ara
Tool for creating colorful and customizable plane graphs visualizations

## Install

```
chmod +x install.sh
./install.sh
```

## Usage

```
ara path/to/input/file
```

The input file should look like:

```
numer_of_vertices number_of_edges
a_1 b_1
a_2 b_2
...
a_m b_m
```

Where `a_i b_i` are graph edges. Look inside `examples/` for clarification.

## TODO

* Builder for graph data class
* Drag and drop feature
* Custom colors of vertices/edges/background
* Hide/show grid 
* Schnyder wood visualization
