#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

uniform vec3 eye_position;

uniform vec3 material_ke;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;


uniform vec3 SpotLightPosition;
uniform vec3 SpotLightDirection;
uniform vec3 SpotLightColor;

uniform int tranzitie;

layout(location = 0) out vec4 out_color;

vec3 SpotLightContribution(vec3 L, vec3 H, vec3 lightPos, vec3 lightDir, vec3 lightColor)
{
    vec3 color;
    vec3 diffuse_light = material_kd * max(dot(world_normal, L), 0);

    float specular_aux = 0;
    if (dot(world_normal, L) > 0)
        specular_aux = 1;

    vec3 specular_light = material_ks * specular_aux *
        pow(max(dot(world_normal, H), 0), material_shininess);

    float light_att_factor = 0.4;
    float cut_off = radians(50.0f);
    float spot_light = dot(-L, lightDir);
    float spot_light_limit = cos(cut_off);
    if (spot_light > spot_light_limit) 
    {
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        light_att_factor = linear_att * linear_att*0.5;
    }

    float d = distance(world_position, lightPos);
    float factor;
    if (d == 0)
        factor = 1;
    else
        factor = 1 / (d * d);
    color =(material_ke +(light_att_factor * factor) * lightColor*( diffuse_light + specular_light )) ;
	return color;
}

void main()
{
    vec3 V = normalize(eye_position - world_position);
    vec3 color = vec3(0,0,0);
    vec3 color1 = vec3(0,0,0);
   
    V = normalize( eye_position - world_position );
    vec3 L = normalize( SpotLightPosition - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect(-L, world_normal);
    color += SpotLightContribution(L,H,SpotLightPosition, SpotLightDirection, SpotLightColor);
  
    if( tranzitie == 1 )
        out_color = vec4(color1, 0.5f);
    else
        out_color = vec4(color, 0.5f);
}


