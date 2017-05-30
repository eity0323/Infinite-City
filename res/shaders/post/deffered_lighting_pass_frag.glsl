# version 400

uniform sampler2D m_color;
uniform sampler2D m_pos;
uniform sampler2D m_norm;

in vec4 lightPos;
in vec4 lightColor;
in vec4 screenPos;

out vec4 fColor;

void main() {
	
	vec2 uvs = (screenPos.xy/screenPos.w + vec2(1.0))/2.0;
	
	vec4 color = texture2D(m_color, uvs);
	vec4 pos = texture2D(m_pos, uvs);

	float radius = lightPos.w;
	

	vec3 d = pos.xyz - lightPos.xyz;
	float dist = length(d);
	//if(dist > radius) discard;

	vec3 dn = normalize(d);

	vec3 norm = texture2D(m_norm, uvs).xyz;

	float str = max(dot(-dn, norm), 0);

	float br = 1 - (dist/radius);
	br = clamp(br, 0, 1);

	br = br*str;
	fColor = vec4(color.rbg*br, 1)*lightColor;
} 
