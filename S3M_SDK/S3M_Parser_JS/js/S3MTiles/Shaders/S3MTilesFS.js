define(function() {
    'use strict';
    return `
        uniform sampler2D uTexture;
    #ifdef TexCoord2
        uniform sampler2D uTexture2;
    #endif
        uniform vec4 uDiffuseColor;
        uniform vec4 uAmbientColor;
        varying vec4 vColor;
        varying vec4 vTexCoord;
        void main()
        {
            vec4 color = vColor * texture2D(uTexture, vTexCoord.xy);
        #ifdef TexCoord2
            color *= texture2D(uTexture2, vTexCoord.zw);
        #endif
            gl_FragColor = czm_gammaCorrect(color);
        }
    `;
});