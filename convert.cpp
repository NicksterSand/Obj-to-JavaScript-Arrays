#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum WordType{Verts, Indices, Normals, None};

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout << "Please enter a filename!" << std::endl;
		exit(1);
	}
	std::ifstream objFile;
	objFile.open(argv[1]);
	if(!objFile.is_open()){
		std::cout << "Error opening file '" << argv[1] << "'!" << std::endl;
		exit(1);
	}
	std::ofstream arrayFile ((argc < 3)?"arrays.txt":argv[2]);
	if(!arrayFile.is_open()){
		std::cout << "Error writing to file '" << ((argc < 3)?"arrays.txt":argv[2]) << "'!" << std::endl;
	}
	
	std::string word;
	WordType wordType = None;

	std::vector<std::string> vertices;
	std::vector<int> indices;
	std::vector<std::string> normalsTemp;
	std::vector<int> normalIndicesTemp;
	int slashCount;
	int index;
	while(objFile >> word){
		if(word == "v"){
			wordType = Verts;
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
						index = i;
					}
				}
			}
			indices.push_back(std::stoi(word.erase(word.find("/")))-1);
			//normalIndicesTemp.push_back(std::stoi(word.erase(index,word.length())));
		}else if(wordType == Normals){
			normalsTemp.push_back(word);
		}
	}
	
	arrayFile << "Vertices = [";
	for(auto it = vertices.begin(); it != vertices.end(); it++){
		arrayFile << *it << (it + 1 == vertices.end()?"]\n":", ");
	}

	arrayFile << "Indices = [";
	for(auto it = indices.begin(); it != indices.end(); it++){
		arrayFile << *it << (it + 1 == indices.end()?"]\n":", ");
	}
	
	objFile.close();
	arrayFile.close();
	return 0;
}
