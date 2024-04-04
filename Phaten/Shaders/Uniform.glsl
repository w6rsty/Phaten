uniform float uTime;

uniform mat4 uModel;

layout (std140) uniform Matrices {
    mat4 uProjection;
    mat4 uView;
};

uniform float uDragFloat;

uniform vec3 uCameraPosition;

uniform samplerCube uSkybox;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;