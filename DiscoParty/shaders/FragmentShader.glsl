#version 330

in vec3 world_position;
in vec3 world_normal;

uniform vec3 eye_position;

uniform vec3 material_ke;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;
uniform int lightsNumber;

uniform  vec3 light_direction[70];
uniform  vec3 light_position[70];
uniform  vec3 light_color[70];

uniform vec3 SpotLightPosition[4];
uniform vec3 SpotLightDirection[4];
uniform vec3 SpotLightColor[4];

uniform int lightningMode;

// Output
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

vec3 PointLightContribution(vec3 lightPos, vec3 lightColor, vec3 L,vec3 H)
{
	vec3 color =  vec3(0,0,0);
    float light_radius = 1.5f;
    vec3 diffuse_light = material_kd * max(dot(world_normal, L), 0);
    float specular_aux;
    specular_aux = 0;
    if (dot(world_normal, L) > 0)
        specular_aux = 1;
    vec3 specular_light = material_ks * specular_aux * pow(max(dot(world_normal, H), 0), material_shininess);
    float att = 0.f;
    float dist = distance(lightPos,world_position);
    if(dist < light_radius) 
    {
        att = pow(light_radius - dist, 2);
        color = lightColor * (att * (diffuse_light + specular_light));
    }
    return color;
}

void main()
{
    vec3 V = normalize(eye_position - world_position);
    vec3 color = vec3(0,0,0), colorTemp = vec3(0, 0, 0);

    for(int i = 0;i<lightsNumber;i++)
    { 
        vec3 L = normalize( light_position[i] - world_position );  
        vec3 H = normalize( L + V );
        vec3 R = reflect(-L, world_normal);
        color += PointLightContribution(light_position[i], light_color[i], L, H);
        colorTemp += PointLightContribution(light_position[i], light_color[i], L, H);
    }

    if(lightningMode == 1)
    { 
        if(material_kd == vec3 (0,0,0) && material_ks == vec3(0,0,0) )
            colorTemp += material_ke;
    }
   
    for(int i = 0; i< 4;i++)
    {
        V = normalize( eye_position - world_position );
        vec3 L = normalize( SpotLightPosition[i] - world_position );
        vec3 H = normalize( L + V );
        vec3 R = reflect(-L, world_normal);
        color += SpotLightContribution(L, H, SpotLightPosition[i], SpotLightDirection[i], SpotLightColor[i]);
    }
  
    out_color = (lightningMode == 1) ? vec4(colorTemp, 0.5f) : vec4(color, 0.5f);
}


