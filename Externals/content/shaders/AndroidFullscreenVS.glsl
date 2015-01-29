#version 100
//#extension GL_ARB_explicit_attrib_location : require

attribute vec2 position;

varying vec2 uvCoord;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    uvCoord = position;
}