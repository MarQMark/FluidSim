#ifndef KIKAN_EAR_CLIPPING_H
#define KIKAN_EAR_CLIPPING_H

#include <vector>
#include "glm/glm.hpp"

namespace Kikan {
    float cross2d(glm::vec2 a, glm::vec2 b){
        return a.x * b.y - a.y * b.x;
    }

    bool isPointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c){
        glm::vec2 ab = b - a;
        glm::vec2 bc = c - b;
        glm::vec2 ca = a - c;

        glm::vec2 ap = p - a;
        glm::vec2 bp = p - b;
        glm::vec2 cp = p - c;

        float cross[3] = {
                cross2d(ab, ap),
                cross2d(bc, bp),
                cross2d(ca, cp),
        };

        if(cross[0] > 0.0f || cross[1] > 0.0f || cross[2] > 0.0f)
            return false;

        return true;
    }

/*
 * Uses Ear-Clipping Algorithm
 */
    int triangulate(std::vector<glm::vec2>& points, std::vector<unsigned int>& indices){
        if(points.size() < 3)
            return -1;

        //create index list for all points
        std::vector<unsigned int> pointIndices(points.size());
        for (int i = 0; i < points.size(); ++i)
            pointIndices[i] = i;

        unsigned int triangleIndexCount = 0;

        while (pointIndices.size() > 3){
            for (int i = 0; i < pointIndices.size(); ++i) {
                unsigned int a = pointIndices[i];
                unsigned int b = pointIndices[(i - 1 < 0 ? pointIndices.size() - 1 : i - 1)];
                unsigned int c = pointIndices[i + 1];

                glm::vec2 va = points[a];
                glm::vec2 vb = points[b];
                glm::vec2 vc = points[c];

                glm::vec2 vab = vb - va;
                glm::vec2 vac = vc - va;

                //test if points make up an ear
                if(cross2d(vab, vac) < 0.0f)
                    continue;

                //test if new triangle contains any points
                bool isEar = true;
                for (int j = 0; j < points.size(); ++j) {
                    if(j == a || j == b || j == c)
                        continue;

                    if(isPointInTriangle(points[j], va, vb, vc)){
                        isEar = false;
                        break;
                    }
                }

                //add triangle
                if(isEar){
                    indices[triangleIndexCount++] = b;
                    indices[triangleIndexCount++] = a;
                    indices[triangleIndexCount++] = c;

                    pointIndices.erase(pointIndices.begin() + i);
                    break;
                }
            }
        }
        indices[triangleIndexCount++] = pointIndices[0];
        indices[triangleIndexCount++] = pointIndices[1];
        indices[triangleIndexCount++] = pointIndices[2];

        return 0;
    }
}

#endif //KIKAN_EAR_CLIPPING_H
