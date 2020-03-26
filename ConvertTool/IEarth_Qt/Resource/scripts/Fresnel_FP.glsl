// Fragment program for distorting a texture using a 3D noise texture

varying vec3 noiseCoord;//			: TEXCOORD0,
varying vec4 projectionCoord;//		: TEXCOORD1,
varying vec3 oEyeDir;//				: TEXCOORD2,
varying vec3 oNormal;//				: TEXCOORD3,
		
//out float4 col		: COLOR,
		
uniform vec4 tintColour;
uniform vec4 waterColour;
uniform float noiseScale; 
uniform float minFresnel;
uniform float maxFresnel;
uniform float fresnelPower;
uniform float waterBrightness;
uniform sampler2D noiseMap;// : register(s0),
uniform sampler2D reflectMap;// : register(s1),
uniform sampler2D refractMap;// : register(s2)

vec4 AdjSaturation(in vec4 inputColor,in float saturation)
{
	vec3 lumCoeff = vec3(0.2125, 0.7154, 0.0721);
	vec3 intensity = vec3(dot(inputColor.rgb, lumCoeff));
	vec3 tempColor = mix(intensity, inputColor.rgb, saturation);
	return vec4(tempColor, 1.0);
}

void main(void)
{
	// Do the tex projection manually so we can distort _after_
	vec2 final = projectionCoord.xy / projectionCoord.w;

	// Noise
	vec3 noiseNormal = (texture2D(noiseMap, (noiseCoord.xy / 5.0)).rgb - 0.5).rbg * noiseScale;
	final += noiseNormal.xz;

	// Fresnel
	//oNormal = normalize(oNormal + noiseNormal.xz);
	float realMinFrenel, realMaxFrenel;
	if(minFresnel < maxFresnel)
	{
		realMinFrenel = minFresnel;
		realMaxFrenel = maxFresnel;
	}
	else
	{
		realMinFrenel = maxFresnel;
		realMaxFrenel = minFresnel;
	}
	float fresnelBias = realMinFrenel;
	float fresnelScale = (realMaxFrenel - realMinFrenel) / 1.0;
	float fresnel = fresnelBias + fresnelScale * pow(1.0 + dot(normalize(oEyeDir), oNormal), fresnelPower);
	fresnel = clamp(fresnel, 0.05, 0.95);

	// Reflection / refraction
	vec4 reflectionColour = texture2D(reflectMap, final);
	vec4 refractionColour = texture2D(refractMap, final) + tintColour;
	vec4 resultColor = mix(waterColour, reflectionColour, fresnel); 
	resultColor = AdjSaturation(resultColor, 1.0);
	resultColor = resultColor * waterBrightness;
	resultColor.a = waterColour.a;
	gl_FragColor = resultColor;
	
}
