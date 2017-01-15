#version 330

layout(points) in;
in vec4 gColor[];
in vec2 gPsize[];

layout(triangle_strip, max_vertices = 6) out;
out vec4 fColor;

void main()
{
    float w = gPsize[0].x;
    float h = gPsize[0].y;
    fColor = gColor[0];
    
    gl_Position = gl_in[0].gl_Position + vec4(-w, -h, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(w, -h, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(w, h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position + vec4(-w, -h, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(w, h, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-w, h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

