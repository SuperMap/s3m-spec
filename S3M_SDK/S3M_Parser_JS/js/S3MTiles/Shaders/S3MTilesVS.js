define(function() {
    'use strict';
    return `
        attribute vec4 aPosition;
    #ifdef VertexNormal
        attribute vec3 aNormal;
        varying vec3 vNormalEC;
    #endif
    #ifdef TexCoord
        attribute vec4 aTexCoord0;
        varying vec4 vTexCoord;
        varying vec4 vTexMatrix;
    #endif
    #ifdef TexCoord2
        attribute vec4 aTexCoord1;
        varying vec4 vTexCoord2;
        varying vec4 vTexMatrix2;
    #endif
        attribute vec4 aColor;
        varying vec4 vColor;
        const float SHIFT_LEFT8 = 256.0;
        const float SHIFT_RIGHT8 = 1.0 / 256.0;
        const float SHIFT_RIGHT4 = 1.0 / 16.0;
        const float SHIFT_LEFT4 = 16.0;
        void getTextureMatrix(in float nZ, inout float XTran, inout float YTran, inout float scale)
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
        void main()
        {
        #ifdef TexCoord
            vTexCoord.xy = aTexCoord0.xy;
            vTexMatrix = vec4(0.0,0.0,1.0,0.0);
            if(aTexCoord0.z < -999.0)
            {
                vTexMatrix.z = -1.0;
            }
            getTextureMatrix(floor(aTexCoord0.z), vTexMatrix.x, vTexMatrix.y, vTexMatrix.z);
        #endif
        #ifdef TexCoord2
            vTexCoord.zw = aTexCoord1.xy;
            vTexMatrix2 = vec4(0.0,0.0,1.0,0.0);
            if(aTexCoord1.z < -999.0)
            {
                vTexMatrix2.z = -1.0;
            }
            getTextureMatrix(floor(aTexCoord1.z), vTexMatrix2.x, vTexMatrix2.y, vTexMatrix2.z);
        #endif
        #ifdef VertexNormal
            vNormalEC = czm_normal3D * aNormal;
        #endif
            vec4 positionMC = vec4(aPosition.xyz, 1.0);
            gl_Position = czm_modelViewProjection * positionMC;
            vColor = aColor;
        }
    `;
});