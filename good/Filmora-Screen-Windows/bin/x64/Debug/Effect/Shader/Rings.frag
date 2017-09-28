/********************************************************************
author: RuanShengQiang
date: 2017/3/21
********************************************************************/
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float Opacity;
uniform float Duration;
uniform float Enable;//if enable > 0.5, the effect is enabled;
uniform float Radius;
uniform vec3 Color;
uniform vec2 iMouse;
uniform float clicked; // it should be used type "bool", but the client only can transimit the type "float".If this variable is set to be">0.5", it shows the mouse is left-clicked. 
uniform float process;

void main()
{
    float maxSize = u_resolution.x>=u_resolution.y?u_resolution.x:u_resolution.y;
    float radius = Radius/1.2+0.01;
    vec3 rgb = Color;
    vec4 img = texture2D(mSample0,textureCoord);
    if(Enable>0.5)
    {
		float alpha1;
		float r = length(textureCoord*u_resolution - iMouse.xy)/maxSize;
		 r = r*(100.0/radius)*(1.0);
		if(clicked>0.5)
		{
			float x = process;
			
			//float x = fract(0.2);
			float a = 0.1+0.1*x;
			float b = 0.1+0.3*x;
			// float opacity1 = (6.0*x)*exp(-6.0*x)*2.7;//(6x)*exp(-6x)*2.7;
			float opacity1; // sin((1.34(1.0-x))*(1.34(1.0-x)+1))
			//float opacity2 = (6.0*(1.0-x))*exp(-6.0*(1.0-x))*2.7;//(6(1.0-x))*exp(-6(1.0-x))*2.7
			float opacity2;
			
			if(x > 0.0&&x<1.0)
			{
				opacity1 = sin((1.34*(1.0-x))*(1.34*(1.0-x)+1.0)); 
				opacity2 = sin((1.34*x)*(1.34*x+1.0));
				float temp = 2.0*3.1415926*b;
				float smoothEdge =  1.0-smoothstep(temp*0.8,temp*1.2,r);
				float expo=  clamp( (cos(r/a) - 0.1)*opacity1,0.0,1.0) + clamp( (cos(r/b) -0.1)*opacity2,0.0,1.0) ;
				alpha1 = clamp( expo*(0.2+Opacity),0.0,1.0)*smoothEdge;
				gl_FragColor = vec4(mix(img.rgb, rgb,alpha1),1.0);
			
			}else{
				gl_FragColor = img;
			}
		}else
		{
			gl_FragColor = img;
		}
	
    }else
    {
        gl_FragColor = img;
    }
}