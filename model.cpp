#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) {
    std::vector<Vec4f> verts_;
    std::vector<std::vector<Vec3i> > faces_;
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;

    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(!in.is_open()) {
        std::cerr << "open file err " <<filename << std::endl;
    }
    std::string line;
    while(!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec4f v;
            for(int i=0;i<3;i++) iss >> v[i];
            v[3] = 1.0f;
            verts_.push_back(v);
        } else if(!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for(int i=0;i<2;i++) iss >> uv[i];
            uv_.push_back(uv);
        } else if(!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f norm;
            for(int i=0;i<3;i++) iss >> norm[i];
            norms_.push_back(norm);
        } else if(!line.compare(0, 2, "f ")) {
            iss >> trash;
            std::vector<Vec3i> face;
            Vec3i tmp;
            while( iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for(int i = 0; i < 3; i++) tmp[i]--;
                face.push_back(tmp);
            }
            faces_.push_back(face);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# "
    << uv_.size() << " vn# " << norms_.size() << std::endl;

    triangleList.resize(faces_.size());
    triangleList.clear();

    for( int i = 0; i < faces_.size(); i++) {
        Triangle tmp;

        std::vector<Vec3i> face = faces_[i];
    
        tmp.v[0] = verts_[face[0][0]];
        tmp.v[1] = verts_[face[1][0]];
        tmp.v[2] = verts_[face[2][0]];

        tmp.texCoords[0] = uv_[face[0][1]];
        tmp.texCoords[1] = uv_[face[1][1]];
        tmp.texCoords[2] = uv_[face[2][1]];

        tmp.normal[0] = norms_[face[0][2]];
        tmp.normal[1] = norms_[face[1][2]];
        tmp.normal[2] = norms_[face[2][2]];

        triangleList.push_back(tmp);
    }

    vertNum = static_cast<int>(verts_.size());
    faceNum = static_cast<int>(faces_.size());
}

Model::~Model() {
}

int Model::nverts() {
    return vertNum;
}

int Model::nfaces() {
    return faceNum;
}


