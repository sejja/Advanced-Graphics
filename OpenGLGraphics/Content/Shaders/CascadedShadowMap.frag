uniform sampler2DArray shadowMap;

texture(depthMap, vec3(TexCoords, currentLayer))