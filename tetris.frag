// Shader-ul de fragment / Fragment shader  
#version 400

in vec3 ex_Color; 
in vec3 tex_Coord;

out vec4 out_Color;

uniform int codCol;
uniform int codUmbra;
uniform sampler2D myTexture;
uniform sampler2D grassTexture;
uniform samplerCube skybox;

void main(void)
{
    vec2 tex_Coord_2 = vec2(tex_Coord.x, tex_Coord.y);
    switch (codCol)
    {
        case 1: out_Color = vec4(0.0, 0.0, 0.0, 1.0); break; // negru
        case 2: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(1.0, 0.0, 0.0, 0.5), 0.5); break; // rosu
        case 3: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(0.0, 1.0, 0.0, 0.5), 0.5); break; // verde
        case 4: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(0.0, 0.0, 1.0, 0.5), 0.5); break; // albastru
        case 5: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(1.0, 0.0, 1.0, 0.5), 0.5); break; // mov
        case 6: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(1.0, 1.0, 0.0, 0.5), 0.5); break; // galben
        case 7: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(1.0, 0.5, 0.0, 0.5), 0.5); break; // portocaliu
        case 8: out_Color = mix(texture(myTexture, tex_Coord_2), vec4(0.0, 1.0, 1.0, 0.5), 0.5); break; // cyan
        case 10: out_Color = texture(myTexture, tex_Coord_2); break;
        case 11: out_Color = texture(grassTexture, tex_Coord_2); break;
        case 50: out_Color = texture(skybox, tex_Coord); break;
        
        default: out_Color=vec4(ex_Color.x, ex_Color.y, ex_Color.z, 1.0);
    }

    if(codUmbra == 1)
    {
        out_Color = vec4(0.15, 0.15, 0.15, 0.5);
    }

}