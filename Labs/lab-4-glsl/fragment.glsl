varying vec3 normal;
varying vec4 world_position;

void main()
{
    vec4 ambient = vec4(1, 0, 0, 1);
    vec4 diffuse = vec4(0, 1, 0, 1);
    vec4 specular = vec4(0, 0, 1, 1);
    
	vec3 lightDir = normalize(gl_LightSource[0].position.xyz - world_position.xyz);
	vec3 r = 2.0 * dot(lightDir, normal)*normal - lightDir;
	
	ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * max(dot(lightDir, normal), 0.0);
	specular = gl_LightSource[0].specular
			 * gl_FrontMaterial.specular
			 * pow(max(dot(normalize(-world_position.xyz), r), 0.0) , gl_FrontMaterial.shininess);

    gl_FragColor = ambient + diffuse + specular;
}
