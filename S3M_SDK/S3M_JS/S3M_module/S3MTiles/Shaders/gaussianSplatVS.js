export default `
varying vec2 v_vertPos;
varying vec4 vColor;
uniform float u_splatCount;
uniform float u_splatScale;
uniform vec2 u_textureSize;
uniform vec2 u_indexTextureSize;
uniform int u_indexOffset;
uniform highp usampler2D u_splatAttributeTexture;
uniform mediump usampler2D u_splatIndexesTexture;
varying vec4 vSecondColor;
varying vec3 v_positionMC;

vec2 getDataUV(in uint splatIndex, in int stride, in int offset, in vec2 dimensions) {
    vec2 samplerUV = vec2(0.0, 0.0);
    float d = float(splatIndex * uint(stride) + uint(offset)) / dimensions.x;
    samplerUV.y = float(floor(d)) / dimensions.y;
    samplerUV.x = fract(d);
    return samplerUV;
}

ivec2 getUVivec2(int idx) {
    ivec2 samplerUV = ivec2(0, 0);
    int x = idx % int(u_indexTextureSize.x);
    int y = idx / int(u_indexTextureSize.x);
    samplerUV.x = int(x);
    samplerUV.y = int(y);
    return samplerUV;
}

vec4 calcCovVectors(vec3 viewPos, mat3 Vrk) {
    vec4 t = vec4(viewPos, 1.0);
    float focal = czm_viewport.z * czm_projection[0][0];

    float J1 = focal / t.z;
    vec2 J2 = -J1 / t.z * t.xy;
    mat3 J = mat3(
        J1, 0.0, J2.x,
        0.0, J1, J2.y,
        0.0, 0.0, 0.0
    );

    mat3 R = mat3(czm_modelView);
    vec3 scale;
    scale.x = length(R[0].xyz);
    scale.y = length(R[1].xyz);
    scale.z = length(R[2].xyz);

    mat3 Rs = mat3(
        R[0].xyz / scale.x,
        R[1].xyz / scale.y,
        R[2].xyz / scale.z
    );

    vec3 modelScale = scale * u_splatScale;
    mat3 scaleMat = mat3(
        modelScale.x, 0.0, 0.0,
        0.0, modelScale.y, 0.0,
        0.0, 0.0, modelScale.z
    );

    Vrk = scaleMat * Vrk * transpose(scaleMat);

    mat3 Vrk_view = Rs * Vrk * transpose(Rs);
    mat3 cov = transpose(J) * Vrk_view * J;

    float diagonal1 = cov[0][0] + .3;
    float offDiagonal = cov[0][1];
    float diagonal2 = cov[1][1] + .3;

    float mid = 0.5 * (diagonal1 + diagonal2);
    float radius = length(vec2((diagonal1 - diagonal2) * 0.5, offDiagonal));
    float lambda1 = mid + radius;
    float lambda2 = max(mid - radius, 0.1);

    vec2 diagonalVector = normalize(vec2(offDiagonal, lambda1 - diagonal1));

    return vec4(
        min(sqrt(2.0 * lambda1), 1024.0) * diagonalVector,
        min(sqrt(2.0 * lambda2), 1024.0) * vec2(diagonalVector.y, -diagonalVector.x)
    );
}

highp vec4 discardVec = vec4(0.0, 0.0, 2.0, 0.0);

float gaussianSplatStage(inout vec4 positionClip) {
    int instanceID = gl_InstanceID + u_indexOffset;

    ivec2 spatUV = getUVivec2(int(instanceID % int(u_splatCount)));
    uint texIdx = uint(texelFetch(u_splatIndexesTexture, spatUV, 0).r);

    vec2 posCoord = getDataUV(texIdx, 2, 0, u_textureSize);
    vec4 splatPosition = vec4( uintBitsToFloat(uvec4(texture(u_splatAttributeTexture, posCoord))) );
    float batchId = splatPosition.w;

    v_positionMC = splatPosition.xyz;

    vec4 splatViewPos = czm_modelView * vec4(splatPosition.xyz, 1.0);

    splatViewPos /= splatViewPos.w;

    vec4 clipPosition = czm_projection * splatViewPos;

    float clip = 1.2 * clipPosition.w;
    if (clipPosition.z < -clip || clipPosition.x < -clip || clipPosition.x > clip ||
        clipPosition.y < -clip || clipPosition.y > clip) {
        positionClip = vec4(0.0, 0.0, 2.0, 1.0);
        return batchId;
    }

    vec2 covCoord = getDataUV(texIdx, 2, 1, u_textureSize);
    uvec4 covariance = uvec4(texture(u_splatAttributeTexture, covCoord));

    positionClip = clipPosition;

    vec2 u1 = unpackHalf2x16(covariance.x) ;
    vec2 u2 = unpackHalf2x16(covariance.y);
    vec2 u3 = unpackHalf2x16(covariance.z);
    mat3 Vrk = mat3(u1.x, u1.y, u2.x, u1.y, u2.y, u3.x, u2.x, u3.x, u3.y);

    vec4 covVectors = calcCovVectors(
        splatViewPos.xyz,
        Vrk
    );

    if (dot(covVectors.xy, covVectors.xy) < 4.0 && dot(covVectors.zw, covVectors.zw) < 4.0) {
        gl_Position = discardVec;
        return batchId;
    }

    vec2 corner = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2) - 1.;

    positionClip += vec4((corner.x * covVectors.xy + corner.y * covVectors.zw) / czm_viewport.zw * positionClip.w, 0, 0);
    positionClip.z = clamp(positionClip.z, -abs(positionClip.w), abs(positionClip.w));

    v_vertPos = corner ;
    vColor = vec4(covariance.w & 0xffu, (covariance.w >> 8) & 0xffu, (covariance.w >> 16) & 0xffu, (covariance.w >> 24) & 0xffu) / 255.0;

    return  batchId;
}

void operation(vec4 operationType, vec4 color, vec4 selectedColor, inout vec4 vertexColor)
    {
        float right_2 = operationType.x * 0.5;
        float right_4 = right_2 * 0.5;
        float right_8 = right_4 * 0.5;
        float right_16 = right_8 * 0.5;
        float isSetColor = fract(right_2);
        if(isSetColor > 0.1)
        {
            vertexColor *= color;
        }
        float isPicked = fract(floor(right_2)* 0.5);
        if(isPicked > 0.1)
        {
            vertexColor *= selectedColor;
        }
        float isHide = fract(floor(right_4)* 0.5);
        if(isHide > 0.1)
        {
            vertexColor.a = 0.0;
        }
    }

void main() 
{ 
    vec4 vertexPos = vec4(1.0); 
    float batchId = gaussianSplatStage(vertexPos);
    gl_Position = vertexPos;

    #ifdef BatchTable
        vec4 operationType = s3m_batchTable_operation(batchId);
        vSecondColor = s3m_batchTable_pickColor(batchId);
        vec4 objsColor = s3m_batchTable_color(batchId);
        operation(operationType, objsColor, uSelColor, vColor);
    #endif
}
`
