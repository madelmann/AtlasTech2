
/*
uniform sampler2D object;
uniform sampler2D outline;

uniform vec4 outlineColor;

varying vec2 texcoords;

void main(void)
{
   gl_FragColor = mix(texture2D(object,texcoords),outlineColor,texture2D(outline,texcoords).r);
}
*/


uniform sampler2D depth;
uniform float Near;
uniform float Far;

float LinearDepth(in float depth){
  return (2.0 * Near) / (Far + Near - depth * (Far - Near));
}

float IsEdge(in vec2 coords){
  float dxtex = 1.0 / 512.0;
  float dytex = 1.0 / 512.0;
  
  float depth0 = texture2D(depth,coords).r;
  float depth1 = texture2D(depth,coords + vec2(dxtex,0.0)).r;
  float depth2 = texture2D(depth,coords + vec2(0.0,-dytex)).r;
  float depth3 = texture2D(depth,coords + vec2(-dxtex,0.0)).r;
  float depth4 = texture2D(depth,coords + vec2(0.0,dytex)).r;
  
  float ddx = abs((depth1 - depth0) - (depth0 - depth3));
  float ddy = abs((depth2 - depth0) - (depth0 - depth4));
  return clamp((ddx + ddy - 0.01)*100.0,0.0,1.0);
}

varying vec2 texcoords;

void main(void)
{
  float dxtex = 1.0 / 512.0;
  float dytex = 1.0 / 512.0;

  float delta = IsEdge(texcoords);
  //Comment out for harder edges
  delta += IsEdge(texcoords + vec2(dxtex,0.0));
  delta += IsEdge(texcoords + vec2(0.0,-dytex));
  delta += IsEdge(texcoords + vec2(-dxtex,0.0));
  delta += IsEdge(texcoords + vec2(0.0,dytex));
  delta /= 5.0;
  //End comment out 
  
  vec4 color = vec4(0.0,0.0,0.0,1.0);
  color.r = delta;
  gl_FragColor = color;
}
