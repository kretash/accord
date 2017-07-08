#version 330

in vec3 diffuse;
in vec3 frag_normal; 
out vec4 gl_FragColor; 

void main(){
    gl_FragColor.xyz = length(diffuse)*vec3(0.8f,0.8f,0.8f);
	gl_FragColor.a = 1.0f;
} 