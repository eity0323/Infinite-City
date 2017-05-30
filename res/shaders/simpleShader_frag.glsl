# version 400

out vec4 fColor;
in vec4 color;
in vec4 globalPos;
in vec4 localPos;

void main() { 
	float depth = length(localPos.xyz) / 50.;

	depth = clamp(depth - .5, 0, 1);

	fColor = color*(1-depth);
} 
