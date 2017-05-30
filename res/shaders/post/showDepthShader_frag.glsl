# version 400

uniform sampler2D Texture0;

in vec2 uvs;

out vec4 fColor;

void main() { 
	float d = texture2D(Texture0, uvs).x;

	d = d*2.0 - 1.0;

	float m_near = 0.5;
	float m_far = 100.0;

	float depthw = -(2. * m_far * m_near) / (m_far - m_near);
	float depthz = -(m_far + m_near) / (m_far - m_near);

	d = depthw / (d + depthz);

	d = d/(m_far);

	fColor = vec4(d, d, d, 1);
} 
