#version 100

precision highp float;

varying vec2 uvCoord;

uniform sampler2D sampler;

void main() {
    vec2 uv = (uvCoord + vec2(1.0)) / 2.0;
    //uv.y = 1.0 - uv.y;

    gl_FragColor = texture2D(sampler, uv);
    //color = vec4(uv.x, uv.y, 0.0f, 1.0f);
	//color = vec4(0.4, 0.0, 0.7, 1.0);
}