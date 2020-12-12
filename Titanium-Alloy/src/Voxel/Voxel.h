#pragma once

/*
* Voxel Framework for rendering voxels. Created by Gijs Groenewegen. Usage: `#include "Voxel/Voxel.h"`.
* For more control of the framework, extra classes can be included.
*/

// Todo: Only include files required to use the framework (e.g. Renderer, Mesh)

#include "src/Mesh/CubLoader.h""
#include "src/Mesh/Cube.h"
#include "src/Mesh/Mesh.h"
#include "src/Mesh/Quad.h"

#include "src/OpenGL/Shader.h"
#include "src/OpenGL/Texture.h"
#include "src/OpenGL/VertexArray.h"
#include "src/OpenGL/VertexBuffer.h"
#include "src/OpenGL/VertexBufferLayout.h"
#include "src/OpenGL/IndexBuffer.h"

#include "src/Renderer/Renderer.h"
#include "src/Renderer/Camera.h"

// Todo: Figure out if this is needed
namespace Voxel {}