define(function() {
    'use strict';
    return `
        attribute vec4 aPosition;
        attribute vec4 aTexCoord0;
    #ifdef TexCoord2
        attribute vec4 aTexCoord1;
    #endif
        varying vec4 vTexCoord;
        attribute vec4 aColor;
        varying vec4 vColor;
        void main()
        {
            vTexCoord.xy = aTexCoord0.xy;
        #ifdef TexCoord2
            vTexCoord.zw = aTexCoord1.xy;
        #endif
            vColor = aColor;
            gl_Position = czm_modelViewProjection * vec4(aPosition.xyz, 1.0);
        }
    `;
});