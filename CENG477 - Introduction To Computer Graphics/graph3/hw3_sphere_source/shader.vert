#version 410

layout(location = 0) in vec3 position;

uniform mat4 MVP; 
uniform mat4 MV;
uniform mat4 normalMatrix;

uniform vec3 cameraPosition;
uniform float heightFactor;
uniform float cameraOffset;


uniform sampler2D rgbTexture;
uniform sampler2D grayScaleTexture;
uniform int texture_Width;
uniform int texture_Height;


out vec2 textureCoordinate; 
out vec3 normalVector;
out vec3 lightVector; 
out vec3 cameraVector;

uniform vec3 lightPosition;

/* Hello future reader,
 * I highly suggest you read hw3_flat_source/shader.vert file before you read this one.
 * Execution flow highly resembles flat surface, except for spherical coordinates,
 * For each vertex:
     - calculate texture coordinates,
     - calculate height using r-channel of heightmap * heightFactor
     - calculate spherical coordinates (alpha , beta) given x and z coordinates
     - calculate point on the sphere (x,y,z)
     - as center is (0,0,0) normal calculation is trivial
     - using this normal, add the height to the point as a vector
     - camera and light vectors are same as in flat version of the assignment
 */

 /* Note , this version contains a visual bug on southern pole,
  * we believe this is due to overlapping triangles in a crowded area,
  * We solved it by using index buffer object implementation, but it messed up visually, on height factor changes
  * therefore we reverted using our basic method, but if you believe you can handle them , go for it and let me know.
  */


vec2 computeSphericalParameters(vec2 twoDimentionalCoords){
    const float PI = 3.1415926535897932384626433832795;
    float horizontal_split_count = 250.0f;
    float vertical_split_count = 125.0f;
    float alpha = 2 * PI * (twoDimentionalCoords.x / horizontal_split_count);
    float beta = PI * (twoDimentionalCoords.y / vertical_split_count);
    return vec2(alpha , beta);
}
vec3 computeSmoothSphereCoordinates(vec2 sphericalCoordinates){
    const float sphere_radius = 350.0f;
    float z_sphere = sphere_radius * cos(sphericalCoordinates.y); //cos(beta) *
    float y_sphere = sphere_radius * sin(sphericalCoordinates.y) * sin(sphericalCoordinates.x); //sin(beta) * 
    float x_sphere = sphere_radius * sin(sphericalCoordinates.y) * cos(sphericalCoordinates.x); //sin(beta) * 

    return vec3(x_sphere , y_sphere , z_sphere);                                                                                       
}

vec3 calculateNormalFromVertex(vec3 vertexOnSmoothSphere){
    return normalize(vertexOnSmoothSphere);
}

vec3 calculateFinalPosition(vec3 vertexOnSmoothSphere , vec3 normalVector, float heightDifference){
    vec3 additionalHeight = normalVector * (heightDifference);
    return vertexOnSmoothSphere + additionalHeight;
}

void main()
{
    vec3 temp_Position = position;

    float horizontal_split_count = 250.0f;
    float vertical_split_count = 125.0f;

    float textureCoordinate_x = position.x / horizontal_split_count;
    textureCoordinate_x -= cameraOffset;
    float textureCoordinate_y = position.z / vertical_split_count;
    textureCoordinate = vec2(textureCoordinate_x, textureCoordinate_y);

    vec4 texture_height = texture(grayScaleTexture, textureCoordinate);
    float heightDifference = heightFactor * texture_height.r;

    vec2 sphericalParameters = computeSphericalParameters(position.xz);

    vec3 smoothSphereCoordinates = computeSmoothSphereCoordinates(sphericalParameters);

    normalVector = calculateNormalFromVertex(smoothSphereCoordinates);

    vec3 finalCoordinates = calculateFinalPosition(smoothSphereCoordinates , normalVector ,heightDifference );

    //normalVector = normalize(vec3(normalMatrix * vec4(normalVector, 1.0f)));
    //cameraVector = normalize(vec3(MV * (vec4(vec3(cameraPosition) - temp_Position, 1.0f))));
    cameraVector = normalize(cameraPosition - finalCoordinates);
    lightVector = normalize(lightPosition - finalCoordinates);
    //lightVector = normalize(vec3(MV * vec4(lightPosition - temp_Position, 1.0f)));

    gl_Position = MVP * vec4(finalCoordinates, 1.0);
}
