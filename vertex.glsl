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


void main()
{
    //Light in eye-position
    //vec4 lightPosEye = vec4(lightPosition.xyz, 1.0) *mvMatrix;

    // Get vertex position in eye coordinates
    // Must have it in same space as light
    vec4 vertexPositionEye4 = mvMatrix * vec4(posAttr, 1.0);
    vec3 vertexPositionEye3 = vertexPositionEye4.xyz / vertexPositionEye4.w;

    // Calculate vector (l) to the light source
    vec3 vectorToLightSource = normalize(lightPosition - vertexPositionEye3);

    // Transform the normal (n) to eye coordinates
    vec3 normalEye3 = normalize(nMatrix * normalAttr);

    // Calculate n dot l for diffuse lighting
    float diffuseLightWeightning = max(dot(normalEye3, vectorToLightSource), 0.0);

    vec3 diffuseReflectance = diffuseColor * diffuseLightWeightning;

    //col = colAttr * (vec4(ambientColor, 0.0));
    col = colAttr * (vec4(ambientColor, 0.0) + vec4(diffuseReflectance, 1.0));
    //col = colAttr * vec4(diffuseReflectance, 1.0);

    gl_Position = pMatrix*mvMatrix * vec4(posAttr, 1.0);

}
