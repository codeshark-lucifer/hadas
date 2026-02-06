#version 430 core

struct Transform
{
    ivec2 ioffset;
    ivec2 isize;

    vec2 pos;
    vec2 size;
};

layout (std430, binding = 0) buffer TransformSBO
{
    Transform transforms[];
};

layout (location = 0) out vec2 textureCoordsOut;

uniform vec2 screenSize;

void main() {
    Transform transform = transforms[gl_InstanceID];

    vec2 vertices[6] = {
        transform.pos,
        transform.pos + vec2(0.0, transform.size.y),
        transform.pos + vec2(transform.size.x, 0.0),
        transform.pos + vec2(transform.size.x, 0.0),
        transform.pos + vec2(0.0, transform.size.y),
        transform.pos + transform.size
    };

    float left = transform.ioffset.x;
    float top = transform.ioffset.y;
    float right = transform.ioffset.x + transform.isize.x;
    float bottom = transform.ioffset.y + transform.isize.y;

    vec2 textureCoords[6] = {
        vec2(left,  top),
        vec2(left,  bottom),
        vec2(right, top),
        vec2(right, top),
        vec2(left,  bottom),
        vec2(right, bottom)
    };

    {
        vec2 vertexPos = vertices[gl_VertexID];
        vertexPos.y = -vertexPos.y + screenSize.y;
        vertexPos = 2.0 * (vertexPos / screenSize) - 1.0;
        
        gl_Position = vec4(vertexPos, 0.0, 1.0);
    }
    textureCoordsOut = textureCoords[gl_VertexID];
}
