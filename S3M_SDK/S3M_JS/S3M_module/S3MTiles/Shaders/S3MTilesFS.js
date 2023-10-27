export default `
#ifdef GL_OES_standard_derivatives
#extension GL_OES_standard_derivatives : enable
#endif
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif
    uniform vec4 uDiffuseColor;
    uniform vec4 uAmbientColor;
#ifdef TexCoord
    varying vec4 vTexCoord;
#ifdef COMPUTE_TEXCOORD
    uniform sampler2D uTexture;
#ifdef TextureAtlas
    uniform vec4 uTexAtlasDim;
    varying vec4 vTexAtlasTran;
    varying vec4 vTexAtlasScale;
    varying vec4 vTexAtlasSize;
    varying vec2 vMaxMipLevel;
#else
    uniform float uTexture0Width;
    varying vec4 vTexCoordTransform;
    varying vec4 vTexMatrix;
#endif
#endif
#endif
#ifdef NO_TEXCOORD
    uniform sampler2D uTexture;
#endif
    varying vec4 vColor;
    varying vec4 vSecondColor;
    varying vec4 vPositionMC;
    varying vec3 vPositionEC;
#ifdef VertexNormal
    varying vec3 vNormalEC;
#endif
#ifdef TexCoord2
    uniform sampler2D uTexture2;
    uniform float uTexture1Width;
    varying vec4 vTexMatrix2;
#endif 
    void calculateMipLevel(in vec2 inTexCoord, in float vecTile, in float fMaxMip, inout float mipLevel)
    {
        vec2 dx = dFdx(inTexCoord * vecTile);
        vec2 dy = dFdy(inTexCoord * vecTile);
        float dotX = dot(dx, dx);
        float dotY = dot(dy, dy);
        float dMax = max(dotX, dotY);
        float dMin = min(dotX, dotY);
        float offset = (dMax - dMin) / (dMax + dMin);
        offset = clamp(offset, 0.0, 1.0);
        float d = dMax * (1.0 - offset) + dMin * offset;
        mipLevel = 0.5 * log2(d);
        mipLevel = clamp(mipLevel, 0.0, fMaxMip - 1.62);
    }

    void calculateMipLevel(in vec2 inTexCoord, in vec2 vecTile, in float fMaxMip, inout float mipLevel)
    {
        vec2 dx = dFdx(inTexCoord * vecTile.x);
        vec2 dy = dFdy(inTexCoord * vecTile.y);
        float dotX = dot(dx, dx);
        float dotY = dot(dy, dy);
        float dMax = max(dotX, dotY);
        float dMin = min(dotX, dotY);
        float offset = (dMax - dMin) / (dMax + dMin);
        offset = clamp(offset, 0.0, 1.0);
        float d = dMax * (1.0 - offset) + dMin * offset;
        mipLevel = 0.5 * log2(d);
        mipLevel = clamp(mipLevel, 0.0, fMaxMip - 1.62);
    }
    
    void calculateTexCoord(in vec3 inTexCoord, in float scale, in float XTran, in float YTran, in float fTile, in float mipLevel, inout vec2 outTexCoord)
    {
        if(inTexCoord.z < -9000.0)
        {
            outTexCoord = inTexCoord.xy;
        }
        else
        {
            vec2 fTexCoord = fract(inTexCoord.xy);
            float offset = 1.0 * pow(2.0, mipLevel) / fTile;
            fTexCoord = clamp(fTexCoord, offset, 1.0 - offset);
            outTexCoord.x = (fTexCoord.x + XTran) * scale;
            outTexCoord.y = (fTexCoord.y + YTran) * scale;
        }
    }
    
    vec4 getTexColorForS3M(sampler2D curTexture, vec3 oriTexCoord, float texTileWidth, float fMaxMipLev, float fTexCoordScale, vec2 vecTexCoordTranslate)
    {
        vec4 color = vec4(1.0);
        float mipLevel = 0.0;
    #ifdef GL_OES_standard_derivatives
        calculateMipLevel(oriTexCoord.xy, texTileWidth, fMaxMipLev, mipLevel);
    #endif
        vec2 realTexCoord;
        calculateTexCoord(oriTexCoord, fTexCoordScale, vecTexCoordTranslate.x, vecTexCoordTranslate.y, texTileWidth, mipLevel, realTexCoord);
        if(oriTexCoord.z < -9000.0)
        {
            color = texture2D(curTexture, realTexCoord.xy);
        }
        else
        {
            #ifdef GL_EXT_shader_texture_lod
                color = texture2DLodEXT(curTexture, realTexCoord.xy, mipLevel);
            #else
                color = texture2D(curTexture, realTexCoord.xy, mipLevel);
            #endif
        }
        return color;
    }
#ifdef COMPUTE_TEXCOORD
#ifdef TextureAtlas
    vec4 getTextureAtlasColor(sampler2D texture, vec2 uv, vec2 texDim, vec2 texTran, vec2 texScale, float maxMipLevel)
    {
        if(maxMipLevel < 0.0)
        {
            return vec4(1.0);
        }
        vec4 colorCeil = vec4(1.0);
        float mipLevel = 0.0;
    #ifdef GL_OES_standard_derivatives
        calculateMipLevel(uv, texDim, maxMipLevel, mipLevel);
    #endif
        float ceilMipLevel = ceil(mipLevel);
        vec2 translate = vec2(texTran.x, texTran.y);
        float temp;
        if(ceilMipLevel > 0.0)
        {
            translate.x = texTran.x + texScale.x;
            temp = pow(2.0, ceilMipLevel - 1.0);
            translate.y = texTran.y + texScale.y * (temp - 1.0) / temp;
        }
        float scale = 1.0 / pow(2.0, ceilMipLevel);
        vec2 texcoord = fract(uv);
        float offsetX = pow(2.0, ceilMipLevel) / texDim.x;
        float offsetY = pow(2.0, ceilMipLevel) / texDim.y;
        texcoord.x = clamp(texcoord.x, 0.0 + offsetX, 1.0 - offsetX);
        texcoord.y = clamp(texcoord.y, 0.0 + offsetY, 1.0 - offsetY);
        texcoord.x = texcoord.x * texScale.x * scale + translate.x;
        texcoord.y = texcoord.y * texScale.y * scale + translate.y;
    #ifdef GL_EXT_shader_texture_lod
        colorCeil = texture2DLodEXT(texture, texcoord.xy, 0.0);
    #else
        colorCeil = texture2D(texture, texcoord.xy, 0.0);
    #endif
        vec4 colorFloor = vec4(1.0);
        float floorMipLevel = floor(mipLevel);
        translate = vec2(texTran.x, texTran.y);
        if(floorMipLevel > 0.0)
        {
            translate.x = texTran.x + texScale.x;
            temp = pow(2.0, floorMipLevel - 1.0);
            translate.y = texTran.y + texScale.y * (temp - 1.0) / temp;
        }
        scale = 1.0 / pow(2.0, floorMipLevel);
        texcoord = fract(uv);
        offsetX = pow(2.0, floorMipLevel) / texDim.x;
        offsetY = pow(2.0, floorMipLevel) / texDim.y;
        texcoord.x = clamp(texcoord.x, 0.0 + offsetX, 1.0 - offsetX);
        texcoord.y = clamp(texcoord.y, 0.0 + offsetY, 1.0 - offsetY);
        texcoord.x = texcoord.x * texScale.x * scale + translate.x;
        texcoord.y = texcoord.y * texScale.y * scale + translate.y;
    #ifdef GL_EXT_shader_texture_lod
        colorFloor = texture2DLodEXT(texture, texcoord.xy, 0.0);
    #else
        colorFloor = texture2D(texture, texcoord.xy, 0.0);
    #endif
        vec4 color = colorCeil * 0.5 + colorFloor * 0.5;
        return color;
    }
#else
    vec4 getTextureColor()
    {
        if(vTexMatrix.z < 0.0)
        {
            return vec4(1.0);
        }
        float texTileWidth0 = vTexMatrix.z * uTexture0Width;
        vec3 realTexCoord = vec3(vTexCoord.xy, vTexCoordTransform.x);
        vec4 FColor = getTexColorForS3M(uTexture, realTexCoord, texTileWidth0, vTexMatrix.w, vTexMatrix.z, vTexMatrix.xy);
    #ifdef TexCoord2
        float texTileWidth1 = vTexMatrix2.z * uTexture1Width;
        realTexCoord = vec3(vTexCoord.zw, vTexCoordTransform.y);
        vec4 SColor = getTexColorForS3M(uTexture2, realTexCoord, texTileWidth1, vTexMatrix2.w, vTexMatrix2.z, vTexMatrix2.xy);
        SColor.r = clamp(SColor.r, 0.0, 1.0);
        SColor.g = clamp(SColor.g, 0.0, 1.0);
        SColor.b = clamp(SColor.b, 0.0, 1.0);
        // return FColor * SColor;
        return FColor;
    #else
        return FColor;
    #endif
    }
#endif
#endif
    
    const float M_PI = 3.141592653589793;
    vec3 SRGBtoLINEAR3(vec3 srgbIn) 
    {
        return pow(srgbIn, vec3(2.2));
    }
    
    vec4 SRGBtoLINEAR4(vec4 srgbIn)
    {
        srgbIn = srgbIn ;
        vec3 linearOut = pow(srgbIn.rgb, vec3(2.2));
        return vec4(linearOut, srgbIn.a);
    }
    vec3 LINEARtoSRGB(vec3 linearIn)
    {
    #ifndef HDR 
        return pow(linearIn, vec3(1.0/2.2));
    #else
        return linearIn;
    #endif
    }
    
    vec3 fresnelSchlick2(vec3 f0, vec3 f90, float VdotH) 
    {
        return f0 + (f90 - f0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
    }
    
    float smithVisibilityG1(float NdotV, float roughness) 
    {
        float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
        return NdotV / (NdotV * (1.0 - k) + k);
    }
    
    float smithVisibilityGGX(float roughness, float NdotL, float NdotV) 
    {
        return smithVisibilityG1(NdotL, roughness) * smithVisibilityG1(NdotV, roughness);
    }
    
    float GGX(float roughness, float NdotH) 
    {
        float roughnessSquared = roughness * roughness;
        float f = (NdotH * roughnessSquared - NdotH) * NdotH + 1.0;
        return roughnessSquared / (M_PI * f * f);
    }

    vec3 computeNormal(in vec3 oriVertex)
    {
        vec3 normal = cross(vec3(dFdx(oriVertex.x), dFdx(oriVertex.y), dFdx(oriVertex.z)), vec3(dFdy(oriVertex.x), dFdy(oriVertex.y), dFdy(oriVertex.z)));
        normal = normalize(normal);
        return normal;
    }
    
    vec3 directionLight(vec3 normalEC) 
    {
        const vec3 SunLightColor = vec3(0.8);
        const vec3 AmbientLightColor = vec3(0.7);
        vec3 totalDiffuseLight = vec3(0.0);
        vec3 totalSpecularLight = vec3(0.0);
        float sunLightDiffuseWeight = max( dot( normalEC, czm_lightDirectionEC ), 0.0 );
        totalDiffuseLight = SunLightColor * sunLightDiffuseWeight;
        return uDiffuseColor.rgb * (totalDiffuseLight + AmbientLightColor * uAmbientColor.rgb) + totalSpecularLight;
    }
    #ifdef ADJUST_COLOR
        uniform float uBrightness;
        uniform float uContrast;
        uniform float uHue;
        uniform float uSaturation;
        uniform float uOneOverGamma;
        vec3 adjustColor(vec3 inputColor)
        {
            vec3 outputColor = inputColor;
            outputColor.rgb = mix(vec3(0.0), outputColor.rgb, uBrightness);
            outputColor.rgb = mix(vec3(0.5), outputColor.rgb, uContrast);
            outputColor.rgb = czm_hue(outputColor.rgb, uHue);
            outputColor.rgb = clamp(outputColor.rgb, 0.0, 1.0);
            outputColor.rgb = czm_saturation(outputColor.rgb, uSaturation);
            outputColor.rgb = pow(outputColor.rgb, vec3(uOneOverGamma));
            return outputColor;
        }
    #endif


    void main()
    {
        if(vColor.a < 0.1)
        {
            discard;
        } 
    #ifdef APPLY_SWIPE
        rollerShutter(gl_FragCoord.xy, uSwipeRegion);
    #endif
        vec4 baseColorWithAlpha = vColor;
    #ifdef COMPUTE_TEXCOORD
    #ifdef TextureAtlas
        baseColorWithAlpha *= getTextureAtlasColor(uTexture, vTexCoord.xy, vTexAtlasSize.xy, vTexAtlasTran.xy, vTexAtlasScale.xy, vMaxMipLevel.x);
    #else
        baseColorWithAlpha *= getTextureColor();
    #endif
    #endif
    #ifdef NO_TEXCOORD
        #ifndef REPLACE_COLOR_TYPE
            baseColorWithAlpha *= texture2D(uTexture, vec2(0.5));
        #endif
    #endif
        if(baseColorWithAlpha.a < 0.1)
        {
            discard;
        }
        vec4 outColor = baseColorWithAlpha;
    #ifndef USE_LINECOLOR    
        vec3 ng = vec3(0.0);
    #ifdef VertexNormal
        ng = normalize(vNormalEC);
    #endif
        ng = length(ng) > 0.1 ? ng : computeNormal(vPositionMC.xyz);
    #ifdef HAS_NORMAL_TEXTURE
        vec3 pos_dx = dFdx(vPositionEC);
        vec3 pos_dy = dFdy(vPositionEC);
        vec3 tex_dx = dFdx(vec3(vTexCoord.xy, 0.0));
        vec3 tex_dy = dFdy(vec3(vTexCoord.xy, 0.0));
        vec3 t = (tex_dy.t * pos_dx - tex_dx.t * pos_dy) / (tex_dx.s * tex_dy.t - tex_dy.s * tex_dx.t);
        t = normalize(t - ng * dot(ng, t));
        vec3 b = normalize(cross(ng, t));
        mat3 tbn = mat3(t, b, ng);
        vec3 n = texture2D(uNormalTexture, vTexCoord.xy).rgb;
        n = normalize(tbn * (2.0 * n - 1.0));
    #else
        vec3 n = ng;
    #endif
    vec3 baseColor = baseColorWithAlpha.rgb;
    outColor.rgb = baseColor * directionLight(n);
    #else
    outColor = baseColorWithAlpha * uDiffuseColor;
    #endif
    #ifdef ADJUST_COLOR
        outColor.rgb = adjustColor(outColor.rgb);
    #endif
        gl_FragColor = outColor;
    }
`;