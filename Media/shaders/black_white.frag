uniform sampler2D source;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(source, gl_TexCoord[0].xy);

    // multiply it by the color
    float c = dot(pixel, vec4(0.299,0.587,0.184,0));
    gl_FragColor = vec4(c, c, c, pixel.a);
    //gl_FragColor = pixel;
}
