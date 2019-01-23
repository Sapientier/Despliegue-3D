const char* vertexShaderCode =
"#version 400\r\n"
""
"uniform mat4 view_matrix;" //Matriz de View
"uniform mat4 projection_matrix;" //Matriz de proyeccion
"uniform mat4 model_matrix;" //Matriz de model
"uniform vec3 TheColor;" //Color de BB y normales
"uniform int normal_o_bounding;" //si es normal o BB
"uniform vec3 lightPos;" //Posicion de luz
"uniform vec3 view;" //Vector de EYE
"uniform vec3 color_ambiental;" //Color Ambienta;
"uniform vec3 color_difuso;" //Color difuso
"uniform vec3 color_luza;" //Color Luz Ambiental
"uniform vec3 color_luzd;" //Color Luz Difuso
"uniform vec3 color_luze;" //Color Luz Especular
"uniform vec3 color_especular;" //Color espcular
"uniform float brillo_shiny;" //Color espcular
"uniform int tipo_de_shader;"
""
"in vec3 position;" //Posicion de los vertices
"in vec3 normal;" //Posicion de normales
"in vec3 normal_cara;" //Posicion de normales de cara
""
"out vec3 color_fragment;"
"out vec3 lightColor_a;"
"out vec3 lightColor_d;"
"out vec3 lightColor_e;"
"out vec3 FragPos;"
"out vec3 normales;"
"out vec3 lightPosfrag;"
"out vec3 viewPos;"
"out vec3 color_a;"
"flat out int n_or_bb;"
"out vec3 color_d;"
"out vec3 color_e;"
"out float brillo;"
"out vec3 guro_shader;"
"flat out int tipo;"
""
""
"void main()"
"{"
	//Posicion de los vertices
"   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);" //La posicion viene por la mult de matrices
"   n_or_bb = normal_o_bounding;"
"   color_fragment = TheColor;"
"   mat4 model = model_matrix;"
"   if(tipo_de_shader == 1){"
""
"       vec3 normales_g = mat3(transpose(inverse(model))) * normal;"
"       vec3 lightC =  color_luza;"
"       vec3 VecP = vec3(model * vec4(position, 1.0));"
		//Reflexion Ambiental
"       vec3 ambient = lightC * color_ambiental;"
""
""
		//Reflexion Difusa
"		lightC = color_luzd;"
"       vec3 norm = normalize(normales_g);"
"       vec3 lightDir = normalize(lightPos - VecP);"
"       float diff = max(dot(norm, lightDir), 0.0);"
"       vec3 diffuse = lightC * (diff * color_difuso);"
""
""
		//Refleccion Especular
"		lightC = color_luze;"
"       vec3 viewDir = normalize(view - VecP);"
"       vec3 halfwayDir = normalize(lightDir + viewDir);"
"       float spec = pow(max(dot(norm, halfwayDir), 0.0), brillo_shiny);"
"       vec3 specular = lightC * (spec * color_especular);"
""
""
		//Resultado final
"		guro_shader = (ambient + diffuse + specular);"
"		tipo = 1;"
""
"   }else if(tipo_de_shader == 2){"
""
"       FragPos = vec3(model * vec4(position, 1.0));" //
"       lightColor_a = color_luza;" //Color de la luz ambiental
"       lightColor_d = color_luzd;" //Color de la luz difusa
"       lightColor_e = color_luze;" //Color de la luz especular
"       lightPosfrag = lightPos;"
"       viewPos = view;"
"       color_a = color_ambiental;"
"       color_d = color_difuso;"
"       color_e = color_especular;"
"       brillo = brillo_shiny;"
"		normales = mat3(transpose(inverse(model))) * normal_cara;"
"		tipo = 2;"
""
"   }"
""
"}";
