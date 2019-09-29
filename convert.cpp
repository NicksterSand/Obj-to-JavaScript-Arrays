#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum WordType{Verts, Indices, Normals, None};

int main(int argc, char *argv[]){
	if(argc < 3){
		std::cout << "Not enough arguments!" << std::endl;
		std::cout << "Usage: "<< argv[0] << " OBJFILE USENORMALS(y/n) OUTPUTFILE(optional)" << std::endl;
		exit(1);
	}
	std::ifstream objFile;
	objFile.open(argv[1]);
	if(!objFile.is_open()){
		std::cout << "Error opening file '" << argv[1] << "'!" << std::endl;
		exit(1);
	}
	std::ofstream arrayFile ((argc < 4)?"arrays.txt":argv[3]);
	if(!arrayFile.is_open()){
		std::cout << "Error writing to file '" << ((argc < 4)?"arrays.txt":argv[3]) << "'!" << std::endl;
	}
	
	std::string word;
	WordType wordType = None;

	std::vector<std::string> vertices;
	std::vector<int> indices;
	std::vector<std::string> normalsTemp;
	std::vector<int> normalIndicesTemp;
	int slashCount;
	int index;
	int vertCount = 0;
	while(objFile >> word){
		if(word == "v"){
			wordType = Verts;
			vertCount++;
		}else if(word == "f"){
			wordType = Indices;
		}else if(word == "vn"){
			wordType = Normals;
		}else if(word=="vt" || word=="s" || word=="vp" || word=="o" || word=="g" || word=="mtllib" || word=="usemtl" || word=="#"){
			wordType = None;
		}else if(wordType == Verts){
			vertices.push_back(word);
		}else if(wordType == Indices){
			slashCount = 0;
			index = 0;
			for(uint i = 0; i < word.length(); i++){
				if(word[i] == '/'){
					slashCount++;
					if(slashCount == 2){
						index = i+1;
					}
				}
			}
			std::string wordTemp = word;
			indices.push_back((std::stoi(word.erase(word.find("/"))))-1);
			normalIndicesTemp.push_back(std::stoi(wordTemp.erase(0,index)) - 1);
		}else if(wordType == Normals){
			normalsTemp.push_back(word);
		}
	}
	std::string normals[vertCount * 3];
	std::string argv2 = argv[2];
	if(argv2.compare("n") == 0){
		arrayFile << "Vertices = [";
		for(auto it = vertices.begin(); it != vertices.end(); it++){
			arrayFile << *it << (it + 1 == vertices.end()?"]\n":", ");
		}
		arrayFile << "Indices = [";
		for(auto it = indices.begin(); it != indices.end(); it++){
			arrayFile << *it << (it + 1 == indices.end()?"]\n":", ");
		}
	}else{
		int i = 0;
		for(auto it = indices.begin(); it != indices.end(); it++){
			normals[(*it) * 3] = normalsTemp[normalIndicesTemp[i]*3];
			normals[((*it) * 3)+1] = normalsTemp[(normalIndicesTemp[i]*3)+1];
			normals[((*it) * 3)+2] = normalsTemp[(normalIndicesTemp[i]*3)+2];
			i++;
		}
		arrayFile << "Vertices = [";
		for(i = 0; i < vertCount; i++){
			arrayFile << vertices[i*3] << ", " << vertices[(i*3)+1] << ", " << vertices[(i*3)+2] << ", ";
			arrayFile << normals[i*3] << ", " << normals[(i*3)+1] << ", " << normals[(i*3)+2] << (i+1 == vertCount?"];\n":", ");
		}
		arrayFile << "Indices = [";
		for(auto it = indices.begin(); it != indices.end(); it++){
			arrayFile << *it << (it + 1 == indices.end()?"];\n":", ");
		}
	}
	
	std::cout << "Vertex " << (argv2.compare("n")!=0?"and normal ":"") << "data from " << argv[1] << " has been written to " << (argc < 4?"arrays.txt":argv[3]) << "." << std::endl;
	
	objFile.close();
	arrayFile.close();
	return 0;
}
