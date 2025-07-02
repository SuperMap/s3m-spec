export default `
    attribute vec4 aPosition;
    attribute vec4 aColor;
    attribute vec3 aNormal;
#ifdef TexCoord
    attribute vec4 aTexCoord0;
    varying vec4 vTexCoord;
    uniform vec2 decodeTexCoord0Min;
#ifdef COMPUTE_TEXCOORD
    uniform float uTexture0Width;
    varying vec4 vTexCoordTransform;
    varying vec4 vTexMatrix;
#endif
#ifdef USE_TextureCoordMatrix
    attribute vec2 aTextureCoordMatrix;
#endif
#endif
#ifdef COMPRESS_VERTEX
    uniform vec4 decodePositionMin;
    uniform float decodePositionNormConstant;
#endif

// meshopt判断
#ifdef MeshOPT_Compress
    uniform vec3 decodeTexCoord0vNormConstant;
    uniform vec3 decode_texCoord1_vNormConstant;
#else
    uniform float decode_texCoord0_normConstant;
    uniform float decode_texCoord1_normConstant;
#endif

#ifdef HYPSOMETRIC
    varying float wValue;    
#endif
#ifdef FLATTEN
    uniform mat4 uGeoMatrix;
    uniform mat4 uInverseGeoMatrix;
    uniform sampler2D uFlattenTexture;
    uniform vec4 uFlattenRect;
#endif
    
    uniform vec4 uSelectedColor;
    
    varying vec4 vSecondColor;
    varying vec4 vPositionMC;
    varying vec3 vPositionEC;
#ifdef VertexColor
    varying vec4 vColor;
#endif
    
    const float SHIFT_LEFT8 = 256.0;
    const float SHIFT_RIGHT8 = 1.0 / 256.0;
    const float SHIFT_RIGHT4 = 1.0 / 16.0;
    const float SHIFT_LEFT4 = 16.0;
    void getTextureMatrixFromZValue( float nZ, inout float XTran, inout float YTran, inout float scale)
    {
        if(nZ <= 0.0)
        {
            return;
        }
        float nDel8 = floor(nZ * SHIFT_RIGHT8);
        float nDel16 = floor(nDel8 * SHIFT_RIGHT8);
        float nDel20 = floor(nDel16 * SHIFT_RIGHT4);
        YTran = nZ - nDel8 * SHIFT_LEFT8;
        XTran = nDel8 - nDel16 * SHIFT_LEFT8;
        float nLevel = nDel16 - nDel20 * SHIFT_LEFT4;
        scale = 1.0 / pow(2.0, nLevel);
    }
    
#ifdef FLATTEN
    float unpackValue(vec4 packedValue)
    {
        float SHIFT_LEFT16 = 65536.0;
        float SHIFT_LEFT8 = 256.0;
        vec4 value = packedValue * 255.0;
        return value.r * SHIFT_LEFT16 + value.g * SHIFT_LEFT8 + value.b - 9000.0;
    }
    vec4 calculateHeight(vec4 vertexPos)
    {
        vec4 vecPos = uGeoMatrix * vec4(vertexPos.xyz, 1.0);
        vec2 vecRatio = vec2(uFlattenRect.z - uFlattenRect.x, uFlattenRect.w - uFlattenRect.y);
        vec2 vecTexCoord = vec2(vecPos.x - uFlattenRect.x, vecPos.y - uFlattenRect.y);
        vecTexCoord.x = vecTexCoord.x / vecRatio.x;
        vecTexCoord.y = vecTexCoord.y / vecRatio.y;
        if(vecTexCoord.x > 1.0 || vecTexCoord.x < 0.0 || vecTexCoord.y > 1.0 || vecTexCoord.y < 0.0)
        {
            return vertexPos;
        }
        float fHeight = unpackValue(texture2D(uFlattenTexture, vecTexCoord.xy));
        fHeight = fHeight;
        if(vecPos.z > fHeight)
        {
            vecPos.z = fHeight;
            vecPos.w = vecPos.z;
        }
        return uInverseGeoMatrix * vec4(vecPos.xyz, 1.0);
    }
#endif
    void main()
    {

    #ifdef TexCoord
        vTexCoord.xy = aTexCoord0.xy;
    #ifdef COMPUTE_TEXCOORD
        vTexMatrix = vec4(0.0,0.0,1.0,0.0);
        vTexCoordTransform.x = aTexCoord0.z;
            #ifdef USE_TextureCoordMatrix
                vTexCoordTransform.x = aTextureCoordMatrix.x;
            #endif
        if(vTexCoordTransform.x < -90000.0)
        {
            vTexMatrix.z = -1.0;
        }
        getTextureMatrixFromZValue(floor(vTexCoordTransform.x), vTexMatrix.x, vTexMatrix.y, vTexMatrix.z);
        vTexMatrix.w = log2(uTexture0Width * vTexMatrix.z);
        // meshopt压缩 这里对照主版本的  主版本命名的都是通过下划线，但是插件获取uniform是通过函数，所以需要大写
        #ifdef MeshOPT_Compress
            vec2 texCoord0;
            texCoord0.x = aTexCoord0.x * decodeTexCoord0vNormConstant.x;
            texCoord0.y = aTexCoord0.y * decodeTexCoord0vNormConstant.y;
            vTexCoord.xy = decodeTexCoord0Min + texCoord0.xy;
        #endif
    #endif    
    #endif

    #ifdef COMPRESS_VERTEX
        vec4 vertexPos = vec4(1.0);
        vertexPos = decodePositionMin + vec4(aPosition.xyz, 1.0) * decodePositionNormConstant;
    #else
        vec4 vertexPos = aPosition;
    #endif
#ifdef FLATTEN
    vertexPos = calculateHeight(vertexPos);
#endif
    vertexPos.w = 1.0;
    #ifdef HYPSOMETRIC
        wValue = vertexPos.w;
    #endif
        vec4 positionMC = vec4(vertexPos.xyz, 1.0);
#ifdef VertexColor
        vColor = aColor;
#endif
        vPositionMC = positionMC;
        vPositionEC = (czm_modelView * positionMC).xyz;
        gl_Position = czm_modelViewProjection * vec4(vertexPos.xyz, 1.0);
    }
`;