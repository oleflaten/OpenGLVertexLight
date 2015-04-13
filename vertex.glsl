attribute highp vec3 posAttr;
attribute highp vec3 normalAttr;
attribute lowp vec4 colAttr;
uniform highp mat4 mvMatrix;
uniform highp mat4 pMatrix;
uniform highp mat3 nMatrix;

varying lowp vec4 col;

uniform highp vec3 lightPosition;
uniform lowp vec3 ambientColor;
uniform lowp vec3 diffuseColor;
uniform lowp vec3 specularColor;

//could come from the program
const float shininess = 32.0;

void main()
{
    //Light in eye-position - should have its own mvMatrix, else
    //the light will rotate with the triangle
    //vec4 lightPosEye = mvMatrix * vec4(lightPosition.xyz, 1.0);

    // Get vertex position in eye coordinates
    // Must have it in same space as light
    vec4 vertexPositionEye4 = mvMatrix * vec4(posAttr, 1.0);
    vec3 vertexPositionEye3 = vertexPositionEye4.xyz / vertexPositionEye4.w;

    // Calculate vector (l) to the light source
    vec3 vectorToLightSource = normalize(lightPosition - vertexPositionEye3);

    // Transform the normal (n) to eye coordinates
    vec3 normalEye3 = normalize(nMatrix * normalAttr);

    // Calculate the reflection vector (r)
    vec3 reflectionVector = normalize(reflect(-vectorToLightSource, normalEye3));

    // The camera in eye coordinates is located in the origin
    // and pointing along the negative z-axis.
    // Calculate viewVectorEye (v) in eye coordinates as
    // (0.0, 0.0, 0.0) - vertexPositionEye3
    vec3 viewVectorEye = -normalize(vertexPositionEye3);
    float rdotv = max(dot(reflectionVector, viewVectorEye), 0.0);
    float specularLightWeight = pow(rdotv, shininess);
    vec3 specularReflection = specularColor * specularLightWeight;

    // Calculate n dot l for diffuse lighting
    float diffuseLightWeightning = max(dot(normalEye3, vectorToLightSource), 0.0);

    vec3 diffuseReflectance = diffuseColor * diffuseLightWeightning;

    col = colAttr * (vec4(ambientColor, 0.0) + vec4(diffuseReflectance, 1.0) + vec4(specularReflection, 1.0));

    gl_Position = pMatrix*mvMatrix * vec4(posAttr, 1.0);
}
