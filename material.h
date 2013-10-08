#include <string>

class Material{
	private:
		std::string name;
		float diffuse[4];
		float ambient[4];
		float specular[4];
		float shininess;
	
	public:
		Material(){}
		Material(std::string n);
	
		std::string getName(void);
		float* getDiffuse(void);
		float* getAmbient(void);
		float* getSpecular(void);
		float getShininess(void);
		
		void setDiffuse(float r, float g, float b);
		void setAmbient(float r, float g, float b);
		void setSpecular(float r, float g, float b);
		void setShininess(float n);
};