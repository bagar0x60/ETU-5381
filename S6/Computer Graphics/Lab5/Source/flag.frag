uniform vec2 rectanglePosition;
uniform float stretch;
uniform float t;

void main()
{
    vec2 p = stretch*(gl_FragCoord.xy - rectanglePosition);  // now coordinates must be in [0;1000]x[0;600] range    
    
    // image have axial symmetry
    // so we only need to draw upper right part of image
    p = p - vec2(500.0, 300.0); // set center in the middle
    p = vec2(abs(p.x), abs(p.y));

    float h0 = t*60.0; // width of red and diagonal white strip
    float h1 = 100.0;  // width of horizontal white strip
    float k = 0.6; // 600 / 1000

    if (p.x < h0 || p.y < h0) {  
        gl_FragColor = vec4(0.76, 0.07, 0.16, 1.0); // red
    } else if ( (p.x < h1 || p.y < h1) || abs(k*p.x - p.y) < h0) {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);  // white
    } else {
        gl_FragColor = vec4(0.0, 0.22, 0.77, 1.0);  // blue
    }
}