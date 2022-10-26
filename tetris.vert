// Shader-ul de varfuri  
#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Color;
layout(location=2) in vec2 in_Texture;

out vec4 gl_Position; // comentati daca este cazul!
out vec3 ex_Color;
out vec3 tex_Coord;

uniform int codCol;
uniform int codUmbra;
uniform mat4 viewShader;
uniform mat4 projectionShader;
uniform mat4 matrTransl;
uniform mat4 matrUmbra;

void main(void)
{
    if(codUmbra == 0)
    {
        if(codCol == 50)
        {
            mat4 newView = mat4(mat3(viewShader));
            vec4 pos = projectionShader*newView*matrTransl*in_Position;
            gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
            tex_Coord = vec3(in_Position.x, in_Position.y, -in_Position.z);
        }
        else
        {
            gl_Position = projectionShader*viewShader*matrTransl*in_Position;
            tex_Coord = vec3(in_Texture.x, in_Texture.y, 0);
        }
         
    }
    else
    {
        gl_Position = projectionShader*viewShader*matrUmbra*matrTransl*in_Position;
        tex_Coord = vec3(in_Texture.x, in_Texture.y, 0);
    }
    ex_Color = in_Color;
} 
