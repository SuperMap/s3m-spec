// Vertex program for fresnel reflections / refractions


	//mat4 scalemat = mat4(
	//				0.5,   0.0,   0.0, 0.5, 
	//               	 	0.0,	   -0.5,   0.0, 0.5,
	//				0.0,   0.0, 0.5, 0.5,
	//				0.0,   0.0,   0.0,   1.0);


//vec4 pos			: POSITION,
//vec4 normal		: NORMAL,
//vec2 tex			: TEXCOORD0,
		
//out vec4 oPos		: POSITION,
varying vec3 noiseCoord;// : TEXCOORD0,
varying vec4 projectionCoord;// : TEXCOORD1,
varying vec3 oEyeDir;// : TEXCOORD2, 
varying vec3 oNormal;// : TEXCOORD3, 

uniform mat4 worldViewProjMatrix;
uniform vec3 eyePosition; // object space
uniform float timeVal;
uniform float scale;  // the amount to scale the noise texture by
uniform float scroll; // the amount by which to scroll the noise
uniform float noise;  // the noise perturb as a factor of the  time
uniform vec2 bumpSpeed;

void main(void)
{
	vec4 oPos = worldViewProjMatrix * gl_Vertex;


	mat4 scalemat = mat4(0.5, 0.0, 0.0, 0.0,
					0.0, -0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0);
	projectionCoord = scalemat * oPos;
	noiseCoord.xy = gl_MultiTexCoord0.xy * scale + bumpSpeed * timeVal;
	noiseCoord.z = noise * timeVal;

	oEyeDir = gl_Vertex.xyz - eyePosition; 
	oNormal = gl_Normal.rgb;	

	gl_Position = oPos;

	//gl_Position = vec4(0.0, 0.0, 0.0. 0.0);
}
