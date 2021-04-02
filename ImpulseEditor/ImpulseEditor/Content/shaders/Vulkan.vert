#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 u_ViewProjection;
    mat4 u_Transform;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.u_ViewProjection * ubo.u_Transform * vec4(inPosition,1.0);
    fragColor = inColor;
}