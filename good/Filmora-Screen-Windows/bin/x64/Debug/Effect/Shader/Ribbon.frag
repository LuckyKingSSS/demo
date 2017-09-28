/*{
	"GUID":"83A3EBFF-0A46-465b-B201-1E8A7DD5DA1B",
  "PARAMS":[
    {
      "NAME":"swing",
      "TYPE":"float",
      "DEFAULT":0.3,
      "MIN":0.0,
      "MAX":1.0
    },
    {
      "NAME":"frequency",
      "TYPE":"float",
      "DEFAULT":0.41,
      "MIN":0.0,
      "MAX":1.0
    }
  ]
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_swing;
uniform float u_frequency;

#define FILTER_PI 3.1415926

void main()
{
	vec2 tc = textureCoord;
	vec2 fragCoord = gl_FragCoord.xy;
  vec4 col;
  float fre = u_frequency/0.2*FILTER_PI/u_resolution.x;
  float y = fragCoord.y + u_swing * 100.0 * sin(fre * fragCoord.x);
  
  if(y < 0.0 || y >= u_resolution.y) 
  {
    col = vec4(0.0, 0.0, 0.0, 1.0);
  }
  else
  {
    col = texture2D(mSample0, vec2(fragCoord.x / u_resolution.x, (fragCoord.y/u_resolution.y + y )/ u_resolution.y));
  }
  
  gl_FragColor = col;
}

