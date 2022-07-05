export default `
    attribute vec4 aPosition;
#ifdef VertexColor
    attribute vec4 aColor;
#endif
#ifdef VertexNormal
    attribute vec3 aNormal;
#endif
#ifdef Instance
    attribute float instanceId;
#else
    attribute float batchId;
#endif 

#ifdef USE_VertexWeight
    attribute float aVertexWeight;
#endif

#ifdef USE_TextureCoordMatrix
    attribute vec2 aTextureCoordMatrix;
#endif

#ifdef TexCoord
    attribute vec4 aTexCoord0;
    varying vec4 vTexCoord;
    uniform mat4 uTexMatrix;
#ifdef COMPUTE_TEXCOORD
    uniform float uTexture0Width;
    varying vec4 vTexMatrix;
    varying vec4 vTexCoordTransform;
    varying vec2 vIsRGBA;
#endif
#endif

#ifdef TexCoord2
    attribute vec4 aTexCoord1;
    uniform float uTexture1Width;
    varying vec4 vTexMatrix2;
#endif
#ifdef InstanceBim
    attribute vec4 uv2;
    attribute vec4 uv3;
    attribute vec4 uv4;
    attribute vec4 secondary_colour;
    attribute vec4 uv6;   
#endif

#ifdef InstancePipe
    attribute vec4 uv1;
    attribute vec4 uv2;
    attribute vec4 uv3;
    attribute vec4 uv4;
    attribute vec4 uv5;
    attribute vec4 uv6;
    attribute vec4 uv7;
    attribute vec4 secondary_colour;
    attribute vec4 uv9;
#endif
    uniform vec4 uFillForeColor;
    uniform vec4 uSelectedColor;
    varying vec4 vSecondColor;
    varying vec4 vPositionMC;
    varying vec3 vPositionEC;
#ifdef VertexNormal
    varying vec3 vNormalEC;
#endif
    varying vec4 vColor;
    
    const float SHIFT_LEFT8 = 256.0;
    const float SHIFT_RIGHT8 = 1.0 / 256.0;
    const float SHIFT_RIGHT4 = 1.0 / 16.0;
    const float SHIFT_LEFT4 = 16.0;
    void getTextureMatrixFromZValue(in float nZ, inout float XTran, inout float YTran, inout float scale, inout float isRGBA)
    {
        if(nZ <= 0.0)
        {
            return;
        }
        float nDel8 = floor(nZ * SHIFT_RIGHT8);
        float nDel16 = floor(nDel8 * SHIFT_RIGHT8);
        float nDel20 = floor(nDel16 * SHIFT_RIGHT4);
        isRGBA = floor(nDel20);
        YTran = nZ - nDel8 * SHIFT_LEFT8;
        XTran = nDel8 - nDel16 * SHIFT_LEFT8;
        float nLevel = nDel16 - nDel20 * SHIFT_LEFT4;
        scale = 1.0 / pow(2.0, nLevel);
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
    
#ifdef COMPRESS_TEXCOORD
#ifdef TexCoord
    uniform vec2 decode_texCoord0_min;
#endif
#ifdef TexCoord2
    uniform vec2 decode_texCoord1_min;
#endif
#ifdef MeshOPT_Compress
    uniform vec3 decode_texCoord0_vNormConstant;
    uniform vec3 decode_texCoord1_vNormConstant;
#else
    uniform float decode_texCoord0_normConstant;
    uniform float decode_texCoord1_normConstant;
#endif
#endif

#ifdef COMPRESS_VERTEX
    uniform vec4 decode_position_min;
    uniform float decode_position_normConstant;
#endif

#ifdef COMPRESS_NORMAL
    uniform float normal_rangeConstant;
#endif
    void main()
    {
    #ifdef COMPRESS_VERTEX
        vec4 vertexPos = vec4(1.0);
        vertexPos = decode_position_min + vec4(aPosition.xyz, 1.0) * decode_position_normConstant;
    #else
        vec4 vertexPos = aPosition;
    #endif
    #ifdef TexCoord
    
    #ifdef COMPRESS_TEXCOORD
    #ifdef MeshOPT_Compress
        vec2 texCoord0;
        texCoord0.x = aTexCoord0.x * decode_texCoord0_vNormConstant.x;
        texCoord0.y = aTexCoord0.y * decode_texCoord0_vNormConstant.y;
        vTexCoord.xy = decode_texCoord0_min + texCoord0.xy;
    #else
        vTexCoord.xy = decode_texCoord0_min.xy + aTexCoord0.xy * decode_texCoord0_normConstant;
    #endif
    #else
        vTexCoord.xy = aTexCoord0.xy;
    #endif
    
    #ifdef COMPUTE_TEXCOORD
        vTexMatrix = vec4(0.0,0.0,1.0,0.0);
        vIsRGBA.x = 0.0;
        vTexCoordTransform.x = aTexCoord0.z;
    #ifdef USE_TextureCoordMatrix
        vTexCoordTransform.x = aTextureCoordMatrix.x;
    #endif
        if(vTexCoordTransform.x < -90000.0)
        {
            vTexMatrix.z = -1.0;
        }
        getTextureMatrixFromZValue(floor(vTexCoordTransform.x), vTexMatrix.x, vTexMatrix.y, vTexMatrix.z, vIsRGBA.x);
        vTexMatrix.w = log2(uTexture0Width * vTexMatrix.z);
    #endif
    #endif
    
    #ifdef TexCoord2
    
    #ifdef COMPRESS_TEXCOORD
    #ifdef MeshOPT_Compress
        vec2 texCoord1;
        texCoord1.x = aTexCoord1.x * decode_texCoord1_vNormConstant.x;
        texCoord1.y = aTexCoord1.y * decode_texCoord1_vNormConstant.y;
        vTexCoord.zw = decode_texCoord1_min + texCoord1.xy;
    #else
        vTexCoord.zw = decode_texCoord1_min.xy + aTexCoord1.xy * decode_texCoord1_normConstant;
    #endif
    #else
        vTexCoord.zw = aTexCoord1.xy;
    #endif
    
        vTexMatrix2 = vec4(0.0,0.0,1.0,0.0);
        vIsRGBA.y = 0.0;
        vTexCoordTransform.y = aTexCoord1.z;
    #ifdef USE_TextureCoordMatrix
        vTexCoordTransform.y = aTextureCoordMatrix.y;
    #endif
        if(vTexCoordTransform.y < -90000.0)
        {
            vTexMatrix2.z = -1.0;
        }
        getTextureMatrixFromZValue(floor(vTexCoordTransform.y), vTexMatrix2.x, vTexMatrix2.y, vTexMatrix2.z, vIsRGBA.y);
        vTexMatrix2.w = log2(uTexture1Width * vTexMatrix.z);
    #endif
    
        vec4 vertexColor = uFillForeColor;
    #ifdef VertexColor
        vertexColor *= aColor;
    #endif
    #ifdef VertexNormal
        vec3 normal = aNormal;
    #ifdef COMPRESS_NORMAL
    #ifdef MeshOPT_Compress
        normal.x = aNormal.x / 127.0;
        normal.y = aNormal.y / 127.0;
        normal.z = 1.0 - abs(normal.x) - abs(normal.y);
        normal = normalize(normal);
    #else
        normal = czm_octDecode(aNormal.xy, normal_rangeConstant).zxy;
    #endif
    #endif
    #endif
    #ifdef InstanceBim
        mat4 worldMatrix;
        worldMatrix[0] = uv2;
        worldMatrix[1] = uv3;
        worldMatrix[2] = uv4;
        worldMatrix[3] = vec4(0, 0, 0, 1);
        vertexPos = vec4(vertexPos.xyz,1.0) * worldMatrix;
        vertexColor *= secondary_colour; 
    #endif
    #ifdef InstancePipe
        mat4 worldMatrix;
        mat4 worldMatrix0;
        mat4 worldMatrix1;
        vec4 worldPos0;
        vec4 worldPos1;
        worldMatrix0[0] = uv1;
        worldMatrix0[1] = uv2;
        worldMatrix0[2] = uv3;
        worldMatrix0[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
        worldMatrix1[0] = uv4;
        worldMatrix1[1] = uv5;
        worldMatrix1[2] = uv6;
        worldMatrix1[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
        vec4 realVertex = vec4(vertexPos.xyz, 1.0);
        realVertex.x = realVertex.x * uv7.z;
        worldPos0 = realVertex * worldMatrix0;
        worldPos1 = realVertex * worldMatrix1;
        vertexColor *= secondary_colour; 
    #ifdef TexCoord
        if(aTexCoord0.y > 0.5)
        {
            vec4 tex4Vec = uTexMatrix * vec4(uv7.y, aTexCoord0.x, 0.0, 1.0);
            vTexCoord.xy = tex4Vec.xy;
            vertexPos = worldPos1;
            worldMatrix = worldMatrix1;
        }
        else
        {
            vec4 tex4Vec = uTexMatrix * vec4(uv7.x, aTexCoord0.x, 0.0, 1.0);
            vTexCoord.xy = tex4Vec.xy;
            vertexPos = worldPos0;
            worldMatrix = worldMatrix0;
        }
    #endif
    #ifdef VertexNormal
        normal.x = normal.x * uv7.z;
    #endif
    #endif
    #ifdef Instance  
        float index = instanceId;
    #else
        float index = batchId;
    #endif  
        vec4 operationType = batchTable_operation(index);
        operation(operationType, vec4(1.0), uSelectedColor, vertexColor);
        vSecondColor = batchTable_pickColor(index);
        vec4 positionMC = vec4(vertexPos.xyz, 1.0);
        vColor = vertexColor;
    #ifdef VertexNormal
        vNormalEC = czm_normal * normal;
    #endif
        vPositionMC = positionMC;
        vPositionEC = (czm_modelView * positionMC).xyz;
        gl_Position = czm_modelViewProjection * vec4(vertexPos.xyz, 1.0);
    }
`;