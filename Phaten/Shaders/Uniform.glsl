uniform float uTime;

uniform mat4 uModel;

layout (std140) uniform Matrices {
    mat4 uProjectionView;
};

uniform sampler2D uTexture;
