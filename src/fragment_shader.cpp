const char* fragmentShaderCode =
"#version 400\r\n"
""
"in vec3 color_fragment;" //Color de BB y normales
"in vec3 lightColor_a;" //Color luz ambiental
"in vec3 lightColor_d;" //Color luz difuso
"in vec3 lightColor_e;" //Color luz especular
"in vec3 normales;" //Normales
"flat in int n_or_bb;" //BB o normal
"flat in int tipo;" //Que shader es
"in vec3 guro_shader;"
"in vec3 lightPosfrag;" //Posicion de la luz
"in vec3 FragPos;" //Posicion del fragmento
"in vec3 viewPos;" //Posicion de la vista
"in vec3 color_a;" //Color ambiental
"in vec3 color_d;"//Color difuso
"in vec3 color_e;" //Color especular
"in float brillo;" //Brillo de la luz
""
"out vec4 color;" //Botamos un color
"void main()"
"{"
""
"	vec3 result = vec3(0.0, 0.0, 0.0);"
"	if(tipo == 1){"
"		result = guro_shader;"
"	}else if (tipo == 2){"
""
		//Refleccion Ambiental
"		vec3 ambient = lightColor_a * color_a;"
""
""
		//Refleccion Difusa
"		vec3 norm = normalize(normales);"
"		vec3 lightDir = normalize(lightPosfrag - FragPos);"
"		float diff = max(dot(norm, lightDir), 0.0);"
"		vec3 diffuse = lightColor_d * (diff * color_d);"
""
""
		//Refleccion Especular
"		vec3 viewDir = normalize(viewPos - FragPos);"
"		vec3 halfwayDir = normalize(lightDir + viewDir);"
"		float spec = pow(max(dot(norm, halfwayDir), 0.0), brillo);"
"		vec3 specular = lightColor_e * (spec * color_e);"
""
""
		//Resultado final
"		result = (ambient + diffuse + specular);"
""
"	}"
""
"	if(n_or_bb == 1) color = vec4(color_fragment , 1.0);"
"	else color = vec4(result,1.0);"
"}";