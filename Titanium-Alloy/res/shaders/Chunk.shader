#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 v_Color;
out vec3 v_Normal;
//uniform mat4 u_MVP;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    v_Color = color;
    v_Normal = normal;
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

    float diffuseIntensity = 0.1f;
    float diffuseFactor = dot(normalize(v_Normal), normalize(-vec3(0.0f, -1.0f, 0.0f)));

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