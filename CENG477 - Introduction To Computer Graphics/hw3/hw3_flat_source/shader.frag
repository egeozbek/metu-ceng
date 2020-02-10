#version 410

out vec4 color;

uniform sampler2D rgbTexture;

/* To any future reader,
 * fragment shader part of this assignment is kept same
 * ever since this assignment was first given 5-6 years ago
 * do not bother to reinvent the wheel and just understand it and copy it
 */

in vec2 textureCoordinate;
in vec3 normalVector; // For Lighting computation
in vec3 lightVector; // Vector from Vertex to Light;
in vec3 cameraVector; // Vector from Vertex to Camera;

void main() {

  vec4 textureColor = texture(rgbTexture, textureCoordinate);

  vec4 ka = vec4(0.25f , 0.25f , 0.25f , 1.0f); // reflectance coeff. for ambient
  vec4 Ia = vec4(0.3f  , 0.3f  , 0.3f  , 1.0f); // light color for ambient

  vec4 ks = vec4(1.0f , 1.0f , 1.0f , 1.0f); // reflectance coeff. for specular
  vec4 Is = vec4(1.0f , 1.0f , 1.0f , 1.0f); // light color for specular
  float specExp = 100.0f; // specular exponent

  vec4 Id = vec4(1.0f , 1.0f , 1.0f , 1.0f); // light color for diffuse
  vec4 kd = vec4(1.0f , 1.0f , 1.0f , 1.0f); // reflectance coeff. for diffuse

  vec3 half_vector = normalize(cameraVector + lightVector);

  float alpha = max(dot(normalVector, half_vector), 0);
  float theta = max(dot(normalVector, lightVector), 0);

  vec4 ambient = Ia * ka;

  vec4 diffuse = Id * kd * theta;

  vec4 specular = Is * ks * pow(alpha, specExp);

  vec4 final_Color = textureColor * (ambient + diffuse + specular);

  color = clamp( textureColor * (ambient + diffuse + specular), 0.0, 1.0);
}
