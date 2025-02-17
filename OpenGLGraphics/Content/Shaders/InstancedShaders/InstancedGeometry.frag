#version 460 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;
layout (location = 3) out vec3 gBrightness;

layout (location = 5) in vec2 oUVs;
layout (location = 6) in vec3 oNormal;
layout (location = 7) in vec4 oPosition;
layout (location = 8) in vec3 oTangent;
layout (location = 9) in vec3 oBitangent;

layout(location = 0) uniform mat4 uModel;

layout (std140, binding = 0) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

layout(std140, binding = 1) uniform TransformBlock {
    mat4 instancedGeometryPos[100]; 
};


layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 1) uniform sampler2D uNormalTex;
layout(binding = 9) uniform samplerCube uSkyBox;

in flat int instancedID;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = oPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(inverse(transpose(mat3(ubView * instancedGeometryPos[instancedID]))) * 
        mat3(oTangent, oBitangent, oNormal) * 
        (texture(uNormalTex, oUVs).rgb * 2.0f - 1.0f));
    // and the diffuse per-fragment color
    const vec4 albedo = texture(uDiffuseTex, oUVs) ;
    gAlbedoSpec.rgb = albedo.rgb;
    gBrightness = vec3(0, 0, 0);
}
