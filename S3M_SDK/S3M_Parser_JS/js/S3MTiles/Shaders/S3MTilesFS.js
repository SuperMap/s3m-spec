define(function() {
    'use strict';
    return `
    #ifdef GL_OES_standard_derivatives
        #extension GL_OES_standard_derivatives : enable
    #endif
    #ifdef TexCoord
        uniform sampler2D uTexture;
        uniform float uTextureWidth;
        varying vec4 vTexCoord;
        varying vec4 vTexMatrix;
    #endif
    #ifdef TexCoord2
        uniform sampler2D uTexture2;
        uniform float uTexture2Width;
        varying vec4 vTexMatrix2;
    #endif
    #ifdef VertexNormal
        varying vec3 vNormalEC;
    #endif
        varying vec4 vColor;
    #ifdef TexCoord
        void calculateTexCoord(in vec2 inTexCoord, in float scale, in float XTran, in float YTran, in float fTile, in float mipLevel, inout vec2 outTexCoord)
        {
            vec2 fTexCoord = fract(inTexCoord);
            float offset = 1.0 * pow(2.0, mipLevel) / fTile;
            fTexCoord = clamp(fTexCoord, offset, 1.0 - offset);
            outTexCoord.x = (fTexCoord.x + XTran) * scale;
            outTexCoord.y = (fTexCoord.y + YTran) * scale;
        }
        vec4 sampleTexture(sampler2D curTexture, vec2 oriTexCoord, float texTileWidth, float fMaxMipLev, float fTexCoordScale, vec2 vecTexCoordTranslate)
        {
            vec2 realTexCoord;
            calculateTexCoord(oriTexCoord, fTexCoordScale, vecTexCoordTranslate.x, vecTexCoordTranslate.y, texTileWidth, 0.0, realTexCoord);
            vec4 color = texture2D(curTexture, realTexCoord.xy);
            return color;
        }
    #endif
    #ifdef TexCoord
        vec4 getTextureColor()
        {
            if(vTexMatrix.z < 0.0)
            {
                return vec4(1.0);
            }
            float texTileWidth = vTexMatrix.z * uTextureWidth;
            vec4 color = sampleTexture(uTexture, vTexCoord.xy, texTileWidth, vTexMatrix.w, vTexMatrix.z, vTexMatrix.xy);
            return color;
        }
    #endif
    #ifdef TexCoord2
        vec4 getTexture2Color()
        {
            if(vTexMatrix2.z < 0.0)
            {
                return vec4(1.0);
            }
            float texTileWidth = vTexMatrix2.z * uTexture2Width;
            vec4 color = sampleTexture(uTexture2, vTexCoord.zw, texTileWidth, vTexMatrix2.w, vTexMatrix2.z, vTexMatrix2.xy);
            color.r = clamp(color.r * 1.618, 0.0, 1.0);
            color.g = clamp(color.g * 1.618, 0.0, 1.0);
            color.b = clamp(color.b * 1.618, 0.0, 1.0);
            return color;
        }
    #endif
        void main()
        {
            vec4 color = vColor;
        #ifdef TexCoord
            color *= getTextureColor();
        #endif
        #ifdef TexCoord2
            color *= getTexture2Color();
        #endif
        #ifdef VertexNormal
            float diffuseIntensity = max(dot(czm_sunDirectionEC, normalize(vNormalEC)), 0.0);
            color = vec4(color.rgb * diffuseIntensity, color.a);
        #endif
            gl_FragColor = color;
        }
    `;
});