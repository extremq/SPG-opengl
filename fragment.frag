#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 lighting(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
              vec3 ambient, vec3 diffuse, vec3 specular, float specPower)
{
    vec3 lightDir = normalize(lightPos - pos);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);

    vec3 ambientColor = ambient;
    vec3 diffuseColor = diff * diffuse;
    vec3 specularColor = spec * specular;

    return ambientColor + diffuseColor + specularColor;
}

void main()
{
    vec3 ambient = vec3(0.2);
    vec3 diffuse = vec3(1.0, 0, 0);
    vec3 specular = vec3(0.8);
    float specPower = 32;

    vec3 color = lighting(pos, normal, lightPos, viewPos,
                         ambient, diffuse, specular, specPower);

    fragColor = vec4(color, 1.0);
}
