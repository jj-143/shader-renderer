in vec2 uv;

layout (location = 0) out vec4 fragColor;

uniform sampler2D tDiffuse;
uniform float iTime;

// Translucent overlay animated horizontally
void main() {
   fragColor = texture(tDiffuse, uv);

   if (uv.x < 0.5f * sin(iTime) + 0.5f) {
      fragColor += vec4(vec3(0.1f), 0.0f);
   }
}