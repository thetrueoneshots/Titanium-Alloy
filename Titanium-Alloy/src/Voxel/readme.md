# Voxel Framework Readme

## Introduction
Welcome to everyone reading this readme. In this readme will explain the functionalities of the framework and contains a small tutorial on how to setup the framework. 

## Functionalities
This framwork contains the following functionalities:
- It can render voxel meshes as single instances, but a mesh can also be batch rendered with multiple transformations.
- It supports the usage of different rendercontexts (OpenGL Shaders, buffers, e.t.c.)
- It contains camera control
- It _will_ contain frustum culling and depth culling

## Tutorial
To start rendering a voxel, you will have to have setup a window and OpenGL already.
Once that is done you can start creating a renderer to render the world and a camera to view the world.

### The setup
You first have to create you renderer and camera.
```c++
struct RendererObject
{
  Voxel::Camera* camera;
  Voxel::Renderer* renderer;
}

RendererObject SetupRenderer(glm::ivec2* screenSize, float renderDistance)
{
  Voxel::Camera* camera = new Camera(screenSize, renderDistance);

  Voxel::Renderer* renderer = new Renderer(camera);
  renderer->Init();
  
  return { camera, renderer };
}
```

At the end, do not forget to clean up your allocated memory.
```c++
void DeleteRenderer(RendererObject obj)
{
  if (obj.camera)
  {
    delete obj.camera;
  }
  
  if (obj.renderer)
  {
    delete obj.renderer;
  }
}
```

### Rendering your first mesh
Once you have your renderer setup, you can start rendering a mesh.

```c++
...

glm::ivec2 screenSize = glm::ivec2(600, 400);
RendererObject rendererObject = SetupRenderer(&screenSize, 500.0f);

Voxel::Mesh* m = new Voxel::Mesh(10, 10, 10);
m->AddCube(glm::vec3(0, 0, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

while(shouldRun)
{
  rendererObject.renderer->Clear();
  rendererObject.renderer->Update();
  
  rendererObject.renderer->Render(m, Voxel::RendeType::VOXEL);
}

DeleteRenderer();
...
```
