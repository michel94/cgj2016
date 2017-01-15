#version 330

layout(points) in;
in vec4 gColor[];

layout(triangle_strip, max_vertices = 6) out;
out vec4 fColor;

uniform float w, h;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main()
{
	vec4 pos1 = ProjMatrix * ViewMatrix * Matrix * gl_in[0].gl_Position;
	vec4 pos2 = ProjMatrix * ViewMatrix * Matrix * (gl_in[0].gl_Position + vec4(0,-0.2,0,0));

    fColor = gColor[0];
    gl_Position = pos2 + vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos2+ vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1 + vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = pos2+ vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1+ vec4(w, 0, 0.0, 0.0);
    EmitVertex();
    gl_Position = pos1+ vec4(-w, 0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

