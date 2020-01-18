#version 410

layout(location = 0) in vec3 position;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 normalMatrix;

uniform float cameraOffset;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float heightFactor;

uniform sampler2D rgbTexture;
uniform sampler2D grayScaleTexture;

uniform int texture_Width;
uniform int texture_Height;

out vec2 textureCoordinate;
out vec3 normalVector;
out vec3 lightVector;
out vec3 cameraVector;

/* Hello future reader,
 * If you are as unlucky as us, to be subjected to arbitrary library restrictions, whose sole purpose 
 * is to punish students due a feedback in Mera Lab, on his lack of skill in writing template code for an assignment.
 * Huge thanks goes to Yusuf Mucahit Cetinkaya : http://yusufmucahit.com/ for deserving the saying
 * "Kod yazabilseydi, asistan olmazdı"
 *
 * Here is the gist of whats going on in here,
 * so you don't have to suffer with the hellhole called OpenGL as we did,
 * - Read two textures in helper.cpp
      -- Slot 0 for height map , (just uses R channel for depth info)
      (You calculate final height - y axis value as = heightFactor * read R channel value)
      -- Slot 1 for actual color information
    - For each vertex calculate final position(with calculated y axis), multiply it with MVP matrix
    - Calculate where this vertex would land on a given texture (u-v coordinates)
    - For each vertex, you calculate something called vertex normal,
      which is average of at most 6 neighbor faces normals,(look for the commented figure down below)
    - For each vertex calculate camera and light vectors,(just difference vectors)

  As the final step , pass these to fragment shader as 'varying' variables, fragment shader will do the rest

 */

float get_Y_Value(inout vec3 neighbor){
    vec2 neighbor_Texture;
    neighbor_Texture.x = 1 - (float(neighbor.x) / (texture_Width + 1));
    neighbor_Texture.y = 1 - (float(neighbor.z) / (texture_Height + 1));

    neighbor_Texture.x -= cameraOffset;

    vec4 texture_height = texture(grayScaleTexture, neighbor_Texture);
    return heightFactor * texture_height.r;
}

void main()
{
    vec3 temp_Pos = position;

    float u = 1 - (float(temp_Pos.x) / (texture_Width + 1));
    float v = 1 - (float(temp_Pos.z) / (texture_Height + 1));

    u -= cameraOffset;

    textureCoordinate = vec2(u, v);

    vec4 texture_height = texture(grayScaleTexture, textureCoordinate);
    vec3 rgb_height = texture_height.xyz;

    temp_Pos.y = heightFactor * rgb_height.r;

    vec3 n0, n1, n2, n3, n4, n5;
    vec3 v0, v1, v2, v3, v4, v5;

    //  .. ----- n0 ----- n1
    //   |   /    |   /   |
    //  n5 ---- Pos ----- n2
    //   |   /   |    /   |
    //  n4 ----- n3 ----- ..

    n0 = vec3(temp_Pos.x    , 0, temp_Pos.z + 1);
    n1 = vec3(temp_Pos.x + 1, 0, temp_Pos.z + 1);
    n2 = vec3(temp_Pos.x + 1, 0, temp_Pos.z    );
    n3 = vec3(temp_Pos.x    , 0, temp_Pos.z - 1);
    n4 = vec3(temp_Pos.x - 1, 0, temp_Pos.z - 1);
    n5 = vec3(temp_Pos.x - 1, 0, temp_Pos.z    );

    n0.y = get_Y_Value(n0);
    n1.y = get_Y_Value(n1);
    n2.y = get_Y_Value(n2);
    n3.y = get_Y_Value(n3);
    n4.y = get_Y_Value(n4);
    n5.y = get_Y_Value(n5);

    v0 = n0 - temp_Pos;
    v1 = n1 - temp_Pos;
    v2 = n2 - temp_Pos;
    v3 = n3 - temp_Pos;
    v4 = n4 - temp_Pos;
    v5 = n5 - temp_Pos;
    
    vec3 normal = normalize(    vec3(normalMatrix * vec4(cross(v0, v1), 0)) +
                                vec3(normalMatrix * vec4(cross(v1, v2), 0)) +
                                vec3(normalMatrix * vec4(cross(v2, v3), 0)) +
                                vec3(normalMatrix * vec4(cross(v3, v4), 0)) +
                                vec3(normalMatrix * vec4(cross(v4, v5), 0)) +
                                vec3(normalMatrix * vec4(cross(v5, v0), 0)) );

    normalVector = normal;
    cameraVector = normalize(vec3(MV * (vec4(vec3(cameraPosition) - temp_Pos, 0))));
    lightVector = normalize(vec3(MV * vec4(lightPosition - temp_Pos, 0)));

    gl_Position = MVP * vec4(temp_Pos, 1.0);
}
