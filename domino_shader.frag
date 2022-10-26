
// Shader-ul de fragment / Fragment shader  
 
 #version 400

in vec4 ex_Color;
uniform int codCol;
out vec4 out_Color;

uniform sampler2D myTexture;

void main(void)
  {
	if ( codCol==0 )
		out_Color = ex_Color;
	if ( codCol==1 )
		out_Color = texture(myTexture, tex_Coord);

  }
 