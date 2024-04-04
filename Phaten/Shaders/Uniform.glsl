uniform float uTime;

uniform mat4 uModel;

layout (std140) uniform Matrices {
    mat4 uProjectionView;
};

uniform float uDragFloat;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
