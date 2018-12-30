#ifndef TRIANGULATION_H_INCLUDED
#define TRIANGULATION_H_INCLUDED

#include "../Geometry/geometry.h"

class BowyerWatson {
    // Bowyer-Watson triangulation algorithm
    public:
        BowyerWatson();
        BowyerWatson(std::vector<Point> points);
        void fromCoords(std::string path);
        std::vector<Point> pick(int n);
        void compute();
        void toEdges(std::string path);
        void toTriangles(std::string path);
        void showStruct();
    private:
        std::vector<Point> points;
        std::vector<Triangle> triangles;
        Triangle bigTriangle();
};


#endif // TRIANGULATION_H_INCLUDED
