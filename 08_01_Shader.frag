// Shader-ul de fragment / Fragment shader  
#version 400

in vec3 ex_Color; 
out vec3 out_Color;
uniform int codCol;

void main(void)
{
    switch (codCol)
    {
        case 1: out_Color=vec3(0.0, 0.0, 0.0); break; // negru
        case 2: out_Color=vec3(1.0, 0.0, 0.0); break; // rosu
        case 3: out_Color=vec3(0.0, 1.0, 0.0); break; // verde
        case 4: out_Color=vec3(0.0, 0.0, 1.0); break; // albastru
        case 5: out_Color=vec3(1.0, 0.0, 1.0); break; // mov
        case 6: out_Color=vec3(1.0, 1.0, 0.0); break; // galben
        case 7: out_Color=vec3(1.0, 0.5, 0.0); break; // portocaliu
        case 8: out_Color=vec3(0.0, 1.0, 1.0); break; // cyan
        default: out_Color=ex_Color;
    }

}