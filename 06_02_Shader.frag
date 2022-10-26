
// Shader-ul de fragment / Fragment shader  
 
 #version 400

in vec3 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;


vec4 green= vec4(0.0,1.0,0.0,1.0);

uniform sampler2D myTexture;

void main(void)
  {
	if (codCol == 0)
		out_Color = ex_Color
		//out_Color = green;
	if (codCol == 1)
		out_Color = texture(myTexture, tex_Coord);

  }
 