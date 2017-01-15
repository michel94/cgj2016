#version 330

layout(points) in;
in vec4 gColor[];
in vec2 gPsize[];

layout(triangle_strip, max_vertices = 6) out;
out vec4 fColor;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main()
{
    float w = gPsize[0].x / 2;
    float h = gPsize[0].y;
    fColor = gColor[0];

    vec4 pos1 = ProjMatrix * ViewMatrix * Matrix * gl_in[0].gl_Position;
    vec4 pos2 = ProjMatrix * ViewMatrix * Matrix * (gl_in[0].gl_Position - vec4(0,h,0,0));

    gl_Position = pos2 + vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos2 + vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1 + vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = pos2 + vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1 + vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1 + vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

