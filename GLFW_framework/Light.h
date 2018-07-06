#pragma once
struct Light {
	glm::vec3 position;
	glm::vec3 intensities; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
};

