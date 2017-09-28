uniform sampler2D mSample0;

varying vec2 textureCoord;

void main()
{
	vec2 tc = textureCoord;

	vec4 color = texture2D(mSample0, tc);

    gl_FragColor = color;
}
