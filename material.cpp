#include "material.h"
#include <iostream>

Material::Material(std::string n){
	name = n;
	diffuse[3] = 1;
	ambient[3] = 1;
	specular[3] = 1;
}

std::string Material::getName(void){
	return name;
}

float* Material::getDiffuse(void){
	return diffuse;
}

float* Material::getAmbient(void){
	return ambient;
}

float* Material::getSpecular(void){
	return specular;
}

float Material::getShininess(void){
	return shininess;
}

void Material::setDiffuse(float r, float g, float b){
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}

void Material::setAmbient(float r, float g, float b){
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
}

void Material::setSpecular(float r, float g, float b){
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

void Material::setShininess(float n){
	shininess = n;
}