# Titanium-Alloy

Open Source Voxel Framework and game developped in C++ and OpenGL.

## Chunk optimizations
- (Research) Draw all the voxels of the same color and use a uniform color for the whole batch.
- (Optional) Greedy Meshing, combining multiple cube quads into one big quad.
- Store a char[32][32][32] for chunk data. (Or char[96] and get the correct offsets with math)
    - AIR = 0,
    - GRASS = 1,
    - DIRT = 2,
    - STONE = 3,
    - WATER = 4,
- Render only seen triangles/quads of a mesh / chunk.
- Do not render empty chunks
- Do not render (completely) surrounded chunks
