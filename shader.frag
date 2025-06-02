#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float ambientStrength;

// Œwiat³o klucza
uniform vec3 keyLightPos;
uniform vec3 keyLightColor;
uniform float keyLightIntensity;

// Nowe uniformy
uniform vec3 mazeSize;
uniform sampler2D wallMap;

//uniformy portalu
uniform vec3 portalPos;
uniform float portalIntensity;


float isLightBlocked(vec3 fragPos, vec3 lightPos) {
    vec3 rayDir = normalize(lightPos - fragPos);
    float rayLength = distance(lightPos, fragPos);
    float stepSize = 0.1;
    
    float t = 0.05; 
    while (t < rayLength) {
        vec3 samplePoint = fragPos + rayDir * t;
        
        // Konwersja do wspó³rzêdnych tekstury
        vec2 uv = (samplePoint.xz + 0.5) / mazeSize.xz;
        uv = clamp(uv, 0.01, 0.99);
        
        float isWall = texture(wallMap, uv).r;
        
        if (isWall > 0.5) {
            return 1.0;
        }
        t += stepSize;
    }
    return 0.0;
}

void main() {
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // Standardowe oœwietlenie
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Œwiat³o od klucza
    vec3 keyLightDir = normalize(keyLightPos - FragPos);
    float keyDiff = max(dot(norm, keyLightDir), 0.0);
    
    // Sprawdzenie czy swiat³o nie jest blokowane przez œciany
    float shadowFactor = 1.0 - isLightBlocked(FragPos, keyLightPos);
    
    // Obliczenie œwiat³a klucza z uwzglêdnieniem przeszkód
    vec3 keyDiffuse = keyDiff * keyLightColor * keyLightIntensity * shadowFactor;

    // obliczenia swiatla dla portalu
    vec3 portalDir = normalize(portalPos - FragPos);
    float portalDot = max(dot(norm, portalDir), 0.0);
    float portalDist = length(portalPos - FragPos);
    float portalAttenuation = 1.0 / (1.0 + 0.2 * portalDist + 0.05 * portalDist * portalDist);
    vec3 portalColor = vec3(0.2, 1.0, 0.6); // jasnozielone œwiat³o
    vec3 portalDiffuse = portalColor * portalDot * portalAttenuation * portalIntensity;

    // T³umienie œwiat³a
    float distance = length(keyLightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    keyDiffuse *= attenuation;
    
    // Finalny kolor
    vec3 result = (ambient + diffuse + keyDiffuse + portalDiffuse) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}