uniform float uTime;

uniform mat4 uModel;

layout (std140) uniform Commons {
    mat4 uProjection;
    mat4 uView;
    // vec3 uCameraPosition;
    // float uDragFloat0;
    // float uDragFloat1;
    // float uDragFloat2;
    // float uDragFloat3;
};

uniform samplerCube uSkybox;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;