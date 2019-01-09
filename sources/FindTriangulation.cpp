#include "Triangulation/triangulation.h"
#include "Geometry/geometry.h"

using namespace std;

int main() {
    BowyerWatson bw;
    // reading points from a file with coordinates
    bw.fromCoords("input.txt");
    // computing the triangulation
    bw.compute();
    // writing to a file
    bw.toEdges("triang.txt");
}
