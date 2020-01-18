#version 410

// Output Color
out vec4 color;

uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;

// Texture-related data;
uniform sampler2D rgbTexture;
uniform sampler2D grayScaleTexture;
uniform int texture_Width;
uniform int texture_Height;

// Data from Vertex Shader
in vec2 textureCoordinate;
in vec3 normalVector; // For Lighting computation
in vec3 lightVector; // Vector from Vertex to Light;
in vec3 cameraVector; // Vector from Vertex to Camera;

void main() {

  // Assignment Constants below
  // get the texture color
  vec4 textureColor = texture(rgbTexture, textureCoordinate);

  // apply Phong shading by using the following parameters
  vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
  vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient

  vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
  vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
  int specExp = 100; // specular exponent

  vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
  vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse


  /****************************************************************************/
  vec3 h = normalize(cameraVector + lightVector);

  float alpha = clamp(dot(normalVector, h), 0, 1);
  float theta = clamp(dot(normalVector, lightVector), 0, 1);

  // compute ambient component
  vec4 ambient = Ia * ka;
  // compute diffuse component
  vec4 diffuse = Id * kd * theta;
  // compute specular component
  vec4 specular = Is * ks * pow(alpha, specExp);

  // compute the color using the following equation
  color = clamp( textureColor * (ambient + diffuse + specular), 0.0, 1.0);
}
