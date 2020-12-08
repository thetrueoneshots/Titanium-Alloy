#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in int color;
layout(location = 2) in int normal;

out vec4 v_Color;
out vec3 v_Normal;
//uniform mat4 u_MVP;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    v_Color = vec4(color & 255, (color & (255 << 8)) >> 8, (color & (255 << 16)) >> 16, 255.0f) / 255.0f;
    v_Normal = vec3(normal & 255, (normal & (255 << 8)) >> 8, (normal & (255 << 16)) >> 16) / 255.0f;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Normal;

void main()
{
    // Todo: Add light direction
    // Todo: Add ambient light
    vec4 ambientColor = vec4(1, 1, 1, 1);
    vec3 light = vec3(-.5f, -1.0f, 0.5f);

    float diffuseIntensity = 0.2f;
    float diffuseFactor = dot(normalize(v_Normal), normalize(-light));

    vec4 diffuseColor;
    if (diffuseFactor > 0)
    {
        diffuseColor = vec4(vec3(1, 1, 1) * diffuseIntensity * diffuseFactor, 1.0f);
    }
    else
    {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    color = v_Color * (ambientColor + diffuseColor);
}