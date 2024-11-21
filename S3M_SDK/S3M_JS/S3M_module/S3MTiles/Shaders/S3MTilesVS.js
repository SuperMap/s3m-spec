export default `
    attribute vec4 aPosition;
#ifdef VertexColor
    attribute vec4 aColor;
#endif
#ifdef USE_TextureCoordMatrix
    attribute vec2 aTextureCoordMatrix;
#endif
#ifdef VertexNormal
    attribute vec3 aNormal;
#endif
#ifdef Instance
    attribute float instanceId;
#else
    attribute float batchId;
#endif 

#ifdef TextureAtlas
    attribute float aTextureBatchId0;
#endif

#ifdef TexCoord
    attribute vec4 aTexCoord0;
    varying vec4 vTexCoord;
    uniform mat4 uTexMatrix;
    uniform vec2 decodeTexCoord0Min;
#ifdef COMPUTE_TEXCOORD
#ifdef TextureAtlas
    uniform vec4 uTexAtlasDim;
    varying vec4 vTexAtlasTran;
    varying vec4 vTexAtlasScale;
    varying vec4 vTexAtlasSize;
    varying vec2 vMaxMipLevel;
#else
    uniform float uTexture0Width;
    varying vec4 vTexMatrix;
    varying vec4 vTexCoordTransform;
#endif    
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
    
    uniform vec4 uSelectedColor;
    uniform vec4 uFillForeColor;
    
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
    void getTextureMatrixFromZValue(in float nZ, inout float XTran, inout float YTran, inout float scale)
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
    

#ifdef TextureAtlas
    uniform highp sampler2D batchTextureAtlas; 
    uniform vec4 batchTextureAtlasStep; 
#ifdef SecTextureAtlas
    uniform highp sampler2D batchTextureAtlasSec; 
    uniform vec4 batchTextureAtlasStepSec; 
#endif
    vec2 computeAtlasSt(float batchId, vec4 step) 
    { 
        float stepX = step.x; 
        float centerX = step.y; 
        float numberOfAttributes = float(1); 
        return vec2(centerX + (batchId * numberOfAttributes * stepX), 0.5); 
    } 
    vec4 atlas_batchTable_xywh(float batchId, sampler2D texture, vec4 step) 
    { 
        vec2 st = computeAtlasSt(batchId, step); 
        st.x += step.x * float(0); 
        vec4 textureValue = texture2D(texture, st); 
        vec4 value = textureValue; 
        return value; 
    } 
    void getTexAtlasParameter(in vec4 xywh, in vec2 textureDim, inout vec2 translate, inout vec2 scale, inout vec2 texSize, inout float maxMipLevel)
    {
        float width = xywh.z;
        float height  = xywh.w;
        width *= 2.0 / 3.0;
        maxMipLevel = log2(min(width, height));
        scale.x = width / textureDim.x;
        scale.y = height / textureDim.y;
        translate.x = xywh.x;
        translate.y  = xywh.y;
        translate /= textureDim;
        texSize.x = width;
        texSize.y = height;
    }
#endif
    void main()
    {
      

    #ifdef TexCoord
        vTexCoord.xy = aTexCoord0.xy;
    #endif
    #ifdef COMPUTE_TEXCOORD
    #ifdef TextureAtlas
        if(aTextureBatchId0 < 0.0)
        {
            vMaxMipLevel.x = -1.0;
        }
        else
        {
            vec4 xywh = atlas_batchTable_xywh(aTextureBatchId0, batchTextureAtlas, batchTextureAtlasStep);
            getTexAtlasParameter(xywh, uTexAtlasDim.xy, vTexAtlasTran.xy, vTexAtlasScale.xy, vTexAtlasSize.xy, vMaxMipLevel.x);
        }
    #else
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
    #endif
    #ifdef TexCoord2
    #ifdef TextureAtlas
        if(aTextureBatchIdSec < 0.0)
        {
            vMaxMipLevel.y = -1.0;
        }
        else
        {
            vec4 xywh2 = atlas_batchTable_xywh(aTextureBatchIdSec, batchTextureAtlasSec, batchTextureAtlasStepSec);
            getTexAtlasParameter(xywh2, uTexAtlasDim.zw, vTexAtlasTran.zw, vTexAtlasScale.zw, vTexAtlasSize.zw, vMaxMipLevel.y);
        }
    #else
        vTexCoord.zw = aTexCoord1.xy;
        vTexMatrix2 = vec4(0.0,0.0,1.0,0.0);
        vTexCoordTransform.y = aTexCoord1.z;
            #ifdef USE_TextureCoordMatrix
                vTexCoordTransform.y = aTextureCoordMatrix.y;
            #endif
        if(vTexCoordTransform.y < -90000.0)
        {
            vTexMatrix2.z = -1.0;
        }
        getTextureMatrixFromZValue(floor(vTexCoordTransform.y), vTexMatrix2.x, vTexMatrix2.y, vTexMatrix2.z);
        vTexMatrix2.w = log2(uTexture1Width * vTexMatrix.z);
    #endif
    #endif
    #endif
    
    #ifdef COMPRESS_VERTEX
        vec4 vertexPos = vec4(1.0);
        vertexPos = decodePositionMin + vec4(aPosition.xyz, 1.0) * decodePositionNormConstant;
    #else
        vec4 vertexPos = aPosition;
    #endif

    // meshopt压缩 这里对照主版本的  主版本命名的都是通过下划线，但是插件获取uniform是通过函数，所以需要大写
    #ifdef TexCoord
        #ifdef COMPRESS_TEXCOORD
            #ifdef MeshOPT_Compress
                vec2 texCoord0;
                texCoord0.x = aTexCoord0.x * decodeTexCoord0vNormConstant.x;
                texCoord0.y = aTexCoord0.y * decodeTexCoord0vNormConstant.y;
                vTexCoord.xy = decodeTexCoord0Min + texCoord0.xy;
            #endif
        #else
            vTexCoord = aTexCoord0;
        #endif // COMPRESS_TEXCOORD
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
        vec4 operationType = s3m_batchTable_operation(index);
        vec4 objsColor = s3m_batchTable_color(index);
        operation(operationType, objsColor, uSelectedColor, vertexColor);
        vSecondColor = s3m_batchTable_pickColor(index);
        vec4 positionMC = vec4(vertexPos.xyz, 1.0);
        vColor = vertexColor;
        vPositionMC = positionMC;
        vPositionEC = (czm_modelView * positionMC).xyz;
    #ifdef VertexNormal
        vNormalEC = czm_normal * normal;
    #endif
        gl_Position = czm_modelViewProjection * vec4(vertexPos.xyz, 1.0);
    }
`;