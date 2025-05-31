#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; // Pozycja fragmentu w �wiecie
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float ambientStrength;

// �wiat�o klucza
uniform vec3 keyLightPos;      // Pozycja klucza
uniform vec3 keyLightColor;    // Kolor �wiat�a (np. z�oty)
uniform float keyLightIntensity;

// Nowe uniformy
uniform vec3 mazeSize;         // Rozmiar labiryntu (width, height, depth)
uniform sampler2D wallMap;     // Tekstura okre�laj�ca gdzie s� �ciany (0.0 = pusto, 1.0 = �ciana)

// Funkcja sprawdza, czy mi�dzy �wiat�em a fragmentem jest �ciana
float isLightBlocked(vec3 fragPos, vec3 lightPos) {
    vec3 rayDir = normalize(lightPos - fragPos);
    float rayLength = distance(lightPos, fragPos);
    float stepSize = 0.1; // Zmniejsz krok dla wi�kszej dok�adno�ci
    
    // Rozpocznij od ma�ego offsetu od powierzchni (unikaj self-shadowingu)
    float t = 0.05; 
    
    while (t < rayLength) {
        vec3 samplePoint = fragPos + rayDir * t;
        
        // Konwersja do wsp�rz�dnych tekstury
        vec2 uv = (samplePoint.xz + 0.5) / mazeSize.xz; // +0.5 aby trafi� w �rodek texela
        uv = clamp(uv, 0.01, 0.99); // Unikaj brzeg�w tekstury
        
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
    
    // Standardowe o�wietlenie (bez zmian)
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // �wiat�o od klucza
    vec3 keyLightDir = normalize(keyLightPos - FragPos);
    float keyDiff = max(dot(norm, keyLightDir), 0.0);
    
    // Sprawd�, czy �wiat�o klucza jest widoczne (nie blokowane przez �ciany)
    float shadowFactor = 1.0 - isLightBlocked(FragPos, keyLightPos);
    
    // Oblicz �wiat�o klucza z uwzgl�dnieniem przeszk�d
    vec3 keyDiffuse = keyDiff * keyLightColor * keyLightIntensity * shadowFactor;
    
    // T�umienie �wiat�a (attenuation)
    float distance = length(keyLightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    keyDiffuse *= attenuation;
    
    // Finalny kolor
    vec3 result = (ambient + diffuse + keyDiffuse) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}