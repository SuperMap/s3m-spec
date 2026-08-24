export default `#ifdef GL_OES_standard_derivatives 
#extension GL_OES_standard_derivatives : enable 
#endif 

varying vec2 v_vertPos;
varying vec4 vColor;
varying vec4 vSecondColor;

void gaussianSplatStage(inout vec4 color) {
    mediump float A = dot(v_vertPos, v_vertPos);
    if(A > 1.0)
        discard;
    mediump float scale = 4.0;
    mediump float B = exp(-A * scale) * (vColor.a);
    color = vec4(vColor.rgb * B, B);
}

void main() 
{ 
    vec4 finalColor = vec4(1.0); 
    gaussianSplatStage(finalColor);
    gl_FragColor = finalColor;
}
`
