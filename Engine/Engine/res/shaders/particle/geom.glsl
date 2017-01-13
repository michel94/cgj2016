#version 330

layout(points) in;
in vec4 gColor[];

layout(triangle_strip, max_vertices = 6) out;
out vec4 fColor;

uniform float d;

void main()
{
    fColor = gColor[0];
    gl_Position = gl_in[0].gl_Position + vec4(-d, -d, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(d, -d, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(d, d, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position + vec4(-d, -d, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(d, d, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-d, d, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

